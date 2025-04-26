# Profiling

Mumble comes with built-in support for the [Tracy](https://github.com/wolfpld/tracy) profiler. In order to activate the baked-in instrumentation, use
`-Dtracy=ON` when compiling Mumble. When using that option, you should see a line in the cmake output that says `TRACY_ENABLE: ON`.


## Instrumented parts

Currently only the Mumble server is instrumented (can be profiled using Tracy).


## Instructions

Once you have built Mumble with `-Dtracy=ON`, Mumble will act as a Tracy _client_ (in Tracy terms) which means that you can connect any Tracy _server_
to it. Most commonly, you'll want to use either the `profiler` or the `capture`. Both of these programs live in the tracy submodule
(`3rdparty/tracy/`) and can be built from there. For build instructions, see the
[Tracy Manual](https://github.com/wolfpld/tracy/releases/latest/download/tracy.pdf).

The `profiler` is an interactive GUI that can be attached to a currently running Mumble instance to see the profiling data (more or less) in realtime
or you can open a previously recorded trace for analysis. Note that it is also possible to connect to a Mumble instance on a remote machine using this
tool. All that is required is that it is able to establish a TCP connection to the target machine.

The `capture` tool has to be run on the same machine as the Mumble instance that shall be profiled. It is a command-line tool that will attach to a
running Tracy _client_ as soon as it is started. It will dump the captured data directly into a file, that can later on be opened in `profiler` for
further analysis. This tool is recommended when you want to profile over a longer period of time or you don't want to add the burden of sending all
profiling data out through the network, while Mumble is running.


## Notes

- Profiling should generally be done in `Release` mode in order to obtain reasonable data
- If you are having issues connecting your Tracy _server_ to the Mumble server, you should not let it fork. The default behavior in release
  mode is to fork, but you can change that by using the `-fg` parameter when starting the server.
