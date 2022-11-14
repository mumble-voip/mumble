// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TransactionHolder.h"

#include <cassert>

namespace mumble {
namespace db {

	TransactionHolder::TransactionHolder(soci::session &sql, bool active, bool *transactionStatusFlag)
		: m_active(active), m_transactionStatusFlag(transactionStatusFlag), m_sql(sql) {
		if (m_active) {
			// auto-start transaction
			m_sql.begin();

			// Indicate that the transaction has started
			if (m_transactionStatusFlag) {
				*m_transactionStatusFlag = true;
			}
		}
	}

	TransactionHolder::TransactionHolder(TransactionHolder &&other) noexcept
		: m_active(other.m_active), m_handled(other.m_handled), m_transactionStatusFlag(other.m_transactionStatusFlag),
		  m_sql(other.m_sql) {
		// Prevent other from automatic rollback
		other.m_active  = false;
		other.m_handled = true;
	}

	TransactionHolder::~TransactionHolder() {
		if (!m_handled) {
			// Wrapper went out of scope without explicit handling -> rollback as this indicates an error has happened
			rollback();
		}
	}

	void TransactionHolder::commit() {
		assert(!m_handled);

		if (m_active) {
			// If this is not the "active" holder, then committing does not actually commit in order to not interrupt
			// the transaction prematurely
			m_sql.commit();

			// Indicate that the transaction has ended
			if (m_transactionStatusFlag) {
				*m_transactionStatusFlag = false;
			}
		}

		m_handled = true;
	}

	void TransactionHolder::rollback() {
		assert(!m_handled);

		m_sql.rollback();

		// Indicate that the transaction has ended
		if (m_transactionStatusFlag) {
			*m_transactionStatusFlag = false;
		}

		m_handled = true;
	}

} // namespace db
} // namespace mumble
