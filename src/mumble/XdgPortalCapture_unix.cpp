// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

// CMake only compiles this file on Linux, so use __linux__ (compiler-native)
// rather than Q_OS_LINUX (Qt-defined) which requires a Qt header to be included first.
#if defined(__linux__) && defined(USE_SCREEN_SHARING) && defined(HAS_WAYLAND_PORTAL)

#	include "XdgPortalCapture.h"

#	include <QtCore/QObject>
#	include <QtCore/QPointer>
#	include <QtCore/QRandomGenerator>
#	include <QtCore/QString>
#	include <QtDBus/QDBusConnection>
#	include <QtDBus/QDBusInterface>
#	include <QtDBus/QDBusMessage>
#	include <QtDBus/QDBusMetaType>
#	include <QtDBus/QDBusPendingCall>
#	include <QtDBus/QDBusPendingCallWatcher>
#	include <QtDBus/QDBusPendingReply>
#	include <QtDBus/QDBusUnixFileDescriptor>
#	include <QtGui/QGuiApplication>
#	include <QtGui/QImage>

// PipeWire and SPA headers (system, not bundled — bundled headers lack spa/param/video/).
extern "C" {
#	include <pipewire/pipewire.h>
#	include <spa/param/video/format-utils.h>
#	include <spa/utils/result.h>
}

#	include <unistd.h> // dup()

// ---------------------------------------------------------------------------
// Constants
// ---------------------------------------------------------------------------
static constexpr uint32_t PORTAL_CURSOR_MODE_EMBEDDED = 2;
// Source type flags: 1=monitor, 2=window — request both.
static constexpr uint32_t PORTAL_SOURCE_TYPES = 3;

static constexpr char PORTAL_SERVICE[]    = "org.freedesktop.portal.Desktop";
static constexpr char PORTAL_OBJECT[]     = "/org/freedesktop/portal/desktop";
static constexpr char PORTAL_INTERFACE[]  = "org.freedesktop.portal.ScreenCast";
static constexpr char REQUEST_INTERFACE[] = "org.freedesktop.portal.Request";

// ---------------------------------------------------------------------------
// Helpers
// ---------------------------------------------------------------------------

/// Generates a unique token safe for use as a D-Bus object path component.
static QString makeToken() {
	return QStringLiteral("mumble_%1").arg(QRandomGenerator::global()->generate());
}


// ---------------------------------------------------------------------------
// PipeWire data structure
// ---------------------------------------------------------------------------
struct PwCapture {
	pw_thread_loop *loop    = nullptr;
	pw_context *context     = nullptr;
	pw_core *core           = nullptr;
	pw_stream *stream       = nullptr;
	spa_video_info_raw info = {};

	std::function< void() > onStarted;
	std::function< void(QImage) > onFrame;
	std::function< void(QString) > onError;

	bool started = false;
};

static void pw_on_param_changed(void *userdata, uint32_t id, const struct spa_pod *param) {
	auto *data = static_cast< PwCapture * >(userdata);
	if (id != SPA_PARAM_Format || !param)
		return;

	if (spa_format_video_raw_parse(param, &data->info) < 0)
		return;

	// Negotiate buffer parameters based on the negotiated video size.
	const uint32_t stride = SPA_ROUND_UP_N(data->info.size.width * 4u, 4u);
	const uint32_t size   = stride * data->info.size.height;

	uint8_t buf[512];
	struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buf, sizeof(buf));
	const struct spa_pod *params[1];
	params[0] = static_cast< const struct spa_pod * >(spa_pod_builder_add_object(
		&b, SPA_TYPE_OBJECT_ParamBuffers, SPA_PARAM_Buffers, SPA_PARAM_BUFFERS_buffers,
		SPA_POD_CHOICE_RANGE_Int(4, 2, 32), SPA_PARAM_BUFFERS_blocks, SPA_POD_Int(1), SPA_PARAM_BUFFERS_size,
		SPA_POD_Int(static_cast< int >(size)), SPA_PARAM_BUFFERS_stride, SPA_POD_Int(static_cast< int >(stride)),
		SPA_PARAM_BUFFERS_align, SPA_POD_Int(16)));

	pw_stream_update_params(data->stream, params, 1);
}

