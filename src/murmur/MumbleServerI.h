// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MURMUR_MURMURI_H_
#define MUMBLE_MURMUR_MURMURI_H_

#include <MumbleServer.h>

#include <exception>
#include <functional>

namespace MumbleServer {

/// Bundles the response and exception callbacks that Ice passes to asynchronously dispatched
/// functions (AMD), so that they can be handed on as a single object.
template< typename Signature > class AMDCallback;

template< typename... Args > class AMDCallback< void(Args...) > {
public:
	AMDCallback(std::function< void(Args...) > response, std::function< void(std::exception_ptr) > exception)
		: m_response(std::move(response)), m_exception(std::move(exception)) {}

	void ice_response(Args... args) { m_response(args...); }

	// This is a template so that the exception is not sliced to a base class when being stored
	template< typename Exception > void ice_exception(const Exception &e) { m_exception(std::make_exception_ptr(e)); }

private:
	std::function< void(Args...) > m_response;
	std::function< void(std::exception_ptr) > m_exception;
};

} // namespace MumbleServer

// Declares the AMD_*Ptr callback types as well as the ServerI and MetaI servant classes
#include "MumbleServerIceWrapper.h"

#endif
