/* Copyright (C) 2012, Stefan Hacker <dd0t@users.sourceforge.net>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
	 this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
	 this list of conditions and the following disclaimer in the documentation
	 and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
	 contributors may be used to endorse or promote products derived from this
	 software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include <initguid.h>
#include <Global.h>
#include <QMutexLocker>
#include "MainWindow.h"
#include <boost/thread/once.hpp>

#include "WASAPINotificationClient.h"

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDevice) {
	const QString device = QString::fromWCharArray(pwstrDefaultDevice);

	qWarning()	<< "WASAPINotificationClient: Default device changed flow=" << flow
				<< "role=" << role
				<< "device" << device;

	QMutexLocker lock(&listsMutex);
	if (!usedDefaultDevices.empty() && role == eCommunications) {
		restartAudio();
	}
	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key) {
	const QString device = QString::fromWCharArray(pwstrDeviceId);

	const bool formatChanged = (key == PKEY_AudioEngine_DeviceFormat);
	const bool channelConfigChanged = (key == PKEY_AudioEndpoint_PhysicalSpeakers);

	QMutexLocker lock(&listsMutex);
	if ((formatChanged || channelConfigChanged) && usedDevices.contains(device)) {
		qWarning()	<<"WASAPINotificationClient: Property changed device=" << device
					<< "formatChanged=" << formatChanged
					<< "channelConfigChanged=" << channelConfigChanged;

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

	qWarning()	<< "WASAPINotificationClient: Device state changed newState=" << dwNewState
				<< "device=" << device;

	return S_OK;
}

HRESULT STDMETHODCALLTYPE WASAPINotificationClient::QueryInterface(REFIID riid, VOID **ppvInterface) {
	if (IID_IUnknown == riid)
	{
		*ppvInterface = (IUnknown*)this;
		AddRef();
	}
	else if (__uuidof(IMMNotificationClient) == riid)
	{
		*ppvInterface = (IMMNotificationClient*)this;
		AddRef();
	}
	else
	{
		*ppvInterface = NULL;
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

void WASAPINotificationClient::_enlistDeviceAsUsed(const QString& device)
{
	if (!usedDevices.contains(device)) {
		usedDevices.append(device);
	}
}

void WASAPINotificationClient::enlistDeviceAsUsed(const QString& device) {
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

void WASAPINotificationClient::_clearUsedDeviceLists()
{
	usedDefaultDevices.clear();
	usedDevices.clear();
}

void WASAPINotificationClient::clearUsedDeviceLists() {
	QMutexLocker lock(&listsMutex);
	_clearUsedDeviceLists();
}

void WASAPINotificationClient::doGetOnce() {
	(void)WASAPINotificationClient::doGet();
}

WASAPINotificationClient& WASAPINotificationClient::doGet() {
	static WASAPINotificationClient instance;
	return instance;
}

static boost::once_flag notification_client_init_once = BOOST_ONCE_INIT;

WASAPINotificationClient& WASAPINotificationClient::get() {
	// Hacky way of making sure we get a thread-safe yet lazy initialization of the static.
	boost::call_once(&WASAPINotificationClient::doGetOnce, notification_client_init_once);
	return doGet();
}

WASAPINotificationClient::WASAPINotificationClient() : QObject(), pEnumerator(0), listsMutex() {
	AddRef(); // Static singleton, always has a self-reference

	HRESULT hr = CoCreateInstance(__uuidof(MMDeviceEnumerator), NULL, CLSCTX_ALL, __uuidof(IMMDeviceEnumerator), reinterpret_cast<void **>(&pEnumerator));
	if (!pEnumerator || FAILED(hr)) {
		if (pEnumerator) {
			pEnumerator->Release();
			pEnumerator = 0;
		}
		qWarning() << "WASAPINotificationClient: Failed to create enumerator, will not receive notifications";
		return;
	}

	g.mw->connect(this, SIGNAL(doResetAudio()), SLOT(onResetAudio()), Qt::QueuedConnection);

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
