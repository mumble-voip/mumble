#include "DebugUI.h"
#include "Teamspeak.hpp"

// MUMBLE-TFAR: the original TFAR debug window (GLFW/OpenGL overlay) is not ported.
// Point the user to the chat based diagnostics instead.
void DebugUI::run() {
    Teamspeak::printMessageToCurrentTab(
        "TFAR: The graphical debug window is not available in mumble-tfar. "
        "Use \"/tfar diag\" or \"/tfar pos\" in chat instead.");
}
