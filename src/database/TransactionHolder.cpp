// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "TransactionHolder.h"

#include <cassert>

#include <iostream>

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
		: m_active(other.m_active), m_transactionStatusFlag(other.m_transactionStatusFlag), m_sql(other.m_sql) {
		// Prevent other from automatic rollback
		other.m_active = false;
	}

	TransactionHolder::~TransactionHolder() {
		if (m_active) {
			// Wrapper went out of scope without explicit handling -> rollback as this indicates an error has happened
			// We normally ignore any exception that might be throw here in order to not throw from the dtor
			try {
				m_sql.rollback();

				// Indicate that the transaction has ended
				if (m_transactionStatusFlag) {
					*m_transactionStatusFlag = false;
				}
			} catch (...) {
				// In debug builds (or more general: builds with assertions enabled) we want to abort here as an error
				// encountered during rollback of a transaction indicates a bug of some sort, that we should
				// investigate.
				assert(false);
			}
		}
	}

	void TransactionHolder::commit() {
		if (m_active) {
			// If this is not the "active" holder, then committing does not actually commit in order to not interrupt
			// the transaction prematurely
			m_sql.commit();

			// Indicate that the transaction has ended
			if (m_transactionStatusFlag) {
				*m_transactionStatusFlag = false;
			}

			m_active = false;
		}
	}

} // namespace db
} // namespace mumble
