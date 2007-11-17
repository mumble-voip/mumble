
#include "GlobalShortcut_macx.h"

GlobalShortcutEngine *GlobalShortcutEngine::platformInit() {
	return new GlobalShortcutMac();
}

// ---

GlobalShortcutMac::GlobalShortcutMac() {
}

GlobalShortcutMac::~GlobalShortcutMac() {
}

void GlobalShortcutMac::run() {
}

QString GlobalShortcutMac::buttonName(const QVariant &v) {
  return QString();
}
