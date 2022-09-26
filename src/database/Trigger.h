// Copyright 2022 The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_DATABASE_TRIGGER_H_
#define MUMBLE_DATABASE_TRIGGER_H_

#include "Backend.h"

#include <string>

namespace mumble {
namespace db {

	class Table;

	class Trigger {
	public:
		enum class Timing {
			Before,
			After,
		};
		enum class Event {
			Insert,
			Update,
			Delete,
		};

		Trigger(const std::string &name = "", Timing timing = Timing::After, Event event = Event::Insert,
				const std::string &triggerBody = "", const std::string &condition = "");

		const std::string &getName() const;
		void setName(const std::string &name);

		Timing getTiming() const;
		void setTiming(Timing timing);

		Event getEvent() const;
		void setEvent(Event event);

		const std::string &getBody() const;
		void setBody(const std::string &body);

		const std::string &getCondition() const;
		void setCondition(const std::string &condition);
		bool hasCondition() const;

		bool dropBeforeDeleteTable() const;
		void setDropBeforeDeleteTable(bool drop);

		bool created() const;
		void setCreated(bool created);

		std::string creationQuery(const Table &table, Backend backend) const;
		std::string dropQuery(const Table &table, Backend backend) const;

		friend bool operator==(const Trigger &lhs, const Trigger &rhs);
		friend bool operator!=(const Trigger &lhs, const Trigger &rhs);

	protected:
		std::string m_name;
		Timing m_timing;
		Event m_event;
		std::string m_triggerBody;
		std::string m_condition;
		bool dropBeforeDelete = true;
		bool m_created         = false;
	};

} // namespace db
} // namespace mumble

#endif // MUMBLE_DATABASE_TRIGGER_H_
