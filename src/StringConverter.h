// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_STRINGCONVERTER_H_
#define MUMBLE_STRINGCONVERTER_H_

#include <string>

namespace mumble {

/**
 * Template helper to perform conversions from std::string to various other types. Note that
 * this template only supports those types for which there exist explicit specializations.
 */
template< typename T > struct StringConverter {
	/**
	 * Converts the given string to the given type. If the conversion failed, a default
	 * constructed value will be returned.
	 *
	 * @param The string to convert
	 * @param An optional pointer to a bool. If this is anything but the nullptr, the function will write a flag to it
	 * indicating whether the conversion was successful.
	 */
	static T convert(const std::string &, bool * = nullptr) = delete;
};

template<> struct StringConverter< std::string > {
	static std::string convert(const std::string &str, bool *success = nullptr) {
		if (success) {
			*success = true;
		}
		return str;
	}
};

template<> struct StringConverter< int > {
	static int convert(const std::string &str, bool *success = nullptr) {
		try {
			std::size_t readChars = 0;
			int val               = std::stoi(str, &readChars);

			if (readChars != str.size()) {
				// We failed to read the entire string as an int
				if (success) {
					*success = false;
				}
				return {};
			} else {
				if (success) {
					*success = true;
				}
				return val;
			}
		} catch (const std::exception &) {
			// Conversion failed
			if (success) {
				*success = false;
			}

			return {};
		}
	}
};

template<> struct StringConverter< unsigned long > {
	static unsigned long convert(const std::string &str, bool *success = nullptr) {
		try {
			std::size_t readChars = 0;
			unsigned long val     = std::stoul(str, &readChars);

			if (readChars != str.size()) {
				// We failed to read the entire string as an int
				if (success) {
					*success = false;
				}
				return {};
			} else {
				if (success) {
					*success = true;
				}
				return val;
			}
		} catch (const std::exception &) {
			// Conversion failed
			if (success) {
				*success = false;
			}

			return {};
		}
	}
};

template<> struct StringConverter< unsigned int > : StringConverter< unsigned long > {
	static unsigned int convert(const std::string &str, bool *success = nullptr) {
		return StringConverter< unsigned long >::convert(str, success);
	}
};

} // namespace mumble

#endif