static void pw_on_process(void *userdata) {
	auto *data = static_cast< PwCapture * >(userdata);

	struct pw_buffer *buf = pw_stream_dequeue_buffer(data->stream);
	if (!buf)
		return;

	struct spa_buffer *sbuf = buf->buffer;
	if (sbuf->n_datas == 0) {
		pw_stream_queue_buffer(data->stream, buf);
		return;
	}
	void *pixels = sbuf->datas[0].data;
	if (!pixels || sbuf->datas[0].chunk->size == 0) {
		pw_stream_queue_buffer(data->stream, buf);
		return;
	}

	const int width         = static_cast< int >(data->info.size.width);
	const int height        = static_cast< int >(data->info.size.height);
	const int32_t rawStride = sbuf->datas[0].chunk->stride;
	const int stride =
		static_cast< int >(rawStride > 0 ? static_cast< uint32_t >(rawStride) : data->info.size.width * 4u);

	// The portal provides BGRx (kCVPixelFormatType_32BGRA equivalent on Linux) or RGBx.
	// BGRx on little-endian matches QImage::Format_ARGB32 memory layout (BB GG RR xx).
	QImage::Format fmt;
	switch (data->info.format) {
		case SPA_VIDEO_FORMAT_BGRx:
		case SPA_VIDEO_FORMAT_BGRA:
			fmt = QImage::Format_ARGB32;
			break;
		case SPA_VIDEO_FORMAT_RGBx:
		case SPA_VIDEO_FORMAT_RGBA:
			fmt = QImage::Format_RGBX8888;
			break;
		default:
			fmt = QImage::Format_ARGB32;
			break;
	}

	// Deep-copy immediately: the PipeWire buffer must be re-queued before we return.
	QImage copy = QImage(static_cast< const uchar * >(pixels), width, height, stride, fmt).copy();
	pw_stream_queue_buffer(data->stream, buf);

	const QImage rgba = copy.convertToFormat(QImage::Format_RGBA8888);

	// Fire the first-started callback exactly once, then deliver frames.
	// Both callbacks are dispatched to the Qt main thread.
	if (!data->started) {
		data->started = true;
		QMetaObject::invokeMethod(
			qApp, [cb = data->onStarted]() { cb(); }, Qt::QueuedConnection);
	}
	QMetaObject::invokeMethod(
		qApp, [cb = data->onFrame, rgba]() { cb(rgba); }, Qt::QueuedConnection);
}

static void pw_on_stream_state_changed(void *userdata, enum pw_stream_state /*old*/, enum pw_stream_state state,
									   const char *error) {
	auto *data = static_cast< PwCapture * >(userdata);
	if (state == PW_STREAM_STATE_ERROR && data->onError) {
		const QString msg = error ? QString::fromUtf8(error) : QStringLiteral("PipeWire stream error");
		QMetaObject::invokeMethod(
			qApp, [cb = data->onError, msg]() { cb(msg); }, Qt::QueuedConnection);
	}
}

// Zero-init via IIFE so -Wmissing-field-initializers never fires, even when
// PipeWire adds new fields in future versions.
static const struct pw_stream_events s_pwStreamEvents = [] {
	struct pw_stream_events e = {};
	e.version                 = PW_VERSION_STREAM_EVENTS;
	e.state_changed           = pw_on_stream_state_changed;
	e.param_changed           = pw_on_param_changed;
	e.process                 = pw_on_process;
	return e;
}();

// ---------------------------------------------------------------------------
// XdgPortalSession — manages the portal D-Bus handshake and PipeWire stream.
// Lives on (and must be used from) the Qt main thread.
// ---------------------------------------------------------------------------
class XdgPortalSession : public QObject {
	Q_OBJECT

public:
	explicit XdgPortalSession(std::function< void() > onStarted, std::function< void() > onCancelled,
							  std::function< void(QString) > onError, std::function< void(QImage) > onFrame,
							  QObject *parent = nullptr)
		: QObject(parent), m_onStarted(std::move(onStarted)), m_onCancelled(std::move(onCancelled)),
		  m_onError(std::move(onError)), m_onFrame(std::move(onFrame)) {}

	~XdgPortalSession() override { cleanup(); }

