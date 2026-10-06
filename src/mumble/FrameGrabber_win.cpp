// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "FrameGrabber.h"

#include <QtGui/QGuiApplication>
#include <QtGui/QScreen>
#include <QtGui/QTransform>
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
#	include <QtGui/qscreen_platform.h>
#endif

#include <algorithm>
#include <cstring>
#include <memory>
#include <mutex>
#include <vector>

#include <windows.h>

#include <d3d11.h>
#include <dxgi1_2.h>
#include <wrl/client.h>

using Microsoft::WRL::ComPtr;

namespace {

/// Recycles the memory of grabbed images. Allocating (and the system zeroing) the memory of a large picture for
/// every frame takes a noticeable amount of time.
///
/// The images use pooled buffers as their memory, which come back to the pool once the last copy of an image is
/// gone. That may happen on any thread (usually the encoder's) and only after the grabber is gone, which is why the
/// pool is shared by the grabber and all buffers in use. As nothing else refers to a buffer that is handed out,
/// writing to the image doesn't copy it.
class ImagePool {
public:
	/// Returns an image of the given size and format that nobody else refers to. The format has to use 32 bits per
	/// pixel.
	QImage get(int width, int height, QImage::Format format) {
		const int bytesPerLine = width * 4;
		Buffer *buffer =
			m_shared->acquire(static_cast< std::size_t >(bytesPerLine) * static_cast< std::size_t >(height), m_shared);
		return QImage(buffer->data.get(), width, height, bytesPerLine, format, &Shared::release, buffer);
	}

private:
	/// See XcbConnection::MAX_SEGMENTS
	static constexpr std::size_t MAX_IMAGES = 6;

	struct Shared;

	struct Buffer {
		std::unique_ptr< uchar[] > data;
		std::size_t size = 0;
		/// Keeps the pool alive while the buffer is in use
		std::shared_ptr< Shared > owner;
	};

	struct Shared {
		Buffer *acquire(std::size_t size, const std::shared_ptr< Shared > &self) {
			Buffer *buffer = nullptr;
			{
				std::lock_guard< std::mutex > lock(mutex);
				if (size != this->size) {
					// The picture size changed, buffers of the old size are of no use anymore
					free.clear();
					this->size = size;
				}
				if (!free.empty()) {
					buffer = free.back().release();
					free.pop_back();
				}
			}

			if (!buffer) {
				buffer       = new Buffer();
				buffer->data = std::make_unique_for_overwrite< uchar[] >(size);
				buffer->size = size;
			}
			buffer->owner = self;
			return buffer;
		}

		/// Hands a buffer back. May be called from any thread.
		static void release(void *info) {
			std::unique_ptr< Buffer > buffer(static_cast< Buffer * >(info));
			// Keeps the pool alive until it is done with the buffer, even if this was the last reference
			const std::shared_ptr< Shared > owner = std::move(buffer->owner);

			std::lock_guard< std::mutex > lock(owner->mutex);
			if (buffer->size == owner->size && owner->free.size() < MAX_IMAGES)
				owner->free.push_back(std::move(buffer));
		}

		std::mutex mutex;
		std::vector< std::unique_ptr< Buffer > > free;
		/// Size of the pictures currently grabbed
		std::size_t size = 0;
	};

	std::shared_ptr< Shared > m_shared = std::make_shared< Shared >();
};

/// Grabs a screen with the DXGI desktop duplication API. Windows only hands out pictures when the screen content
/// changed, which also saves copying and encoding pictures of a static screen.
class DxgiScreenGrabber : public FrameGrabber {
public:
	static std::unique_ptr< DxgiScreenGrabber > create(HMONITOR monitor) {
		std::unique_ptr< DxgiScreenGrabber > grabber(new DxgiScreenGrabber(monitor));
		if (!grabber->duplicate())
			return nullptr;
		return grabber;
	}

