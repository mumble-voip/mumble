// Copyright 2005-2017 The Mumble Developers. All rights reserved.
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

		/// Consruct a CryptographicHash object that can be used
		/// to incrementally compute a cryptographic hash using the
		/// hash function specified in |algo|.
		CryptographicHash(Algorithm algo);

		/// addData adds the content of |buf| to the hash computation.
		void addData(const QByteArray &buf);

		/// result() finalizes the CryptographicHash object,
		/// and returns the computed hash.
		/// If an error occurred during the computatiopn, the
		/// returned QByteArray will be empty.
		QByteArray result() const;

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

	private:
		CryptographicHash();
		CryptographicHashPrivate *d;
};

#endif