	void start() { createSession(); }

private:
	// Helper: send a portal call, get the returned request path, then subscribe
	// to org.freedesktop.portal.Request.Response on that exact path.
	void portalCall(const QString &method, QList< QVariant > args, const char *responseSlot) {
		QDBusMessage msg =
			QDBusMessage::createMethodCall(QString::fromLatin1(PORTAL_SERVICE), QString::fromLatin1(PORTAL_OBJECT),
										   QString::fromLatin1(PORTAL_INTERFACE), method);
		for (auto &a : args)
			msg << a;

		QDBusPendingCall call = QDBusConnection::sessionBus().asyncCall(msg);
		auto *watcher         = new QDBusPendingCallWatcher(call, this);
		connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, responseSlot](QDBusPendingCallWatcher *w) {
			w->deleteLater();
			QDBusPendingReply< QDBusObjectPath > reply = *w;
			if (reply.isError()) {
				m_onError(QStringLiteral("Portal call failed: %1").arg(reply.error().message()));
				return;
			}
			const QString path = reply.value().path();
			QDBusConnection::sessionBus().connect(QString::fromLatin1(PORTAL_SERVICE), path,
												  QString::fromLatin1(REQUEST_INTERFACE), QStringLiteral("Response"),
												  this, responseSlot);
		});
	}

	// ---- Phase 1: CreateSession -------------------------------------------
	void createSession() {
		QVariantMap options;
		options[QStringLiteral("session_handle_token")] = makeToken();
		options[QStringLiteral("handle_token")]         = makeToken();

		portalCall(QStringLiteral("CreateSession"), { QVariant::fromValue(options) },
				   SLOT(onCreateSessionResponse(uint, QVariantMap)));
	}

	// ---- Phase 2: SelectSources -------------------------------------------
	void selectSources() {
		QVariantMap options;
		options[QStringLiteral("handle_token")] = makeToken();
		options[QStringLiteral("types")]        = QVariant::fromValue(static_cast< quint32 >(PORTAL_SOURCE_TYPES));
		options[QStringLiteral("multiple")]     = false;
		options[QStringLiteral("cursor_mode")] =
			QVariant::fromValue(static_cast< quint32 >(PORTAL_CURSOR_MODE_EMBEDDED));

		portalCall(QStringLiteral("SelectSources"),
				   { QVariant::fromValue(QDBusObjectPath(m_sessionHandle)), QVariant::fromValue(options) },
				   SLOT(onSelectSourcesResponse(uint, QVariantMap)));
	}

	// ---- Phase 3: Start ---------------------------------------------------
	void startSession() {
		QVariantMap options;
		options[QStringLiteral("handle_token")] = makeToken();

		portalCall(QStringLiteral("Start"),
				   { QVariant::fromValue(QDBusObjectPath(m_sessionHandle)), QVariant(QString()),
					 QVariant::fromValue(options) },
				   SLOT(onStartResponse(uint, QVariantMap)));
	}

	// ---- Phase 4: OpenPipeWireRemote + stream setup -----------------------
	void openPipeWireRemote(quint32 nodeId) {
		QVariantMap options;
		QDBusMessage msg =
			QDBusMessage::createMethodCall(QString::fromLatin1(PORTAL_SERVICE), QString::fromLatin1(PORTAL_OBJECT),
										   QString::fromLatin1(PORTAL_INTERFACE), QStringLiteral("OpenPipeWireRemote"));
		msg << QVariant::fromValue(QDBusObjectPath(m_sessionHandle)) << options;

		QDBusPendingCall call = QDBusConnection::sessionBus().asyncCall(msg);
		auto *watcher         = new QDBusPendingCallWatcher(call, this);
		connect(watcher, &QDBusPendingCallWatcher::finished, this, [this, nodeId](QDBusPendingCallWatcher *w) {
			w->deleteLater();
			QDBusPendingReply< QDBusUnixFileDescriptor > reply = *w;
			if (reply.isError()) {
				m_onError(QStringLiteral("OpenPipeWireRemote failed: %1").arg(reply.error().message()));
				return;
			}
			const int rawFd = reply.value().fileDescriptor();
			if (rawFd < 0) {
				m_onError(QStringLiteral("OpenPipeWireRemote returned invalid fd"));
				return;
			}
			// dup() because QDBusUnixFileDescriptor closes its fd on destruction.
			const int fd = ::dup(rawFd);
			if (fd < 0) {
				m_onError(QStringLiteral("dup() of PipeWire fd failed"));
				return;
			}
			startPipeWireStream(fd, nodeId);
		});
	}

	// ---- PipeWire stream setup --------------------------------------------
	void startPipeWireStream(int fd, quint32 nodeId) {
		pw_init(nullptr, nullptr);

		m_pw.onStarted = m_onStarted;
		m_pw.onFrame   = m_onFrame;
		m_pw.onError   = m_onError;

		m_pw.loop = pw_thread_loop_new("mumble-screenshare-pw", nullptr);
		if (!m_pw.loop) {
			m_onError(QStringLiteral("Failed to create PipeWire thread loop"));
			::close(fd);
			return;
		}

		m_pw.context = pw_context_new(pw_thread_loop_get_loop(m_pw.loop), nullptr, 0);
		if (!m_pw.context) {
			m_onError(QStringLiteral("Failed to create PipeWire context"));
			::close(fd);
			return;
		}

		if (pw_thread_loop_start(m_pw.loop) < 0) {
			m_onError(QStringLiteral("Failed to start PipeWire thread loop"));
			::close(fd);
			return;
		}

		pw_thread_loop_lock(m_pw.loop);

		// Connect using the portal fd — PipeWire takes ownership of the fd.
		m_pw.core = pw_context_connect_fd(m_pw.context, fd, nullptr, 0);
		if (!m_pw.core) {
			pw_thread_loop_unlock(m_pw.loop);
			m_onError(QStringLiteral("Failed to connect to PipeWire (portal fd)"));
			return;
		}

		m_pw.stream = pw_stream_new(m_pw.core, "mumble-screenshare",
									pw_properties_new(PW_KEY_MEDIA_TYPE, "Video", PW_KEY_MEDIA_CATEGORY, "Capture",
													  PW_KEY_MEDIA_ROLE, "Screen", nullptr));
		if (!m_pw.stream) {
			pw_thread_loop_unlock(m_pw.loop);
			m_onError(QStringLiteral("Failed to create PipeWire stream"));
			return;
		}

		pw_stream_add_listener(m_pw.stream, &m_pwStreamListener, &s_pwStreamEvents, &m_pw);

		// Announce the video formats we accept.
		uint8_t buf[1024];
		struct spa_pod_builder b = SPA_POD_BUILDER_INIT(buf, sizeof(buf));
		const struct spa_pod *params[1];

		// SPA_RECTANGLE / SPA_FRACTION produce temporaries; take addresses of named locals
		// because C++ forbids taking the address of a temporary (unlike C99 compound literals).
		const struct spa_rectangle szDefault = SPA_RECTANGLE(1920, 1080);
		const struct spa_rectangle szMin     = SPA_RECTANGLE(1, 1);
		const struct spa_rectangle szMax     = SPA_RECTANGLE(8192, 8192);
		const struct spa_fraction fpsDefault = SPA_FRACTION(15, 1);
		const struct spa_fraction fpsMin     = SPA_FRACTION(0, 1);
		const struct spa_fraction fpsMax     = SPA_FRACTION(60, 1);

		// clang-format off
		params[0] = static_cast< const struct spa_pod * >(spa_pod_builder_add_object(
			&b,
			SPA_TYPE_OBJECT_Format,    SPA_PARAM_EnumFormat,
			SPA_FORMAT_mediaType,      SPA_POD_Id(SPA_MEDIA_TYPE_video),
			SPA_FORMAT_mediaSubtype,   SPA_POD_Id(SPA_MEDIA_SUBTYPE_raw),
			SPA_FORMAT_VIDEO_format,   SPA_POD_CHOICE_ENUM_Id(
				5,
				SPA_VIDEO_FORMAT_BGRx,
				SPA_VIDEO_FORMAT_BGRx,
				SPA_VIDEO_FORMAT_BGRA,
				SPA_VIDEO_FORMAT_RGBx,
				SPA_VIDEO_FORMAT_RGBA),
			SPA_FORMAT_VIDEO_size,     SPA_POD_CHOICE_RANGE_Rectangle(
				&szDefault, &szMin, &szMax),
			SPA_FORMAT_VIDEO_framerate, SPA_POD_CHOICE_RANGE_Fraction(
				&fpsDefault, &fpsMin, &fpsMax)));
		// clang-format on

		const int ret = pw_stream_connect(
			m_pw.stream, PW_DIRECTION_INPUT, nodeId,
			static_cast< enum pw_stream_flags >(PW_STREAM_FLAG_AUTOCONNECT | PW_STREAM_FLAG_MAP_BUFFERS), params, 1);

		pw_thread_loop_unlock(m_pw.loop);

		if (ret < 0) {
			m_onError(QStringLiteral("pw_stream_connect failed: %1").arg(QString::fromUtf8(spa_strerror(ret))));
		}
	}

	void cleanup() {
		if (m_pw.loop)
			pw_thread_loop_stop(m_pw.loop);

		if (m_pw.stream) {
			pw_stream_destroy(m_pw.stream);
			m_pw.stream = nullptr;
		}
		if (m_pw.core) {
			pw_core_disconnect(m_pw.core);
			m_pw.core = nullptr;
		}
		if (m_pw.context) {
			pw_context_destroy(m_pw.context);
			m_pw.context = nullptr;
		}
		if (m_pw.loop) {
			pw_thread_loop_destroy(m_pw.loop);
			m_pw.loop = nullptr;
		}
	}

