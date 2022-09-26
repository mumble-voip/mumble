// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Trigger.h"
#include "Table.h"
#include "UnsupportedOperationException.h"

#include <boost/algorithm/string.hpp>

#include <cassert>

namespace mumble {
namespace db {

	Trigger::Trigger(const std::string &name, Trigger::Timing timing, Trigger::Event event,
					 const std::string &triggerBody, const std::string &condition)
		: m_name(name), m_timing(timing), m_event(event), m_triggerBody(triggerBody), m_condition(condition) {
		// Ensure trigger body always ends in semicolon
		if (!boost::ends_with(m_triggerBody, ";") && !m_triggerBody.empty()) {
			m_triggerBody += ";";
		}
		// Ensure condition never ends in semicolon
		if (boost::ends_with(m_condition, ";")) {
			m_condition.erase(m_condition.size() - 1);
		}
	}

	const std::string &Trigger::getName() const { return m_name; }

	void Trigger::setName(const std::string &name) { m_name = name; }

	Trigger::Timing Trigger::getTiming() const { return m_timing; }

	void Trigger::setTiming(Trigger::Timing timing) { m_timing = timing; }

	Trigger::Event Trigger::getEvent() const { return m_event; }

	void Trigger::setEvent(Trigger::Event event) { m_event = event; }

	const std::string &Trigger::getBody() const { return m_triggerBody; }

	void Trigger::setBody(const std::string &body) {
		m_triggerBody = body;

		// Ensure trigger body always ends in semicolon
		if (!boost::ends_with(m_triggerBody, ";") && !m_triggerBody.empty()) {
			m_triggerBody += ";";
		}
	}

	const std::string &Trigger::getCondition() const { return m_condition; }

	void Trigger::setCondition(const std::string &condition) {
		m_condition = condition;

		// Ensure condition never ends in semicolon
		if (boost::ends_with(m_condition, ";")) {
			m_condition.erase(m_condition.size() - 1);
		}
	}

	bool Trigger::hasCondition() const { return !getCondition().empty(); }

	bool Trigger::dropBeforeDeleteTable() const { return m_dropBeforeDelete; }

	void Trigger::setDropBeforeDeleteTable(bool drop) { m_dropBeforeDelete = drop; }

	bool Trigger::created() const { return m_created; }

	void Trigger ::setCreated(bool created) { m_created = created; }

	std::string Trigger::creationQuery(const Table &table, Backend backend) const {
		std::string query = "CREATE TRIGGER \"" + m_name + "\"";
		switch (m_timing) {
			case Trigger::Timing::Before:
				query += " BEFORE ";
				break;
			case Trigger::Timing::After:
				query += " AFTER ";
				break;
		}

		switch (m_event) {
			case Trigger::Event::Insert:
				query += " INSERT ";
				break;
			case Trigger::Event::Update:
				query += " UPDATE ";
				break;
			case Trigger::Event::Delete:
				query += " DELETE ";
				break;
		}

		query += "ON \"" + table.getName() + "\" FOR EACH ROW ";

		switch (backend) {
			case Backend::SQLite: {
				if (hasCondition()) {
					query += "WHEN (" + m_condition + ") ";
				}
				query += "BEGIN " + m_triggerBody + " END";
				break;
			}
			case Backend::MySQL: {
				if (hasCondition()) {
					// MySQL doesn't support the WHEN clause -> we require a workaround using IF statement
					query += "BEGIN DECLARE trigger_condition BOOLEAN DEFAULT FALSE; ";
					query += "SET trigger_condition = (SELECT " + m_condition + "); ";
					query += "IF trigger_condition THEN " + m_triggerBody + " END IF; END";
				} else {
					query += "BEGIN " + m_triggerBody + " END";
				}
				break;
			}
			case Backend::PostgreSQL: {
				if (boost::istarts_with(m_triggerBody, "EXECUTE PROCEDURE")) {
					if (hasCondition()) {
						throw UnsupportedOperationException(
							"Can't use conditions with triggers containing an EXECUTE PROCEDURE body");
					}

					query += m_triggerBody;
				} else {
					// Postgres requires us to create a function that can then be executed by the trigger
					std::string queryPrefix = "CREATE FUNCTION \"" + m_name + "_trigger_function\""
											  + "() RETURNS TRIGGER LANGUAGE PLPGSQL AS $$ ";
					if (hasCondition()) {
						queryPrefix += "DECLARE trigger_condition BOOLEAN DEFAULT FALSE;";
						queryPrefix += "BEGIN SELECT " + m_condition + " INTO trigger_condition;";
						queryPrefix += "IF trigger_condition THEN ";
					} else {
						queryPrefix += "BEGIN ";
					}

					queryPrefix += m_triggerBody;

					if (hasCondition()) {
						queryPrefix += " END IF; ";
					}

					// In PostgreSQL a trigger function must always return a value. Returning anything but NEW here,
					// might alter the original query this trigger was fired for.
					queryPrefix += "RETURN NEW; ";

					queryPrefix += " END; $$; ";

					query = queryPrefix + query + "EXECUTE PROCEDURE \"" + m_name + "_trigger_function\"()";
				}
				break;
			}
		}

		return query;
	}

	std::string Trigger::dropQuery(const Table &table, Backend backend) const {
		switch (backend) {
			case Backend::MySQL:
				// Fallthrough
			case Backend::SQLite:
				return "DROP TRIGGER \"" + m_name + "\"";
			case Backend::PostgreSQL:
				std::string query = "DROP TRIGGER \"" + m_name + "\" ON \"" + table.getName() + "\";";

				// Also drop the function that we created for this trigger
				query += " DROP FUNCTION IF EXISTS \"" + m_name + "_trigger_function\"()";

				return query;
		}

		// This code should be unreachable
		assert(false);

		return "DROP TRIGGER \"" + m_name + "\"";
	}

	bool operator==(const Trigger &lhs, const Trigger &rhs) {
		return lhs.m_name == rhs.m_name && lhs.m_timing == rhs.m_timing && lhs.m_event == rhs.m_event
			   && lhs.m_triggerBody == rhs.m_triggerBody;
	}

} // namespace db
} // namespace mumble
