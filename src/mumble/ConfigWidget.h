// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_CONFIGWIDGET_H_
#define MUMBLE_MUMBLE_CONFIGWIDGET_H_

#include <QtCore/QObject>
#include <QtCore/QtGlobal>
#include <QtWidgets/QWidget>

struct Settings;
class ConfigDialog;
class QSlider;
class QAbstractButton;
class QComboBox;

class ConfigWidget : public QWidget {
private:
	Q_OBJECT
	Q_DISABLE_COPY(ConfigWidget)
protected:
	void loadSlider(QSlider *, int);
	void loadCheckBox(QAbstractButton *, bool);
	void loadComboBox(QComboBox *, int);
signals:
	void intSignal(int);

public:
	Settings &s;
	ConfigWidget(Settings &st);
	virtual QString title() const          = 0;
	virtual const QString &getName() const = 0;
	virtual QIcon icon() const;
public slots:
	virtual void accept() const;
	virtual void save() const            = 0;
	virtual void load(const Settings &r) = 0;
};

typedef ConfigWidget *(*ConfigWidgetNew)(Settings &st);

class ConfigRegistrar Q_DECL_FINAL {
	friend class ConfigDialog;
	friend class ConfigDialogMac;

private:
	Q_DISABLE_COPY(ConfigRegistrar)
protected:
	int iPriority;
	static QMap< int, ConfigWidgetNew > *c_qmNew;

public:
	ConfigRegistrar(int priority, ConfigWidgetNew n);
	~ConfigRegistrar();
};

#endif