private slots:
	void onCreateSessionResponse(uint response, const QVariantMap &results) {
		if (response != 0) {
			if (response == 1)
				m_onCancelled();
			else
				m_onError(QStringLiteral("CreateSession failed (response %1)").arg(response));
			return;
		}
		m_sessionHandle = results.value(QStringLiteral("session_handle")).toString();
		if (m_sessionHandle.isEmpty()) {
			m_onError(QStringLiteral("CreateSession returned empty session handle"));
			return;
		}
		selectSources();
	}

	void onSelectSourcesResponse(uint response, const QVariantMap & /*results*/) {
		if (response != 0) {
			if (response == 1)
				m_onCancelled();
			else
				m_onError(QStringLiteral("SelectSources failed (response %1)").arg(response));
			return;
		}
		startSession();
	}

	void onStartResponse(uint response, const QVariantMap &results) {
		if (response != 0) {
			if (response == 1)
				m_onCancelled();
			else
				m_onError(QStringLiteral("Start failed (response %1)").arg(response));
			return;
		}

		// results["streams"] is a(ua{sv}) — array of structs {node_id, properties}.
		// In Qt D-Bus this arrives as QDBusArgument; extract the first stream's node_id.
		const QVariant streamsVar = results.value(QStringLiteral("streams"));
		if (!streamsVar.isValid()) {
			m_onError(QStringLiteral("Portal Start response contained no streams"));
			return;
		}

		// The QDBusArgument for a(ua{sv}) needs to be iterated manually.
		quint32 nodeId = UINT32_MAX;
		if (streamsVar.canConvert< QDBusArgument >()) {
			const QDBusArgument arg = streamsVar.value< QDBusArgument >();
			arg.beginArray();
			if (!arg.atEnd()) {
				arg.beginStructure();
				arg >> nodeId;
				arg.endStructure();
			}
			arg.endArray();
		}

		if (nodeId == UINT32_MAX) {
			m_onError(QStringLiteral("Could not extract PipeWire node ID from portal response"));
			return;
		}

		openPipeWireRemote(nodeId);
	}

