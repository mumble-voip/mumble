// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_NONCOPYABLE_H_
#define MUMBLE_NONCOPYABLE_H_

/*
 * Helper class whose only property is that it can't be copied. Intended
 * to be used as superclass to inherit this behavior.
 */
struct NonCopyable {
	NonCopyable() = default;

	NonCopyable(const NonCopyable &) = delete;
	NonCopyable &operator=(const NonCopyable &) = delete;

	NonCopyable(NonCopyable &&) = default;
	NonCopyable &operator=(NonCopyable &&) = default;
};

#endif // MUMBLE_NONCOPYABLE_H_
