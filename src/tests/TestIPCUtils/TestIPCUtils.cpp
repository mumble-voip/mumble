// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "IPCUtils.h"

#include <cerrno>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>

extern "C" int test_overlay_pipe_path(const char *expected);

namespace {
namespace fs = std::filesystem;

void require(bool condition, const std::string &message) {
	if (!condition) {
		throw std::runtime_error(message);
	}
}

void setEnvironment(const char *name, const std::string &value) {
	require(::setenv(name, value.c_str(), 1) == 0, std::string("setenv: ") + name);
}

void setMode(const fs::path &path, mode_t mode) {
	require(::chmod(path.c_str(), mode) == 0, "chmod: " + path.string());
}

void makeDirectory(const fs::path &path, mode_t mode = 0700) {
	fs::create_directory(path);
	setMode(path, mode);
}

struct stat fileStatus(const fs::path &path) {
	struct stat status;
	require(::lstat(path.c_str(), &status) == 0, "lstat: " + path.string());
	return status;
}

void requirePrivateDirectory(const fs::path &path) {
	const struct stat status = fileStatus(path);
	require(S_ISDIR(status.st_mode), "Not a real directory: " + path.string());
	require(status.st_uid == ::getuid(), "Directory has wrong owner");
	require((status.st_mode & 07777) == 0700, "Directory permissions are not exactly 0700");
	require(::access(path.c_str(), W_OK | X_OK) == 0, "Directory is not usable");
}

bool unchanged(const struct stat &before, const struct stat &after) {
#ifdef __APPLE__
	const bool sameTime = before.st_ctimespec.tv_sec == after.st_ctimespec.tv_sec
						  && before.st_ctimespec.tv_nsec == after.st_ctimespec.tv_nsec;
#else
	const bool sameTime =
		before.st_ctim.tv_sec == after.st_ctim.tv_sec && before.st_ctim.tv_nsec == after.st_ctim.tv_nsec;
#endif
	return sameTime && before.st_dev == after.st_dev && before.st_ino == after.st_ino && before.st_mode == after.st_mode
		   && before.st_uid == after.st_uid && before.st_gid == after.st_gid;
}

void expectSuccess(const fs::path &expected) {
	require(Mumble::getRuntimeDirectory() == expected, "Unexpected runtime directory");
	requirePrivateDirectory(expected);
	require(Mumble::getOverlayPipePath() == expected / "MumbleOverlayPipe", "Unexpected overlay endpoint");
	require(Mumble::getSocketPath("TestRPC") == expected / "TestRPCSocket", "Unexpected RPC endpoint");
	require(test_overlay_pipe_path((expected / "MumbleOverlayPipe").c_str()), "C caller received wrong endpoint");
}

template< typename Function > void expectFilesystemError(Function function) {
	bool threw = false;
	try {
		function();
	} catch (const fs::filesystem_error &error) {
		threw = true;
		require(static_cast< bool >(error.code()), "Filesystem failure has no error code");
	}
	require(threw, "Expected std::filesystem::filesystem_error");
}

void expectFailure(const fs::path &root) {
	expectFilesystemError([] { Mumble::getRuntimeDirectory(); });
	expectFilesystemError([] { Mumble::getOverlayPipePath(); });
	expectFilesystemError([] { Mumble::getSocketPath("TestRPC"); });
	require(test_overlay_pipe_path(nullptr), "C caller did not receive NULL on failure");
	require(!fs::exists(root / "cwd" / "info.mumble.Mumble"), "Failure created a cwd fallback");
}

fs::path tempLeaf(const fs::path &root) {
	return root / "tmp" / ("info.mumble.Mumble-" + std::to_string(::getuid()));
}

// Failed permission tests may leave inaccessible directories. Only restore permissions on
// real directories inside our own fixture tree; never follow a test symlink while cleaning up.
void makeRemovable(const fs::path &path) {
	std::error_code error;
	if (!fs::is_directory(fs::symlink_status(path, error))) {
		return;
	}
	fs::permissions(path, fs::perms::owner_all, fs::perm_options::add, error);
	for (const fs::directory_entry &entry : fs::directory_iterator(path, error)) {
		makeRemovable(entry.path());
	}
}

class TemporaryDirectory {
public:
	TemporaryDirectory() {
		std::string pattern = (fs::temp_directory_path() / "mumble-ipc-test-XXXXXX").string();
		require(::mkdtemp(pattern.data()) != nullptr, "mkdtemp failed");
		path = pattern;
	}

