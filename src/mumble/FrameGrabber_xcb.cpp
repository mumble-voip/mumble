// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "FrameGrabber.h"

#include <QtCore/QRect>
#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>

#include <cstdlib>
#include <memory>
#include <mutex>
#include <vector>

#include <sys/ipc.h>
#include <sys/shm.h>
#include <xcb/shm.h>
#include <xcb/xcb.h>

namespace {

/// Owns the X connection together with the shared memory segments the X server writes the pictures to.
///
/// Every grabbed image uses a segment as its memory, which comes back to the pool once the last copy of the image
/// is gone. That may happen on any thread (usually the encoder's), and only after the grabber is gone, which is why
/// the pool is shared by the grabber and all segments in use. libxcb is thread-safe, so detaching a segment works
/// from any thread.
class XcbConnection {
public:
	struct Segment {
		xcb_shm_seg_t id = 0;
		void *addr       = nullptr;
		std::size_t size = 0;
		/// Keeps the pool alive while the segment is in use
		std::shared_ptr< XcbConnection > owner;
	};

	explicit XcbConnection(xcb_connection_t *connection) : m_connection(connection) {}

	~XcbConnection() {
		for (Segment *segment : m_free)
			destroySegment(segment);
		xcb_disconnect(m_connection);
	}

	xcb_connection_t *get() const { return m_connection; }

	/// Returns a segment of the given size, or nullptr if all are in use or no new one can be created.
	Segment *acquire(std::size_t size, const std::shared_ptr< XcbConnection > &self) {
		std::lock_guard< std::mutex > lock(m_mutex);

		if (size != m_size) {
			// The picture size changed, segments of the old size are of no use anymore
			for (Segment *segment : m_free)
				destroySegment(segment);
			m_count -= m_free.size();
			m_free.clear();
			m_size = size;
		}

		Segment *segment = nullptr;
		if (!m_free.empty()) {
			segment = m_free.back();
			m_free.pop_back();
		} else if (m_count < MAX_SEGMENTS) {
			segment = createSegment(size);
			if (!segment)
				return nullptr;
			++m_count;
		} else {
			return nullptr;
		}

		segment->owner = self;
		return segment;
	}

	/// Hands a segment back. May be called from any thread.
	static void release(void *info) {
		Segment *segment = static_cast< Segment * >(info);
		// Keeps the pool alive until it is done with the segment, even if this was the last reference
		const std::shared_ptr< XcbConnection > owner = std::move(segment->owner);

		std::lock_guard< std::mutex > lock(owner->m_mutex);
		if (segment->size == owner->m_size) {
			owner->m_free.push_back(segment);
		} else {
			owner->destroySegment(segment);
			--owner->m_count;
		}
	}

	/// Whether segments can be shared with the X server at all, i.e. MIT-SHM is available and the server runs on
	/// this machine.
	bool supportsShm() {
		const xcb_query_extension_reply_t *extension = xcb_get_extension_data(m_connection, &xcb_shm_id);
		if (!extension || !extension->present)
			return false;

		// The server only fails to attach a segment when it can't access it (e.g. when it runs elsewhere)
		Segment *segment = createSegment(4096);
		if (!segment)
			return false;
		destroySegment(segment);
		return true;
	}

private:
	/// The encoder keeps up to three pictures (the one being encoded, the next one and the last one for repeating
	/// it as a key frame). A few more allow for encoders that hold on to their input for a moment.
	static constexpr std::size_t MAX_SEGMENTS = 6;

	Segment *createSegment(std::size_t size) {
		const int shmId = shmget(IPC_PRIVATE, size, IPC_CREAT | 0600);
		if (shmId < 0)
			return nullptr;

		void *addr = shmat(shmId, nullptr, 0);
		if (addr == reinterpret_cast< void * >(-1)) {
			shmctl(shmId, IPC_RMID, nullptr);
			return nullptr;
		}

		const xcb_shm_seg_t id           = xcb_generate_id(m_connection);
		xcb_generic_error_t *attachError = xcb_request_check(
			m_connection, xcb_shm_attach_checked(m_connection, id, static_cast< std::uint32_t >(shmId), 0));
		// Both the server and this process have attached the segment now (or the server never will), so it can be
		// marked for removal. It is only actually removed once both have detached it.
		shmctl(shmId, IPC_RMID, nullptr);
		if (attachError) {
			std::free(attachError);
			shmdt(addr);
			return nullptr;
		}

		Segment *segment = new Segment();
		segment->id      = id;
		segment->addr    = addr;
		segment->size    = size;
		return segment;
	}

	void destroySegment(Segment *segment) {
		xcb_shm_detach(m_connection, segment->id);
		xcb_flush(m_connection);
		shmdt(segment->addr);
		delete segment;
	}

	xcb_connection_t *m_connection;

