// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "Accessibility.h"
#include "ChannelFilterMode.h"
#include "Global.h"

#include <QAbstractButton>
#include <QAccessible>
#include <QAccessibleEvent>
#include <QObject>
#include <QRegularExpression>
#include <QWidget>

#include <queue>

namespace Mumble {
namespace Accessibility {

	QString removeHTMLTags(QString value) { return value.remove(QRegularExpression("<[^>]*>")); }

	void setDescriptionFromLabel(QWidget *widget, const QLabel *label) {
		widget->setAccessibleDescription(removeHTMLTags(label->text()));
	}

	void fixWizardButtonLabels(QWizard *wizard) {
		wizard->button(QWizard::NextButton)->setAccessibleName(QObject::tr("Next"));
		wizard->button(QWizard::BackButton)->setAccessibleName(QObject::tr("Back"));
	}

	QString userToText(const ClientUser *user) {
		if (Global::get().uiSession != 0) {
			const ClientUser *self = ClientUser::get(Global::get().uiSession);
			if (self == user) {
				return QString("%1, %2").arg(QObject::tr("This is you")).arg(user->qsName);
			}
		}

		QString name;
		QString prefix;

		if (!user->qsFriendName.isEmpty()) {
			prefix = QObject::tr("friend");
			name   = user->qsFriendName;
		} else if (!user->getLocalNickname().isEmpty()) {
			prefix = QObject::tr("user");
			name   = user->getLocalNickname();
		} else {
			prefix = QObject::tr("user");
			name   = user->qsName;
		}

		return QString("%1 %2").arg(prefix).arg(name);
	}

	QString userToDescription(const ClientUser *user) {
		QString description;

		if (user->bMute || user->bSuppress || user->bSelfMute) {
			if (user->bDeaf || user->bSelfDeaf) {
				description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("muted and deafened"));
			} else {
				description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("muted"));
			}
		} else if (user->bLocalMute) {
			description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("locally muted"));
		} else {
			description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("unmuted"));
		}

		if (user->bRecording) {
			description += QString("%1,").arg(QObject::tr("recording"));
		}

		if (user->bPrioritySpeaker) {
			description += QString("%1,").arg(QObject::tr("priority speaker"));
		}

		if (!user->qsComment.isEmpty()) {
			if (user->qsComment.length() <= 20) {
				description += QString("%1,").arg(user->qsComment);
			} else {
				description += QString("%1,").arg(QObject::tr("has a long comment"));
			}
		}

		if (user->bLocalIgnore) {
			description += QString("%1,").arg(QObject::tr("text messages ignored"));
		}

		if (user->iId >= 0) {
			description += QString("%1,").arg(QObject::tr("registered"));
		}

		return description;
	}

	QString channelToText(const Channel *channel) {
		return QString("%1 %2").arg(QObject::tr("channel")).arg(channel->qsName);
	}

	QString channelToDescription(const Channel *channel) {
		QString description;
		bool selfChannel = false;

		if (Global::get().uiSession != 0) {
			const ClientUser *self = ClientUser::get(Global::get().uiSession);
			if (self->cChannel == channel) {
				selfChannel = true;
			}
		}

		if (selfChannel) {
			description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("your channel"));
		} else if (channel->hasEnterRestrictions.load()) {
			if (channel->localUserCanEnter.load()) {
				description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("accessible"));
			} else {
				description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("inaccessible"));
			}
		} else {
			description = QString("%1 %2,").arg(QObject::tr("status")).arg(QObject::tr("public"));
		}

		switch (channel->m_filterMode) {
			case ChannelFilterMode::HIDE:
				description += QString("%1,").arg(QObject::tr("filtered"));
				break;
			case ChannelFilterMode::PIN:
				description += QString("%1,").arg(QObject::tr("pinned"));
				break;
			case ChannelFilterMode::NORMAL:
				// NOOP
				break;
		}

		if (!channel->qsDesc.isEmpty()) {
			description += QString(": %1").arg(channel->qsDesc);
		}

		return description;
	}

	void setSliderSemanticValue(SemanticSlider *slider, QString value) {
		slider->m_semanticValue = value;

		QAccessibleEvent nameEvent(slider, QAccessible::NameChanged);
		QAccessible::updateAccessibility(&nameEvent);
	}

	void setSliderSemanticValue(SemanticSlider *slider, SliderMode mode, QString suffix) {
		QString description = QString("%1 %2");

		switch (mode) {
			case SliderMode::NONE:
				description = description.arg("");
				break;
			case SliderMode::READ_RELATIVE:
				description = description.arg(
					QString::number(slider->value() / static_cast< double >(slider->maximum()), 'f', 2));
				break;
			case SliderMode::READ_PERCENT:
				description = description.arg(
					static_cast< int32_t >((slider->value() / static_cast< double >(slider->maximum())) * 100));
				break;
			case SliderMode::READ_ABSOLUTE:
				description = description.arg(slider->value());
				break;
		}
		description = description.arg(suffix);

		setSliderSemanticValue(slider, description);
	}

	QWidget *getFirstFocusableChild(QObject *object) {
		std::queue< QObject * > search;
		search.push(object);

		QWidget *selectedWidget = nullptr;

		while (!search.empty()) {
			QObject *o = search.front();
			search.pop();

			for (QObject *child : o->children()) {
				search.push(child);
			}

			selectedWidget = qobject_cast< QWidget * >(o);
			if (!selectedWidget) {
				continue;
			}

			if (!selectedWidget->isEnabled()) {
				selectedWidget = nullptr;
				continue;
			}

			if (selectedWidget->focusPolicy() == Qt::NoFocus) {
				selectedWidget = nullptr;
				continue;
			}

			if (qobject_cast< QLabel * >(selectedWidget)) {
				selectedWidget = nullptr;
				continue;
			}

			break;
		}

		return selectedWidget;
	}

} // namespace Accessibility
} // namespace Mumble
