// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "MumbleApplication.h"

#ifdef _MSC_VER
#	include "Utils.h"
#endif

// Must be included before other Windows stuff
#include "win.h"

#include "versionhelpers.h"

#include "Version.h"

#include "Global.h"

#include <cfloat>
#include <cmath>

#include <dbghelp.h>
#include <tlhelp32.h>
#include <wincrypt.h>

#ifdef _MSC_VER
#	include <delayimp.h>
#endif

#include <emmintrin.h>
#include <share.h> // For share flags for _wfsopen
#include <shlobj.h>
#include <shobjidl.h>

#include <limits>

extern "C" {
void __cpuid(int a[4], int b);
void mumble_speex_init();
};

#define DUMP_BUFFER_SIZE 1024

static wchar_t wcCrashDumpPath[DUMP_BUFFER_SIZE];

static wchar_t wcComment[DUMP_BUFFER_SIZE] = L"";
static MINIDUMP_USER_STREAM musComment;

static int cpuinfo[4];

bool bIsWin7     = false;
bool bIsVistaSP1 = false;

HWND mumble_mw_hwnd = 0;

static LONG WINAPI MumbleUnhandledExceptionFilter(struct _EXCEPTION_POINTERS *ExceptionInfo) {
	MINIDUMP_EXCEPTION_INFORMATION i;
	i.ThreadId          = GetCurrentThreadId();
	i.ExceptionPointers = ExceptionInfo;

	MINIDUMP_USER_STREAM_INFORMATION musi;

	musi.UserStreamCount = 1;
	musi.UserStreamArray = &musComment;

	HANDLE hMinidump =
		CreateFile(wcCrashDumpPath, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
	if (hMinidump != INVALID_HANDLE_VALUE) {
		if (MiniDumpWriteDump(
				GetCurrentProcess(), GetCurrentProcessId(), hMinidump,
				static_cast< MINIDUMP_TYPE >(MiniDumpWithIndirectlyReferencedMemory | MiniDumpWithThreadInfo), &i,
				&musi, nullptr)) {
			FlushFileBuffers(hMinidump);
		}
		CloseHandle(hMinidump);
	}

	return EXCEPTION_CONTINUE_SEARCH;
}

static void enableCrashOnCrashes() {
	// Makes sure the application actually crashes when one of its callbacks
	// called from the kernel crashes.
	//
	// See http://support.microsoft.com/kb/976038
	//     http://www.altdevblogaday.com/2012/07/06/when-even-crashing-doesnt-work/

	typedef BOOL(WINAPI * tGetPolicy)(LPDWORD lpFlags);
	typedef BOOL(WINAPI * tSetPolicy)(DWORD dwFlags);

	const DWORD PROCESS_CALLBACK_FILTER_ENABLED = 0x01;

	HMODULE kernel32 = LoadLibrary(L"kernel32.dll");

	tGetPolicy pGetPolicy = (tGetPolicy) GetProcAddress(kernel32, "GetProcessUserModeExceptionPolicy");

	tSetPolicy pSetPolicy = (tSetPolicy) GetProcAddress(kernel32, "SetProcessUserModeExceptionPolicy");

	if (pGetPolicy && pSetPolicy) { // Only available as of Vista SP2 / Win7 SP1
		DWORD dwFlags;
		if (pGetPolicy(&dwFlags)) {
			if (!pSetPolicy(dwFlags & ~PROCESS_CALLBACK_FILTER_ENABLED))
				qWarning("enableCrashOnCrashes: Failed to set policy");
		} else {
			qWarning("enableCrashOnCrashes: Failed to get policy");
		}
	}
}

BOOL SetHeapOptions() {
	HMODULE hLib = LoadLibrary(L"kernel32.dll");
	if (!hLib)
		return FALSE;

	typedef BOOL(WINAPI * HSI)(HANDLE, HEAP_INFORMATION_CLASS, PVOID, SIZE_T);
	HSI pHsi = (HSI) GetProcAddress(hLib, "HeapSetInformation");
	if (!pHsi) {
		FreeLibrary(hLib);
		return FALSE;
	}

#ifndef HeapEnableTerminationOnCorruption
#	define HeapEnableTerminationOnCorruption (HEAP_INFORMATION_CLASS) 1
#endif

	BOOL fRet = (pHsi)(nullptr, HeapEnableTerminationOnCorruption, nullptr, 0) ? TRUE : FALSE;
	if (hLib)
		FreeLibrary(hLib);

	return fRet;
}

// We only support delay-loading on MSVC, not on MinGW.
#ifdef _MSC_VER
FARPROC WINAPI delayHook(unsigned dliNotify, PDelayLoadInfo pdli) {
	if (dliNotify != dliNotePreLoadLibrary)
		return 0;


	size_t length = strlen(pdli->szDll);
	if (length < 5)
		return 0;

	size_t buflen = length + 10;

	std::vector< char > filename;
	filename.resize(buflen);
	strcpy_s(filename.data(), buflen, pdli->szDll);

	size_t offset = 0;

	if (_stricmp(filename.data() + length - 4, ".dll") == 0)
		offset = length - 4;
	else
		offset = length;

	HMODULE hmod = 0;

	// SSE?
	if (cpuinfo[3] & 0x02000000) {
		// SSE2?
		if (cpuinfo[3] & 0x04000000) {
			// And SSE3?
			if (cpuinfo[2] & 0x00000001) {
				strcpy_s(filename.data() + offset, 10, ".sse3.dll");
				hmod = LoadLibraryA(filename.data());
				if (hmod)
					return (FARPROC) hmod;
			}

			strcpy_s(filename.data() + offset, 10, ".sse2.dll");
			hmod = LoadLibraryA(filename.data());
			if (hmod)
				return (FARPROC) hmod;
		}

		strcpy_s(filename.data() + offset, 10, ".sse.dll");
		hmod = LoadLibraryA(filename.data());
		if (hmod)
			return (FARPROC) hmod;
	}

	return 0;
}

decltype(__pfnDliNotifyHook2) __pfnDliNotifyHook2 = delayHook;
#endif

void os_init() {
	__cpuid(cpuinfo, 1);

#define MMXSSE 0x02800000
	if ((cpuinfo[3] & MMXSSE) != MMXSSE) {
		::MessageBoxA(nullptr, "Mumble requires a SSE capable processor (Pentium 3 / Ahtlon-XP)", "Mumble",
					  MB_OK | MB_ICONERROR);
		exit(0);
	}

	bIsWin7     = IsWindows7OrGreater();
	bIsVistaSP1 = IsWindowsVistaSP1OrGreater();

#if _MSC_VER == 1800 && defined(_M_X64)
	// Disable MSVC 2013's FMA-optimized math routines on Windows
	// versions earlier than Windows 8 (6.2).
	// There are various issues on OSes that do not support the newer
	// instructions.
	// See issue mumble-voip/mumble#1615.
	if (!IsWindows8OrGreater()) {
		_set_FMA3_enable(0);
	}
#endif

	unsigned int currentControl = 0;
	_controlfp_s(&currentControl, _DN_FLUSH, _MCW_DN);

	SetHeapOptions();
	enableCrashOnCrashes();
	mumble_speex_init();

#ifdef QT_NO_DEBUG
	QString hash;
	QFile f(qApp->applicationFilePath());
	if (!f.open(QIODevice::ReadOnly)) {
		qWarning("VersionCheck: Failed to open binary");
	} else {
		QByteArray a = f.readAll();
		if (a.size() > 0) {
			QCryptographicHash qch(QCryptographicHash::Sha1);
			qch.addData(a);
			hash = QLatin1String(qch.result().toHex());
		}
	}

	QString comment = QString::fromLatin1("%1\n%2").arg(Version::getRelease(), hash);

	wcscpy_s(wcComment, DUMP_BUFFER_SIZE, comment.toStdWString().c_str());
	musComment.Type       = CommentStreamW;
	musComment.Buffer     = wcComment;
	musComment.BufferSize = static_cast< ULONG >(wcslen(wcComment) * sizeof(wchar_t));

	QString dump = Global::get().qdBasePath.filePath(QLatin1String("mumble.dmp"));

	QFileInfo fi(dump);
	QDir::root().mkpath(fi.absolutePath());

	if (wcscpy_s(wcCrashDumpPath, DUMP_BUFFER_SIZE, dump.toStdWString().c_str()) == 0)
		SetUnhandledExceptionFilter(MumbleUnhandledExceptionFilter);

#endif

	Global::get().qdBasePath.mkpath(QLatin1String("Snapshots"));
	if (bIsWin7)
		SetCurrentProcessExplicitAppUserModelID(L"net.sourceforge.mumble.Mumble");
}

DWORD WinVerifySslCert(const QByteArray &cert) {
	DWORD errorStatus = std::numeric_limits< DWORD >::max();

	PCCERT_CONTEXT certContext = CertCreateCertificateContext(
		X509_ASN_ENCODING, reinterpret_cast< const BYTE * >(cert.constData()), cert.size());
	if (!certContext) {
		return errorStatus;
	}

	LPCSTR usage[] = { szOID_PKIX_KP_SERVER_AUTH, szOID_SERVER_GATED_CRYPTO, szOID_SGC_NETSCAPE };

	CERT_CHAIN_PARA chainParameter;
	memset(&chainParameter, 0, sizeof(CERT_CHAIN_PARA));
	chainParameter.cbSize                                    = sizeof(CERT_CHAIN_PARA);
	chainParameter.RequestedUsage.dwType                     = USAGE_MATCH_TYPE_OR;
	chainParameter.RequestedUsage.Usage.cUsageIdentifier     = ARRAYSIZE(usage);
	chainParameter.RequestedUsage.Usage.rgpszUsageIdentifier = const_cast< LPSTR * >(usage);

	PCCERT_CHAIN_CONTEXT chainContext = nullptr;
	CertGetCertificateChain(nullptr, certContext, nullptr, nullptr, &chainParameter, 0, nullptr, &chainContext);

	if (chainContext) {
		errorStatus = chainContext->TrustStatus.dwErrorStatus;
		CertFreeCertificateChain(chainContext);
	}

	CertFreeCertificateContext(certContext);

	return errorStatus;
}

#undef DUMP_BUFFER_SIZE
#undef HeapEnableTerminationOnCorruption
#undef MMXSSE