	std::mutex m_mutex;
	std::vector< Segment * > m_free;
	/// Number of segments, free or in use
	std::size_t m_count = 0;
	/// Size of the pictures currently grabbed
	std::size_t m_size = 0;
};

class XcbFrameGrabber : public FrameGrabber {
public:
	XcbFrameGrabber(std::shared_ptr< XcbConnection > connection, xcb_window_t root, xcb_window_t window,
					const QRect &area, QImage::Format format, bool useShm)
		: m_connection(std::move(connection)), m_root(root), m_window(window), m_area(area), m_format(format),
		  m_useShm(useShm) {}

	Result grab(QImage &image) override {
		xcb_connection_t *connection = m_connection->get();
		if (xcb_connection_has_error(connection))
			return Result::Failed;

		QRect area = m_area;
		if (m_window != XCB_WINDOW_NONE) {
			// The window may have been moved, resized, minimised or closed since the last picture
			xcb_get_window_attributes_reply_t *attributes =
				xcb_get_window_attributes_reply(connection, xcb_get_window_attributes(connection, m_window), nullptr);
			if (!attributes)
				return Result::Failed;
			const bool viewable = attributes->map_state == XCB_MAP_STATE_VIEWABLE;
			std::free(attributes);
			if (!viewable)
				return Result::Unchanged;

			xcb_get_geometry_reply_t *geometry =
				xcb_get_geometry_reply(connection, xcb_get_geometry(connection, m_window), nullptr);
			if (!geometry)
				return Result::Failed;
			area = QRect(0, 0, geometry->width, geometry->height);
			std::free(geometry);

			// Only the part of the window that is on the screen can be grabbed
			xcb_translate_coordinates_reply_t *position = xcb_translate_coordinates_reply(
				connection, xcb_translate_coordinates(connection, m_window, m_root, 0, 0), nullptr);
			if (!position)
				return Result::Failed;
			const QRect onScreen = QRect(position->dst_x, position->dst_y, area.width(), area.height())
									   .intersected(QRect(QPoint(0, 0), rootSize()))
									   .translated(-position->dst_x, -position->dst_y);
			std::free(position);
			area = onScreen;
		}

		if (area.isEmpty())
			return Result::Unchanged;

		const xcb_drawable_t drawable = m_window != XCB_WINDOW_NONE ? m_window : m_root;
		const int stride              = area.width() * 4;

		if (m_useShm) {
			XcbConnection::Segment *segment = m_connection->acquire(
				static_cast< std::size_t >(stride) * static_cast< std::size_t >(area.height()), m_connection);
			if (!segment) {
				// The encoder still holds on to all pictures, so it couldn't take a new one anyway
				return Result::Unchanged;
			}

			xcb_generic_error_t *error         = nullptr;
			xcb_shm_get_image_reply_t *picture = xcb_shm_get_image_reply(
				connection,
				xcb_shm_get_image(connection, drawable, static_cast< std::int16_t >(area.x()),
								  static_cast< std::int16_t >(area.y()), static_cast< std::uint16_t >(area.width()),
								  static_cast< std::uint16_t >(area.height()), ~0u, XCB_IMAGE_FORMAT_Z_PIXMAP,
								  segment->id, 0),
				&error);
			// There is neither a reply nor an error when the connection broke
			const bool grabbed = picture && !error;
			std::free(picture);
			if (!grabbed) {
				std::free(error);
				XcbConnection::release(segment);
				return grabError();
			}

			image = QImage(static_cast< uchar * >(segment->addr), area.width(), area.height(), stride, m_format,
						   &XcbConnection::release, segment);
			return Result::Frame;
		}

		xcb_generic_error_t *error     = nullptr;
		xcb_get_image_reply_t *picture = xcb_get_image_reply(
			connection,
			xcb_get_image(connection, XCB_IMAGE_FORMAT_Z_PIXMAP, drawable, static_cast< std::int16_t >(area.x()),
						  static_cast< std::int16_t >(area.y()), static_cast< std::uint16_t >(area.width()),
						  static_cast< std::uint16_t >(area.height()), ~0u),
			&error);
		if (error || !picture) {
			std::free(error);
			std::free(picture);
			return grabError();
		}

		// The image uses the reply as its memory, which saves copying it
		image = QImage(xcb_get_image_data(picture), area.width(), area.height(), stride, m_format, &std::free, picture);
		return Result::Frame;
	}

private:
	QSize rootSize() const {
		const xcb_setup_t *setup = xcb_get_setup(m_connection->get());
		for (xcb_screen_iterator_t it = xcb_setup_roots_iterator(setup); it.rem; xcb_screen_next(&it)) {
			if (it.data->root == m_root)
				return QSize(it.data->width_in_pixels, it.data->height_in_pixels);
		}
		return QSize();
	}

	/// A window is grabbed again with the next frame, as it may just have changed while it was being grabbed. Only if
	/// it is gone, the capture fails.
	Result grabError() {
		if (m_window == XCB_WINDOW_NONE)
			return Result::Failed;

		xcb_connection_t *connection = m_connection->get();
		xcb_get_window_attributes_reply_t *attributes =
			xcb_get_window_attributes_reply(connection, xcb_get_window_attributes(connection, m_window), nullptr);
		std::free(attributes);
		return attributes ? Result::Unchanged : Result::Failed;
	}

