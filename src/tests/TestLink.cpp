#include <QtCore>
#include <windows.h>

struct LinkedMem {
	DWORD	dwVersion;
	DWORD	dwTick;
	float	fPosition[3];
	float	fFront[3];
	float	fTop[3];
	wchar_t	name[256];
};

static HANDLE hMapObject = NULL;
LinkedMem *lm = NULL;

void initMumble() {
	hMapObject = OpenFileMappingW(FILE_MAP_ALL_ACCESS, FALSE, L"MumbleLink");
	if (hMapObject == NULL)
		return;

	lm = (LinkedMem *) MapViewOfFile(hMapObject, FILE_MAP_ALL_ACCESS, 0, 0, sizeof(LinkedMem));
	if (lm == NULL) {
		CloseHandle(hMapObject);
		hMapObject = NULL;
		return;
	}
	wcscpy_s(lm->name, 256, L"TestLink");
}

void updateMumble() {
	if (! lm)
		return;
	lm->dwVersion = 1;
	lm->dwTick = GetTickCount();

	// Left handed coordinate system.
	// X positive towards "left".
	// Y positive towards "up".
	// Z positive towards "into screen".

	// Front looks into scene.
	lm->fFront[0] = 0.0f;
	lm->fFront[1] = 0.0f;
	lm->fFront[2] = 1.0f;

	// Top looks straight up.
	lm->fTop[0] = 0.0f;
	lm->fTop[1] = 1.0f;
	lm->fTop[2] = 0.0f;

	lm->fPosition[0] += 0.001f;
}

int main(int argc, char **argv) {
	QCoreApplication a(argc, argv);

	initMumble();
	if (lm == NULL)
		qFatal("No Link!");

	while (true) {
		qWarning("Tick!");
		updateMumble();
		Sleep(100);
	}
}