private:
	std::function< void() > m_onStarted;
	std::function< void() > m_onCancelled;
	std::function< void(QString) > m_onError;
	std::function< void(QImage) > m_onFrame;

	QString m_sessionHandle;

	PwCapture m_pw;
	spa_hook m_pwStreamListener = {};
};

// ---------------------------------------------------------------------------
// Global session — only one active at a time.
// ---------------------------------------------------------------------------
static XdgPortalSession *g_session = nullptr;

// ---------------------------------------------------------------------------
// Public C++ API
// ---------------------------------------------------------------------------

bool xdg_portal_isNativePickerAvailable() {
	// We require Qt to be running under a Wayland compositor.
	if (QGuiApplication::platformName() != QLatin1String("wayland"))
		return false;

	// Check that the portal service is registered on the session bus.
	QDBusInterface iface(QString::fromLatin1(PORTAL_SERVICE), QString::fromLatin1(PORTAL_OBJECT),
						 QStringLiteral("org.freedesktop.DBus.Introspectable"), QDBusConnection::sessionBus());
	return iface.isValid();
}

void xdg_portal_startCapture(std::function< void() > onStarted, std::function< void() > onCancelled,
							 std::function< void(QString) > onError, std::function< void(QImage) > onFrame) {
	xdg_portal_stop();

	g_session =
		new XdgPortalSession(std::move(onStarted), std::move(onCancelled), std::move(onError), std::move(onFrame));
	g_session->start();
}

void xdg_portal_stop() {
	delete g_session;
	g_session = nullptr;
}

#	include "XdgPortalCapture_unix.moc"

#endif // __linux__ && USE_SCREEN_SHARING && HAS_WAYLAND_PORTAL
