// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_FFDHE_H_
#define MUMBLE_FFDHE_H_

#include <QByteArray>
#include <QStringList>

/// FFDHE provides access to the Diffie-Hellman parameters from RFC 7919.
class FFDHE {
public:
	/// NamedGroups returns a list of the supported named
	/// groups for PEMForNamedGroup.
	static QStringList NamedGroups();

	/// PEMForNamedGroup returns the PEM-encoded
	/// Diffie-Hellman parameters for the RFC 7919
	/// group with the given name, such as "ffdhe2048",
	/// "ffdhe4096", etc.
	///
	/// Returns an empty byte array if the request
	/// could not be fulfilled.
	static QByteArray PEMForNamedGroup(QString name);
};

#endif