	Result grab(QImage &image) override {
		if (!m_duplication && !duplicate()) {
			// The desktop can't be duplicated while e.g. the UAC prompt or the lock screen is shown, so try again
			// later. Anything else means that the screen is gone.
			return m_accessDenied ? Result::Unchanged : Result::Failed;
		}

		DXGI_OUTDUPL_FRAME_INFO info = {};
		ComPtr< IDXGIResource > resource;
		// Right after duplicating the output, the first call hands out the current picture. Waiting for it a moment
		// makes sure that viewers get to see something even if the screen doesn't change.
		HRESULT hr = m_duplication->AcquireNextFrame(m_hasPicture ? 0 : 100, &info, &resource);
		if (hr == DXGI_ERROR_WAIT_TIMEOUT)
			return Result::Unchanged;
		if (hr == DXGI_ERROR_ACCESS_LOST) {
			// E.g. the display mode changed or a full screen application took over. Duplicate the output anew.
			m_duplication.Reset();
			return Result::Unchanged;
		}
		if (FAILED(hr))
			return Result::Failed;

		// Updates that only concern the mouse pointer don't change the picture
		if (info.LastPresentTime.QuadPart == 0 && m_hasPicture) {
			m_duplication->ReleaseFrame();
			return Result::Unchanged;
		}

		ComPtr< ID3D11Texture2D > texture;
		hr = resource.As(&texture);
		if (FAILED(hr) || !ensureStagingTexture(texture.Get())) {
			m_duplication->ReleaseFrame();
			return Result::Failed;
		}

		m_context->CopyResource(m_staging.Get(), texture.Get());
		m_duplication->ReleaseFrame();

		D3D11_MAPPED_SUBRESOURCE mapped = {};
		if (FAILED(m_context->Map(m_staging.Get(), 0, D3D11_MAP_READ, 0, &mapped)))
			return Result::Failed;

		// The duplicated surface is B8G8R8A8, i.e. Format_RGB32 in memory. Its rows are padded differently.
		image = m_pool.get(static_cast< int >(m_stagingDesc.Width), static_cast< int >(m_stagingDesc.Height),
						   QImage::Format_RGB32);
		const std::size_t rowBytes = static_cast< std::size_t >(image.width()) * 4;
		const uchar *src           = static_cast< const uchar * >(mapped.pData);
		for (int y = 0; y < image.height(); ++y)
			std::memcpy(image.scanLine(y), src + static_cast< std::size_t >(y) * mapped.RowPitch, rowBytes);

		m_context->Unmap(m_staging.Get(), 0);

		// The picture comes in the orientation the display is built in, not the one it is used in
		switch (m_rotation) {
			case DXGI_MODE_ROTATION_ROTATE90:
				image = image.transformed(QTransform().rotate(90));
				break;
			case DXGI_MODE_ROTATION_ROTATE180:
				image = image.transformed(QTransform().rotate(180));
				break;
			case DXGI_MODE_ROTATION_ROTATE270:
				image = image.transformed(QTransform().rotate(270));
				break;
			default:
				break;
		}

		m_hasPicture = true;
		return Result::Frame;
	}

private:
	explicit DxgiScreenGrabber(HMONITOR monitor) : m_monitor(monitor) {}