	std::shared_ptr< XcbConnection > m_connection;
	xcb_window_t m_root;
	/// The grabbed window, or XCB_WINDOW_NONE when grabbing m_area of the root window
	xcb_window_t m_window;
	QRect m_area;
	QImage::Format m_format;
	bool m_useShm;
};

/// Returns the QImage format matching the pixels of the given visual and depth, or QImage::Format_Invalid if
/// there is none that can be passed on as it is.
QImage::Format formatOf(const xcb_setup_t *setup, xcb_visualid_t visualId, std::uint8_t depth) {
	if (setup->image_byte_order != XCB_IMAGE_ORDER_LSB_FIRST)
		return QImage::Format_Invalid;

	bool is32Bit = false;
	for (xcb_format_iterator_t it = xcb_setup_pixmap_formats_iterator(setup); it.rem; xcb_format_next(&it)) {
		if (it.data->depth == depth)
			is32Bit = it.data->bits_per_pixel == 32;
	}
	if (!is32Bit)
		return QImage::Format_Invalid;

	for (xcb_screen_iterator_t screen = xcb_setup_roots_iterator(setup); screen.rem; xcb_screen_next(&screen)) {
		for (xcb_depth_iterator_t d = xcb_screen_allowed_depths_iterator(screen.data); d.rem; xcb_depth_next(&d)) {
			for (xcb_visualtype_iterator_t visual = xcb_depth_visuals_iterator(d.data); visual.rem;
				 xcb_visualtype_next(&visual)) {
				if (visual.data->visual_id != visualId)
					continue;

				// Alpha is ignored either way, as there is nothing behind the shared picture
				if (visual.data->red_mask == 0xff0000 && visual.data->green_mask == 0xff00
					&& visual.data->blue_mask == 0xff) {
					return QImage::Format_RGB32;
				}
				if (visual.data->red_mask == 0xff && visual.data->green_mask == 0xff00
					&& visual.data->blue_mask == 0xff0000) {
					return QImage::Format_RGBX8888;
				}
				return QImage::Format_Invalid;
			}
		}
	}

	return QImage::Format_Invalid;
}

} // namespace

std::unique_ptr< FrameGrabber > createXcbFrameGrabber(const CaptureSource &source) {
	// Under Wayland, X11 only knows about the windows of X11 applications
	if (QGuiApplication::platformName() != QLatin1String("xcb"))
		return nullptr;

	// A connection of its own, as the grabber runs on a different thread than Qt's connection
	int screenNumber                = 0;
	xcb_connection_t *rawConnection = xcb_connect(nullptr, &screenNumber);
	auto connection                 = std::make_shared< XcbConnection >(rawConnection);
	if (xcb_connection_has_error(rawConnection))
		return nullptr;

	const xcb_setup_t *setup   = xcb_get_setup(rawConnection);
	xcb_screen_iterator_t root = xcb_setup_roots_iterator(setup);
	for (int i = 0; i < screenNumber && root.rem; ++i)
		xcb_screen_next(&root);
	if (!root.rem)
		return nullptr;

	QRect area;
	xcb_window_t window     = XCB_WINDOW_NONE;
	xcb_visualid_t visualId = root.data->root_visual;
	std::uint8_t depth      = root.data->root_depth;

	if (source.type == CaptureSource::Type::EntireScreen) {
		const QList< QScreen * > screens = QGuiApplication::screens();
		if (source.screenIndex < 0 || source.screenIndex >= screens.size())
			return nullptr;

		// Qt only scales the size of the screen geometry to device independent pixels, not its position
		const QScreen *screen = screens.at(source.screenIndex);
		const QRect geometry  = screen->geometry();
		area                  = QRect(geometry.topLeft(), geometry.size() * screen->devicePixelRatio())
				   .intersected(QRect(0, 0, root.data->width_in_pixels, root.data->height_in_pixels));
		if (area.isEmpty())
			return nullptr;
	} else {
		window = static_cast< xcb_window_t >(source.nativeWindowId);

		xcb_get_window_attributes_reply_t *attributes =
			xcb_get_window_attributes_reply(rawConnection, xcb_get_window_attributes(rawConnection, window), nullptr);
		if (!attributes)
			return nullptr;
		visualId = attributes->visual;
		std::free(attributes);

		xcb_get_geometry_reply_t *geometry =
			xcb_get_geometry_reply(rawConnection, xcb_get_geometry(rawConnection, window), nullptr);
		if (!geometry)
			return nullptr;
		depth = geometry->depth;
		std::free(geometry);
	}

	const QImage::Format format = formatOf(setup, visualId, depth);
	if (format == QImage::Format_Invalid)
		return nullptr;

	const bool useShm = connection->supportsShm();
	return std::make_unique< XcbFrameGrabber >(std::move(connection), root.data->root, window, area, format, useShm);
}
