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

#ifndef WASAPINOTIFICATIONCLIENT_H_
#define WASAPINOTIFICATIONCLIENT_H_

#include <QtCore/QObject>
#include <QtCore/QMutex>
#include <mmdeviceapi.h>

/**
 * @brief Singleton for acting on WASAPINotification events for given devices.
 */
class WASAPINotificationClient : public QObject, public IMMNotificationClient {
	Q_OBJECT
public:
	/* IMMNotificationClient interface */
	HRESULT STDMETHODCALLTYPE OnDefaultDeviceChanged(EDataFlow flow, ERole role, LPCWSTR pwstrDefaultDevice);
	HRESULT STDMETHODCALLTYPE OnPropertyValueChanged(LPCWSTR pwstrDeviceId, const PROPERTYKEY key);
	HRESULT STDMETHODCALLTYPE OnDeviceAdded(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceRemoved(LPCWSTR pwstrDeviceId);
	HRESULT STDMETHODCALLTYPE OnDeviceStateChanged(LPCWSTR pwstrDeviceId, DWORD dwNewState);
	HRESULT STDMETHODCALLTYPE QueryInterface(REFIID riid, VOID **ppvInterface);
	ULONG STDMETHODCALLTYPE AddRef();
	ULONG STDMETHODCALLTYPE Release();

	/* Enlist/Unlist functionality */
	void enlistDefaultDeviceAsUsed(LPCWSTR pwstrDefaultDevice);

	void enlistDeviceAsUsed(LPCWSTR pwstrDevice);
	void enlistDeviceAsUsed(const QString& device);

	void unlistDevice(LPCWSTR pwstrDevice);

	void clearUsedDefaultDeviceList();
	void clearUsedDeviceLists();

	/**
	 * @return Singleton instance reference.
	 */
	static WASAPINotificationClient& get();

private:
	WASAPINotificationClient();
	~WASAPINotificationClient();

	WASAPINotificationClient(const WASAPINotificationClient&);
	WASAPINotificationClient& operator=(const WASAPINotificationClient&);

	static WASAPINotificationClient& doGet();
	static void doGetOnce();

	void restartAudio();

	/* _fu = Non locking versions */
	void _clearUsedDeviceLists();
	void _enlistDeviceAsUsed(const QString& device);

	QStringList usedDefaultDevices;
	QStringList usedDevices;
	IMMDeviceEnumerator *pEnumerator;
	LONG _cRef;
	QMutex listsMutex;

signals:
	void doResetAudio();
};

#endif // WASAPINOTIFICATIONCLIENT_H_
