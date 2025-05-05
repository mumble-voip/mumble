// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "SemanticSlider.h"

#include <QLabel>

SemanticSlider::SemanticSlider(QWidget *parent) : QSlider(parent), m_semanticValue("") {
}

AccessibleAbstractSlider::AccessibleAbstractSlider(QWidget *w, QAccessible::Role r) : QAccessibleWidget(w, r) {
	Q_ASSERT(qobject_cast< QAbstractSlider * >(w));
}

void *AccessibleAbstractSlider::interface_cast(QAccessible::InterfaceType t) {
	if (t == QAccessible::ValueInterface) {
		return static_cast< QAccessibleValueInterface * >(this);
	}
	return QAccessibleWidget::interface_cast(t);
}

QAbstractSlider *AccessibleAbstractSlider::abstractSlider() const {
	return static_cast< QAbstractSlider * >(object());
}

QString AccessibleAbstractSlider::text(QAccessible::Text t) const {
	if (t == QAccessible::Name) {
		// Fallback to raw slider value
		QString sliderName = getSliderName();
		QString newName    = QString::number(abstractSlider()->value()) + ", " + sliderName;
		if (!sliderName.isEmpty()) {
			sliderName += ".";
		}
		return newName;
	}

	return QAccessibleWidget::text(t);
}

QVector< QPair< QAccessibleInterface *, QFlags< QAccessible::RelationFlag > > >
	AccessibleAbstractSlider::relations(QAccessible::Relation match) const {
	// We need to remove buddy labels as screen readers (Orca) might
	// read that instead of our newly created dynamic name
	QVector< QPair< QAccessibleInterface *, QFlags< QAccessible::RelationFlag > > > list =
		QAccessibleWidget::relations(match);

	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		QLabel *label = qobject_cast< QLabel * >(iter->first->object());
		if (label && label->buddy() == abstractSlider()) {
			list.erase(iter);
		}
	}

	return list;
}

QString AccessibleAbstractSlider::getSliderName() const {
	QString name = QAccessibleWidget::text(QAccessible::Name);

	if (!name.isEmpty()) {
		return name;
	}

	// Fallback to buddy label text
	QVector< QPair< QAccessibleInterface *, QFlags< QAccessible::RelationFlag > > > list =
		QAccessibleWidget::relations(QAccessible::Label);

	for (auto iter = list.begin(); iter != list.end(); ++iter) {
		QLabel *label = qobject_cast< QLabel * >(iter->first->object());
		if (label && label->buddy() == abstractSlider() && !label->text().isEmpty()) {
			return label->text();
		}
	}

	// There is no label/name for this slider whatsoever
	return QString();
}

QVariant AccessibleAbstractSlider::currentValue() const {
	return abstractSlider()->value();
}

void AccessibleAbstractSlider::setCurrentValue(const QVariant &value) {
	abstractSlider()->setValue(value.toInt());
}

QVariant AccessibleAbstractSlider::maximumValue() const {
	return abstractSlider()->maximum();
}

QVariant AccessibleAbstractSlider::minimumValue() const {
	return abstractSlider()->minimum();
}

QVariant AccessibleAbstractSlider::minimumStepSize() const {
	return abstractSlider()->singleStep();
}

AccessibleSlider::AccessibleSlider(QWidget *w) : AccessibleAbstractSlider(w) {
	Q_ASSERT(slider());
	addControllingSignal(QLatin1String("valueChanged(int)"));
}

SemanticSlider *AccessibleSlider::slider() const {
	return qobject_cast< SemanticSlider * >(object());
}

QString AccessibleSlider::text(QAccessible::Text t) const {
	if (t == QAccessible::Name) {
		QString semanticValue = slider()->m_semanticValue;
		if (!semanticValue.isEmpty()) {
			QString sliderName = getSliderName();
			QString newName    = slider()->m_semanticValue + ", " + sliderName;
			if (!sliderName.isEmpty()) {
				sliderName += ".";
			}
			return newName;
		}
	}

	return AccessibleAbstractSlider::text(t);
}

QAccessibleInterface *AccessibleSlider::semanticSliderFactory(const QString &classname, QObject *object) {
	// QT wants to automatically create AccessibleInterfaces for certain classes using
	// factories. This is the factory for the SemanticSlider which has to be registered
	// once in the MainWindow.
	// Returning nullptr signals to QT that this factory can not produce an interface for
	// the requested class.
	QAccessibleInterface *interface = nullptr;

	if (object && object->isWidgetType() && classname == QLatin1String("SemanticSlider")) {
		interface = new AccessibleSlider(static_cast< QWidget * >(object));
	}

	return interface;
}

QVariant AccessibleSlider::currentValue() const {
	QString semanticValue = slider()->m_semanticValue;
	if (!semanticValue.isEmpty()) {
		// This does NOT work in Orca. Therefore the whole dynamic name is implemented.
		// But it can be assumed that other screen readers might actually use the value
		// provided here as is, which would be nice and a better UX.
		return semanticValue;
	}

	return AccessibleAbstractSlider::currentValue();
}
