// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_CRYPTOGRAPHICHASH_H_
#define MUMBLE_CRYPTOGRAPHICHASH_H_

#include <QByteArray>

class CryptographicHashPrivate;

/// CryptographicHash is used for computing cryptographic hashes.
class CryptographicHash {
public:
	/// Algorithm specifies an algorithm for use with CryptographicHash.
	enum Algorithm {
		/// The SHA-1 hashing algorithm.
		Sha1,
		/// The SHA-256 hashing algorithm.
		Sha256
	};

	/// Compute the cryptographic hash of |buf| using algorithm |algo|
	/// and return the result.
	/// On failure, the returned QByteArray will be empty.
	static QByteArray hash(const QByteArray &buf, Algorithm algo);

	/// Return the human readable name of |algo|.
	///
	/// Examples:
	///  CryptographicHash::Sha1 -> "SHA-1"
	///  CryptographicHash::Sha256 -> "SHA-256"
	static QString humanReadableAlgorithmName(CryptographicHash::Algorithm algo);

	/// Return a machine-readable short name of |algo|.
	/// This variant is a short, lower-case name for the algorithm, suitable
	/// for prefixing when serializing a cryptographic hash.
	///
	/// Examples:
	///  CryptographicHash::Sha1 -> "sha1"
	///  CryptographicHash::Sha256 -> "sha256"
	static QString shortAlgorithmName(CryptographicHash::Algorithm algo);

	/// Consruct a CryptographicHash object that can be used
	/// to incrementally compute a cryptographic hash using the
	/// hash function specified in |algo|.
	CryptographicHash(Algorithm algo);

	/// addData adds the content of |buf| to the hash computation.
	///
	/// Attempting to add data to the CryptographicHash after result()
	/// has been called is an error, and will cause the CryptographicHash
	/// object to transition to an error state. Subsequent calls to result()
	/// will return an empty QByteArray.
	void addData(const QByteArray &buf);

	/// result() finalizes the CryptographicHash object,
	/// and returns the computed hash.
	/// After calling this method, it is no longer possible
	/// to add more data to the CryptographicHash object.
	/// If an error occurred during the computation, the
	/// returned QByteArray will be empty.
	QByteArray result() const;

private:
	CryptographicHash();
	CryptographicHashPrivate *d;
};

#endif
