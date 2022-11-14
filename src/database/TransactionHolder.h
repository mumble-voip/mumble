// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_TRANSACTIONHOLDER_H_
#define MUMBLE_DATABASE_TRANSACTIONHOLDER_H_

#include "NonCopyable.h"

#include <soci/soci.h>

namespace mumble {
namespace db {

	/**
	 * RAII wrapper for managing database transactions. If the holder goes out of scope without the transaction having
	 * been committed explicitly, the transaction is rolled back. If the holder is not "active" (i.e. it holds the
	 * outermost virtual transaction (and thus the only real one)) call to commit will be no-ops (other than preventing
	 * an automatic rollback).
	 */
	class TransactionHolder : public NonCopyable {
	public:
		TransactionHolder(soci::session &sql, bool active, bool *transactionStatusFlag = nullptr);
		TransactionHolder(TransactionHolder &&) noexcept;
		~TransactionHolder();

		void commit();
		void rollback();

		bool isActive() const;

	protected:
		bool m_active                 = false;
		bool m_handled                = false;
		bool *m_transactionStatusFlag = nullptr;
		soci::session &m_sql;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_TRANSACTIONHOLDER_H_
