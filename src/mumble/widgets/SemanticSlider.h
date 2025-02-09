// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_SEMANTICSLIDER_H_
#define MUMBLE_MUMBLE_WIDGETS_SEMANTICSLIDER_H_

#include <QAccessible>
#include <QAccessibleValueInterface>
#include <QAccessibleWidget>

#include <QFlags>
#include <QObject>
#include <QPair>
#include <QString>
#include <QVariant>
#include <QVector>
#include <QWidget>

#include <QtWidgets/QAbstractSlider>
#include <QtWidgets/QSlider>

class SemanticSlider : public QSlider {
	Q_OBJECT

public:
	SemanticSlider(QWidget *parent = nullptr);

	QString m_semanticValue;
};

class AccessibleAbstractSlider : public QAccessibleWidget, QAccessibleValueInterface {
public:
	explicit AccessibleAbstractSlider(QWidget *w, QAccessible::Role r = QAccessible::Slider);
	void *interface_cast(QAccessible::InterfaceType t) override;
	QString text(QAccessible::Text t) const override;
	QString getSliderName() const;
	QVector< QPair< QAccessibleInterface *, QFlags< QAccessible::RelationFlag > > >
		relations(QAccessible::Relation match) const override;

	QVariant currentValue() const override;
	void setCurrentValue(const QVariant &value) override;
	QVariant maximumValue() const override;
	QVariant minimumValue() const override;
	QVariant minimumStepSize() const override;

protected:
	QAbstractSlider *abstractSlider() const;
};

class AccessibleSlider : public AccessibleAbstractSlider {
public:
	explicit AccessibleSlider(QWidget *w);
	QString text(QAccessible::Text t) const override;

	QVariant currentValue() const override;

	static QAccessibleInterface *semanticSliderFactory(const QString &classname, QObject *object);

protected:
	SemanticSlider *slider() const;
};

#endif
