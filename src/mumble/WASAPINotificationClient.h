// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WASAPINOTIFICATIONCLIENT_H_
#define MUMBLE_MUMBLE_WASAPINOTIFICATIONCLIENT_H_

#include <QtCore/QMutex>
#include <QtCore/QObject>
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
	void incrementWantDefault();
	void decrementWantDefault();

	void enlistDefaultDeviceAsUsed(const QString &device);
	void unlistDefaultDeviceAsUsed(const QString &device);

	void enlistDeviceAsUsed(const QString &device);
	void unlistDeviceAsUsed(const QString &device);

	void enlistDeviceAsWanted(const QString &device);
	void unlistDeviceAsWanted(const QString &device);

	void clearUsedDefaultDeviceList();
	void clearUsedDeviceList();
	void clearWantedDeviceList();

	/**
	 * @return Singleton instance reference.
	 */
	static WASAPINotificationClient &get();

private:
	WASAPINotificationClient();
	~WASAPINotificationClient() Q_DECL_OVERRIDE;

	WASAPINotificationClient(const WASAPINotificationClient &);
	WASAPINotificationClient &operator=(const WASAPINotificationClient &);

	static WASAPINotificationClient &doGet();
	static void doGetOnce();

	void restartAudio();

	/* _fu = Non locking versions */
	void _clearWantDefaultDeviceCount();
	void _clearUsedDefaultDeviceList();
	void _clearUsedDeviceList();
	void _clearWantedDeviceList();
	void _enlistDeviceAsUsed(const QString &device);
	void _enlistDeviceAsWanted(const QString &device);

	QStringList usedDefaultDevices;
	QStringList usedDevices;
	QStringList wantedDevices;
	IMMDeviceEnumerator *pEnumerator;
	LONG _cRef;
	QMutex listsMutex;
	std::atomic_int32_t wantDefaultCount = 0;
signals:
	void doResetAudio();
};

#endif // WASAPINOTIFICATIONCLIENT_H_
