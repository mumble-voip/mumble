// Allow use of wcsncpy without warnings
#define _CRT_SECURE_NO_WARNINGS

#include <QtCore>

#ifdef WIN32
#include <windows.h>
#define uint32_t UINT32
#else
#endif

struct LinkedMem {
	uint32_t uiVersion;
	uint32_t uiTick;
	float	fAvatarPosition[3];
	float	fAvatarFront[3];
	float	fAvatarTop[3];
	wchar_t	name[256];
	float	fCameraPosition[3];
	float	fCameraFront[3];
	float	fCameraTop[3];
	wchar_t	identity[256];
	uint32_t context_len;
	unsigned char context[256];
	wchar_t description[2048];
};

LinkedMem *lm = NULL;


void initMumble() {

#ifdef WIN32
	HANDLE hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMapObject == NULL)
		return;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
	if (lm == NULL) {
		CloseHandle(hMapObject);
		hMapObject = NULL;
		return;
	}
#else
	char memname[256];
	snprintf(memname, 256, "/MumbleLink.%d", getuid());

	int shmfd = shm_open(memname, O_RDWR, S_IRUSR | S_IWUSR);

	if (shmfd < 0) {
		return;
	}

	lm = (LinkedMem *)(mmap(NULL, sizeof(struct LinkedMem), PROT_READ | PROT_WRITE, MAP_SHARED, shmfd,0));

	if (lm == (void *)(-1)) {
		lm = NULL;
		return;
	}
#endif

	wcsncpy(lm->name, L"TestLink", 256);
	wcsncpy(lm->description, L"TestLink is a test of the Link plugin.", 2048);
}

void updateMumble() {
	if (! lm)
		return;
	lm->uiVersion = 2;
	lm->uiTick++;

	// Left handed coordinate system.
	// X positive towards "left".
	// Y positive towards "up".
	// Z positive towards "into screen".

	// Front looks into scene.
	lm->fAvatarFront[0] = 0.0f;
	lm->fAvatarFront[1] = 0.0f;
	lm->fAvatarFront[2] = 1.0f;

	// Top looks straight up.
	lm->fAvatarTop[0] = 0.0f;
	lm->fAvatarTop[1] = 1.0f;
	lm->fAvatarTop[2] = 0.0f;

	// Standing slightly off the origo
	lm->fAvatarPosition[0] += 0.001f;
	lm->fAvatarPosition[1] = 0.0f;
	lm->fAvatarPosition[2] = 0.5f;

	// Camera fixed.
	lm->fCameraPosition[0] = 0.0f;
	lm->fCameraPosition[1] = 0.0f;
	lm->fCameraPosition[2] = 0.0f;

	lm->fCameraFront[0] = 0.0f;
	lm->fCameraFront[1] = 0.0f;
	lm->fCameraFront[2] = 1.0f;

	lm->fCameraTop[0] = 0.0f;
	lm->fCameraTop[1] = 1.0f;
	lm->fCameraTop[2] = 0.0f;

	wcsncpy(lm->identity, L"Unique ID", 256);
	memcpy(lm->context, "ContextBlob\x00\x01\x02\x03\x04", 16);
	lm->context_len = 16;
}

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	initMumble();
	if (lm == NULL)
		qFatal("No Link!");

	lm->fAvatarPosition[0];

	while (true) {
		qWarning("Tick!");
		updateMumble();
		Sleep(100);
	}
}
