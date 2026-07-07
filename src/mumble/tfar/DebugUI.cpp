#include "DebugUI.h"
#include "Teamspeak.hpp"

// Debug values referenced by clientData.cpp (extern float ...). In the
// original TFAR they lived in the GLFW DebugUI which is not ported.
float debugDisplayThing  = 0.f;
float debugDisplayThing2 = 0.f;

// MUMBLE-TFAR: the original TFAR debug window (GLFW/OpenGL overlay) is not ported.
// Point the user to the chat based diagnostics instead.
void DebugUI::run() {
    Teamspeak::printMessageToCurrentTab(
        "TFAR: The graphical debug window is not available in mumble-tfar. "
        "Use \"/tfar diag\" or \"/tfar pos\" in chat instead.");
}