	/// Sets up the duplication of the output that shows m_monitor.
	bool duplicate() {
		m_accessDenied = false;
		m_hasPicture   = false;

		if (!m_device) {
			ComPtr< IDXGIFactory1 > factory;
			if (FAILED(
					CreateDXGIFactory1(__uuidof(IDXGIFactory1), reinterpret_cast< void ** >(factory.GetAddressOf()))))
				return false;

			// The output has to be duplicated with a device on the adapter it is connected to
			ComPtr< IDXGIAdapter1 > adapter;
			for (UINT a = 0; !m_output && factory->EnumAdapters1(a, adapter.ReleaseAndGetAddressOf()) == S_OK; ++a) {
				ComPtr< IDXGIOutput > output;
				for (UINT o = 0; adapter->EnumOutputs(o, output.ReleaseAndGetAddressOf()) == S_OK; ++o) {
					DXGI_OUTPUT_DESC desc = {};
					if (SUCCEEDED(output->GetDesc(&desc)) && desc.Monitor == m_monitor) {
						if (FAILED(output.As(&m_output)))
							return false;
						break;
					}
				}
				if (m_output) {
					if (FAILED(D3D11CreateDevice(adapter.Get(), D3D_DRIVER_TYPE_UNKNOWN, nullptr, 0, nullptr, 0,
												 D3D11_SDK_VERSION, &m_device, nullptr, &m_context))) {
						return false;
					}
				}
			}

			if (!m_device)
				return false;
		}

		const HRESULT hr = m_output->DuplicateOutput(m_device.Get(), &m_duplication);
		if (FAILED(hr)) {
			m_accessDenied = hr == E_ACCESSDENIED;
			return false;
		}

		DXGI_OUTDUPL_DESC desc = {};
		m_duplication->GetDesc(&desc);
		m_rotation = desc.Rotation;
		return true;
	}

	bool ensureStagingTexture(ID3D11Texture2D *texture) {
		D3D11_TEXTURE2D_DESC desc = {};
		texture->GetDesc(&desc);
		if (desc.Format != DXGI_FORMAT_B8G8R8A8_UNORM)
			return false;

		if (m_staging && m_stagingDesc.Width == desc.Width && m_stagingDesc.Height == desc.Height)
			return true;

		desc.Usage          = D3D11_USAGE_STAGING;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_READ;
		desc.BindFlags      = 0;
		desc.MiscFlags      = 0;
		desc.MipLevels      = 1;
		desc.ArraySize      = 1;
		desc.SampleDesc     = { 1, 0 };
		m_staging.Reset();
		if (FAILED(m_device->CreateTexture2D(&desc, nullptr, &m_staging)))
			return false;
		m_stagingDesc = desc;
		return true;
	}

	HMONITOR m_monitor;
	ComPtr< IDXGIOutput1 > m_output;
	ComPtr< ID3D11Device > m_device;
	ComPtr< ID3D11DeviceContext > m_context;
	ComPtr< IDXGIOutputDuplication > m_duplication;
	DXGI_MODE_ROTATION m_rotation = DXGI_MODE_ROTATION_IDENTITY;
	/// Set when duplicating the output failed because the desktop is not accessible right now
	bool m_accessDenied = false;
	/// Whether a picture was grabbed since the output was duplicated
	bool m_hasPicture = false;

	ComPtr< ID3D11Texture2D > m_staging;
	D3D11_TEXTURE2D_DESC m_stagingDesc = {};
	ImagePool m_pool;
};

HMONITOR monitorOf(const QScreen *screen) {
#if QT_VERSION >= QT_VERSION_CHECK(6, 7, 0)
	if (auto *windowsScreen = screen->nativeInterface< QNativeInterface::QWindowsScreen >())
		return windowsScreen->handle();
#endif
	// Qt only scales the size of the screen geometry to device independent pixels, not its position
	const QPoint topLeft = screen->geometry().topLeft();
	return MonitorFromPoint(POINT{ topLeft.x(), topLeft.y() }, MONITOR_DEFAULTTONULL);
}

} // namespace

std::unique_ptr< FrameGrabber > createWindowsFrameGrabber(const CaptureSource &source) {
	if (source.type != CaptureSource::Type::EntireScreen)
		return nullptr;

	const QList< QScreen * > screens = QGuiApplication::screens();
	if (source.screenIndex < 0 || source.screenIndex >= screens.size())
		return nullptr;

	const HMONITOR monitor = monitorOf(screens.at(source.screenIndex));
	if (!monitor)
		return nullptr;

	return DxgiScreenGrabber::create(monitor);
}