	~TemporaryDirectory() {
		makeRemovable(path);
		std::error_code error;
		fs::remove_all(path, error);
	}

	fs::path path;
};

class Skipped : public std::runtime_error {
public:
	using std::runtime_error::runtime_error;
};

int freshProcess(const std::function< void() > &test) {
	std::cout.flush();
	std::cerr.flush();
	const pid_t child = ::fork();
	require(child >= 0, "fork failed");
	if (child == 0) {
		try {
			test();
			::_exit(0);
		} catch (const Skipped &error) {
			std::cerr << "SKIP: " << error.what() << std::endl;
			::_exit(77);
		} catch (const std::exception &error) {
			std::cerr << "FAIL: " << error.what() << std::endl;
			::_exit(1);
		} catch (...) {
			std::cerr << "FAIL: unexpected exception" << std::endl;
			::_exit(1);
		}
	}
	int status = 0;
	while (::waitpid(child, &status, 0) < 0) {
		require(errno == EINTR, "waitpid failed");
	}
	return WIFEXITED(status) ? WEXITSTATUS(status) : 1;
}

int failures = 0;
int skipped  = 0;
int total    = 0;

void run(const std::string &name, const std::function< void(const fs::path &) > &test) {
	TemporaryDirectory fixture;
	std::cout << name << std::endl;
	const int status = freshProcess([&] {
		::umask(0077);
		require(::unsetenv("XDG_RUNTIME_DIR") == 0, "unsetenv failed");
		makeDirectory(fixture.path / "tmp", 01777);
		makeDirectory(fixture.path / "cwd");
		setEnvironment("TMPDIR", (fixture.path / "tmp").string());
		fs::current_path(fixture.path / "cwd");
		test(fixture.path);
	});
	++total;
	skipped += status == 77;
	failures += status != 0 && status != 77;
}

} // namespace

