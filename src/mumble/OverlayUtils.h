// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.


#ifndef MUMBLE_MUMBLE_OVERLAYUTILS_H_
#define MUMBLE_MUMBLE_OVERLAYUTILS_H_

#include <string>

namespace mumble {
namespace overlay {

	std::string getIPCPath();

	std::string getSocketPath();

	std::string getPipePath();

	std::string createSocket();

	std::string createPipe();

} // namespace overlay
} // namespace mumble


extern "C" {

char *mumble_overlay_create_and_get_socket();

char *mumble_overlay_create_and_get_pipe();
}


#endif // MUMBLE_MUMBLE_OVERLAYUTILS_H_
