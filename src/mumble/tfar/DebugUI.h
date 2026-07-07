#pragma once
// MUMBLE-TFAR: stub replacing the original GLFW/OpenGL debug window.
// Diagnostic output is available through the Mumble chat instead ("/tfar diag").

class DebugUI {
public:
    DebugUI() = default;
    ~DebugUI() = default;

    void run();
    void stop() {}
};