int main() {
	// The parent never resolves a runtime directory. Every case starts with an uninitialized
	// function-local cache, while tests that need repeated calls do so inside their own child.
	run("XDG creates a private leaf even with a restrictive umask", [](const fs::path &root) {
		const fs::path base = root / "xdg";
		makeDirectory(base);
		setEnvironment("XDG_RUNTIME_DIR", base.string());
		::umask(0777);
		expectSuccess(base / "info.mumble.Mumble");
		require(!fs::exists(tempLeaf(root)), "XDG unexpectedly used the temp fallback");
	});

	for (const bool useXdg : { false, true }) {
		const std::string kind = useXdg ? "XDG" : "temp";
		run(kind + " creates a private leaf with a permissive umask", [=](const fs::path &root) {
			const fs::path base = useXdg ? root / "xdg" : root / "tmp";
			if (useXdg) {
				makeDirectory(base);
				setEnvironment("XDG_RUNTIME_DIR", base.string());
			}
			::umask(0000);
			expectSuccess(useXdg ? base / "info.mumble.Mumble" : tempLeaf(root));
		});

		run(kind + " accepts an existing private leaf without chmod", [=](const fs::path &root) {
			const fs::path base = useXdg ? root / "xdg" : root / "tmp";
			if (useXdg) {
				makeDirectory(base);
				setEnvironment("XDG_RUNTIME_DIR", base.string());
			}
			const fs::path leaf = useXdg ? base / "info.mumble.Mumble" : tempLeaf(root);
			makeDirectory(leaf);
			std::ofstream(leaf / "marker") << "preserve me";
			const struct stat before = fileStatus(leaf);
			expectSuccess(leaf);
			require(unchanged(before, fileStatus(leaf)), "Existing leaf metadata changed");
			require(fs::exists(leaf / "marker"), "Existing leaf contents changed");
		});

		for (const mode_t mode : { 0755, 0701, 0777, 0600, 01700, 02700 }) {
			run(kind + " rejects insecure leaf mode " + std::to_string(mode), [=](const fs::path &root) {
				const fs::path base = useXdg ? root / "xdg" : root / "tmp";
				if (useXdg) {
					makeDirectory(base);
					setEnvironment("XDG_RUNTIME_DIR", base.string());
				}
				const fs::path leaf = useXdg ? base / "info.mumble.Mumble" : tempLeaf(root);
				makeDirectory(leaf, mode);
				const struct stat before = fileStatus(leaf);
				expectFailure(root);
				require(unchanged(before, fileStatus(leaf)), "Rejected leaf metadata changed");
			});
		}

		for (const std::string obstruction : { "file", "symlink", "dangling symlink" }) {
			run(kind + " rejects a " + obstruction + " leaf", [=](const fs::path &root) {
				const fs::path base = useXdg ? root / "xdg" : root / "tmp";
				if (useXdg) {
					makeDirectory(base);
					setEnvironment("XDG_RUNTIME_DIR", base.string());
				}
				const fs::path leaf   = useXdg ? base / "info.mumble.Mumble" : tempLeaf(root);
				const fs::path target = root / "target";
				if (obstruction == "file") {
					std::ofstream(leaf) << "preserve me";
				} else {
					if (obstruction == "symlink") {
						makeDirectory(target, 0755);
					}
					fs::create_directory_symlink(target, leaf);
				}
				const struct stat before = fileStatus(leaf);
				expectFailure(root);
				require(unchanged(before, fileStatus(leaf)), "Rejected leaf metadata changed");
				if (obstruction == "symlink") {
					require((fileStatus(target).st_mode & 07777) == 0755, "Symlink target was chmodded");
				} else if (obstruction == "dangling symlink") {
					require(!fs::exists(target), "Dangling symlink target was created");
				}
			});
		}

		for (const std::string obstruction : { "missing", "file", "symlink", "dangling symlink", "relative" }) {
			if ((!useXdg && obstruction == "symlink") || (useXdg && obstruction == "relative")) {
				continue;
			}
			run(kind + " rejects a " + obstruction + " base", [=](const fs::path &root) {
				fs::path base = root / "bad-base";
				if (obstruction == "file") {
					std::ofstream(base) << "preserve me";
				} else if (obstruction == "symlink" || obstruction == "dangling symlink") {
					if (obstruction == "symlink") {
						makeDirectory(root / "target");
					}
					fs::create_directory_symlink(root / "target", base);
				} else if (obstruction == "relative") {
					base = "relative-base";
					makeDirectory(base);
				}
				setEnvironment(useXdg ? "XDG_RUNTIME_DIR" : "TMPDIR", base.string());
				expectFailure(root);
				if (obstruction == "missing") {
					require(!fs::exists(base), "Missing base was created");
				}
				if (useXdg) {
					require(!fs::exists(tempLeaf(root)), "Invalid XDG fell back to temp");
				}
			});
		}
	}

	run("Relative XDG is ignored and uses temp", [](const fs::path &root) {
		makeDirectory("relative-base");
		setEnvironment("XDG_RUNTIME_DIR", "relative-base");
		expectSuccess(tempLeaf(root));
		require(!fs::exists("relative-base/info.mumble.Mumble"), "Relative XDG was used");
	});

	run("Symlinked temp base is supported", [](const fs::path &root) {
		const fs::path base = root / "linked-temp";
		fs::create_directory_symlink(root / "tmp", base);
		setEnvironment("TMPDIR", base.string());
		expectSuccess(base / ("info.mumble.Mumble-" + std::to_string(::getuid())));
	});

	run("XDG symlink with trailing separators is rejected", [](const fs::path &root) {
		makeDirectory(root / "target");
		fs::create_directory_symlink(root / "target", root / "xdg");
		setEnvironment("XDG_RUNTIME_DIR", (root / "xdg").string() + "///");
		expectFailure(root);
		require(!fs::exists(root / "target" / "info.mumble.Mumble"), "Created leaf through an XDG symlink");
	});

	run("XDG base with trailing separators is supported", [](const fs::path &root) {
		makeDirectory(root / "xdg");
		setEnvironment("XDG_RUNTIME_DIR", (root / "xdg").string() + "///");
		expectSuccess(root / "xdg" / "info.mumble.Mumble");
	});

	for (const mode_t mode : { 0777, 0770, 0702 }) {
		run("Non-sticky writable temp base is rejected " + std::to_string(mode), [=](const fs::path &root) {
			setMode(root / "tmp", mode);
			const struct stat before = fileStatus(root / "tmp");
			expectFailure(root);
			require(unchanged(before, fileStatus(root / "tmp")), "Rejected temp base metadata changed");
		});
	}

	run("Private temp base is supported", [](const fs::path &root) {
		setMode(root / "tmp", 0700);
		expectSuccess(tempLeaf(root));
	});

	for (const bool empty : { false, true }) {
		run(empty ? "Empty XDG uses temp" : "Absent XDG uses temp", [=](const fs::path &root) {
			if (empty) {
				setEnvironment("XDG_RUNTIME_DIR", "");
			}
			::umask(0777);
			expectSuccess(tempLeaf(root));
		});
	}

	for (const mode_t mode : { 0755, 0711, 0770, 01700 }) {
		run("XDG rejects insecure base mode " + std::to_string(mode), [=](const fs::path &root) {
			const fs::path base = root / "xdg";
			makeDirectory(base, mode);
			setEnvironment("XDG_RUNTIME_DIR", base.string());
			const struct stat before = fileStatus(base);
			expectFailure(root);
			require(unchanged(before, fileStatus(base)), "Rejected base metadata changed");
		});
	}

	run("Unwritable temp base fails without cwd fallback", [](const fs::path &root) {
		if (::geteuid() == 0) {
			throw Skipped("Root can bypass directory write permissions");
		}
		setMode(root / "tmp", 0500);
		expectFailure(root);
		require(!fs::exists(tempLeaf(root)), "Created leaf under unwritable parent");
	});

	run("Foreign-owned XDG base is rejected without modification", [](const fs::path &root) {
		// Inspect existing system directories only; never chown or create anything in them.
		for (const fs::path base : { "/usr", "/etc" }) {
			const struct stat before = fileStatus(base);
			if (before.st_uid != ::getuid()) {
				setEnvironment("XDG_RUNTIME_DIR", base.string());
				expectFailure(root);
				require(unchanged(before, fileStatus(base)), "Foreign-owned base metadata changed");
				return;
			}
		}
		throw Skipped("No safe foreign-owned base fixture is available");
	});

	run("Successful result is cached across environment and cwd changes", [](const fs::path &root) {
		const fs::path expected = tempLeaf(root);
		expectSuccess(expected);
		makeDirectory(root / "other");
		fs::current_path(root / "other");
		setEnvironment("XDG_RUNTIME_DIR", "invalid-relative-path");
		setEnvironment("TMPDIR", "/nonexistent/mumble-test-temp");
		expectSuccess(expected);
	});

	run("Failed initialization can be retried after repair", [](const fs::path &root) {
		const fs::path base = root / "xdg";
		setEnvironment("XDG_RUNTIME_DIR", base.string());
		expectFailure(root);
		makeDirectory(base);
		expectSuccess(base / "info.mumble.Mumble");
	});

	for (const bool useXdg : { false, true }) {
		run(std::string(useXdg ? "XDG" : "temp") + " resolves identically in independent processes",
			[=](const fs::path &root) {
				const fs::path base = root / "xdg";
				if (useXdg) {
					makeDirectory(base);
					setEnvironment("XDG_RUNTIME_DIR", base.string());
				}
				const fs::path expected = useXdg ? base / "info.mumble.Mumble" : tempLeaf(root);
				require(freshProcess([&] { expectSuccess(expected); }) == 0, "First process failed");
				// Change cwd and the availability of an irrelevant local directory between launches.
				makeDirectory(root / "other");
				makeDirectory(root / "cwd" / "info.mumble.Mumble", 0777);
				fs::current_path(root / "other");
				if (useXdg) {
					setMode(root / "tmp", 0500);
				}
				require(freshProcess([&] { expectSuccess(expected); }) == 0, "Second process failed");
			});
	}

	run("Different explicit environments select different endpoints", [](const fs::path &root) {
		for (const std::string name : { "first", "second" }) {
			const fs::path base = root / name;
			makeDirectory(base);
			setEnvironment("XDG_RUNTIME_DIR", base.string());
			require(freshProcess([&] { expectSuccess(base / "info.mumble.Mumble"); }) == 0, "Child failed");
		}
	});

	std::cout << total << " cases, " << failures << " failed, " << skipped << " skipped" << std::endl;
	return failures == 0 ? 0 : 1;
}
