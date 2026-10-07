# IPC runtime-directory regression tests

`TestIPCUtils` is registered with CTest when the `ipcutils` target is enabled on
POSIX platforms. Each case runs in a child process so the runtime-directory cache
starts uninitialized. The parent creates and removes private temporary fixtures.
The C ABI caller is a separate C translation unit.

The focused tests can also run without Qt or a configured Mumble build. From the
repository root:

```sh
build_dir="$(mktemp -d)"
cc -std=c11 -Wall -Wextra -Werror -Iipcutils \
    -c src/tests/TestIPCUtils/TestIPCUtilsC.c -o "$build_dir/TestIPCUtilsC.o"
g++ -std=c++20 -Wall -Wextra -Werror -Iipcutils \
    src/tests/TestIPCUtils/TestIPCUtils.cpp \
    ipcutils/IPCUtils.cpp ipcutils/IPCUtils_c.cpp \
    "$build_dir/TestIPCUtilsC.o" -o "$build_dir/TestIPCUtils"
"$build_dir/TestIPCUtils"
```

Run as an unprivileged user to exercise the unwritable-parent case. Existing
system directories are inspected read-only for the foreign-owned-base case;
that case skips if no suitable directory is available. The suite does not
require root, change system paths, or manufacture foreign-owned directories.
