// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MainWindow.h"

#include "WASAPINotificationClient.h"
#include "Global.h"

#include <QtCore/QMutexLocker>
#include <boost/thread/once.hpp>

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role,
																		   LPCWSTR pwstrDefaultDevice) {
	const QString device = QString::fromWCharArray(pwstrDefaultDevice);

	qWarning() << "WASAPINotificationClient: Default device changed flow=" << flow << "role=" << role << "device"
			   << device;

	QMutexLocker lock(&listsMutex);
	if (!usedDefaultDevices.empty() && role == eCommunications) {
		restartAudio();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId,
																		   const PROPERTYKEY key) {
	const QString device = QString::fromWCharArray(pwstrDeviceId);

	const bool formatChanged        = (key == PKEY_AudioEngine_DeviceFormat);
	const bool channelConfigChanged = (key == PKEY_AudioEndpoint_PhysicalSpeakers);

	QMutexLocker lock(&listsMutex);
	if ((formatChanged || channelConfigChanged) && usedDevices.contains(device)) {
		qWarning() << "WASAPINotificationClient: Property changed device=" << device
				   << "formatChanged=" << formatChanged << "channelConfigChanged=" << channelConfigChanged;

		restartAudio();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnDeviceAdded(LPCWSTR pwstrDeviceId) {
	const QString device = QString::fromWCharArray(pwstrDeviceId);
	qWarning() << "WASAPINotificationClient: Device added=" << device;
	return S_OK;
}
HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnDeviceRemoved(LPCWSTR pwstrDeviceId) {
	const QString device = QString::fromWCharArray(pwstrDeviceId);
	qWarning() << "WASAPINotificationClient: Device removed=" << device;
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState) {
	const QString device = QString::fromWCharArray(pwstrDeviceId);

	qWarning() << "WASAPINotificationClient: Device state changed newState=" << dwNewState << "device=" << device;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::QueryInterface(REFIID riid, VOID **ppvInterface) {
	if (IID_IUnknown == riid) {
		*ppvInterface = (IUnknown *) this;
		AddRef();
	} else if (__uuidof(IMMNotificationClient) == riid) {
		*ppvInterface = (IMMNotificationClient *) this;
		AddRef();
	} else {
		*ppvInterface = nullptr;
		return E_NOINTERFACE;
	}
	return S_OK;
}

ULONG STDMETHODCALLTYPE WASAPINotificationClient::AddRef() {
	return InterlockedIncrement(&_cRef);
}

ULONG STDMETHODCALLTYPE WASAPINotificationClient::Release() {
	// We hold a ref to ourselves all the time (static singleton) so no
	// need to clean ourselves up or anything.
	ULONG ulRef = InterlockedDecrement(&_cRef);
	Q_ASSERT(ulRef > 0);
	return ulRef;
}

void WASAPINotificationClient::enlistDefaultDeviceAsUsed(LPCWSTR pwstrDefaultDevice) {
	const QString device = QString::fromWCharArray(pwstrDefaultDevice);
	QMutexLocker lock(&listsMutex);
	if (!usedDefaultDevices.contains(device)) {
		usedDefaultDevices.append(device);
		_enlistDeviceAsUsed(device);
	}
}

void WASAPINotificationClient::enlistDeviceAsUsed(LPCWSTR pwstrDevice) {
	const QString device = QString::fromWCharArray(pwstrDevice);
	QMutexLocker lock(&listsMutex);
	_enlistDeviceAsUsed(device);
}

void WASAPINotificationClient::_enlistDeviceAsUsed(const QString &device) {
	if (!usedDevices.contains(device)) {
		usedDevices.append(device);
	}
}

void WASAPINotificationClient::enlistDeviceAsUsed(const QString &device) {
	QMutexLocker lock(&listsMutex);
	_enlistDeviceAsUsed(device);
}

void WASAPINotificationClient::unlistDevice(LPCWSTR pwstrDevice) {
	const QString device = QString::fromWCharArray(pwstrDevice);
	QMutexLocker lock(&listsMutex);
	usedDevices.removeOne(device);
	usedDefaultDevices.removeOne(device);
}

void WASAPINotificationClient::clearUsedDefaultDeviceList() {
	QMutexLocker lock(&listsMutex);
	usedDefaultDevices.clear();
}

void WASAPINotificationClient::_clearUsedDeviceLists() {
	usedDefaultDevices.clear();
	usedDevices.clear();
}

void WASAPINotificationClient::clearUsedDeviceLists() {
	QMutexLocker lock(&listsMutex);
	_clearUsedDeviceLists();
}

void WASAPINotificationClient::doGetOnce() {
	(void) WASAPINotificationClient::doGet();
}

WASAPINotificationClient &WASAPINotificationClient::doGet() {
	static WASAPINotificationClient instance;
	return instance;
}

static boost::once_flag notification_client_init_once = BOOST_ONCE_INIT;

WASAPINotificationClient &WASAPINotificationClient::get() {
	// Hacky way of making sure we get a thread-safe yet lazy initialization of the static.
	boost::call_once(&WASAPINotificationClient::doGetOnce, notification_client_init_once);
	return doGet();
}

WASAPINotificationClient::WASAPINotificationClient() : QObject(), pEnumerator(0), listsMutex() {
	AddRef(); // Static singleton, always has a self-reference

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), nullptr, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator),
								  reinterpret_cast< void ** >(&pEnumerator));
	if (!pEnumerator || FAILED(hr)) {
		if (pEnumerator) {
			pEnumerator->Release();
			pEnumerator = 0;
		}
		qWarning() << "WASAPINotificationClient: Failed to create enumerator, will not receive notifications";
		return;
	}

	Global::get().mw->connect(this, SIGNAL(doResetAudio()), SLOT(onResetAudio()), Qt::QueuedConnection);

	pEnumerator->RegisterEndpointNotificationCallback(this);
}

WASAPINotificationClient::~WASAPINotificationClient() {
	if (pEnumerator)
		pEnumerator->Release();
}

void WASAPINotificationClient::restartAudio() {
	qWarning("WASAPINotificationClient: Triggering audio reset");
	_clearUsedDeviceLists();
	emit doResetAudio();
}
