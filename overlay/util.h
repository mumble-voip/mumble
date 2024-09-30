// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_UTIL_H_
#define MUMBLE_UTIL_H_

#include <algorithm>
#include <string>
#include <vector>

/// Returns true if |path| is an absolute path.
/// Returns false if |path| is not an absolute path.
static inline bool isAbsPath(const std::string &path) {
	return path.find("\\") != std::string::npos;
}

/// Convert the string |s| to lowercase and return it.
static inline std::string slowercase(std::string s) {
	std::transform(s.begin(), s.end(), s.begin(), tolower);
	return s;
}

/// Convert all entries of |vec| to lowercase and return the resulting vector.
static inline std::vector< std::string > vlowercase(std::vector< std::string > vec) {
	std::transform(vec.begin(), vec.end(), vec.begin(), slowercase);
	return vec;
}

/// Merge |v1| and |v2| and return the result.
template< typename T > static inline std::vector< T > vmerge(std::vector< T > v1, const std::vector< T > &v2) {
	v1.insert(v1.end(), v2.begin(), v2.end());
	return v1;
}

/// Filter out all entries of |vremove| from |v| and return the result.
template< typename T >
static inline std::vector< T > vexclude(const std::vector< T > &v, const std::vector< T > &vremove) {
	std::vector< T > out;
	std::set_difference(v.begin(), v.end(), vremove.begin(), vremove.end(), std::inserter(out, out.begin()));
	return out;
}

/// Find the intersection between |v1| and |v2| and return the result.
template< typename T >
static inline std::vector< T > vintersect(const std::vector< T > &v1, const std::vector< T > &v2) {
	std::vector< T > out;
	std::set_intersection(v1.begin(), v1.end(), v2.begin(), v2.end(), std::inserter(out, out.begin()));
	return out;
}

#endif
