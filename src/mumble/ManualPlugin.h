// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_MANUALPLUGIN_H_
#define MUMBLE_MUMBLE_MANUALPLUGIN_H_

#include <QtCore/QtGlobal>
#include <QtWidgets/QDialog>
#include <QtWidgets/QGraphicsItem>
#include <QtWidgets/QGraphicsScene>

#include "LegacyPlugin.h"
#include "ui_ManualPlugin.h"

#include <atomic>
#include <chrono>

struct Position2D {
	float x;
	float y;
};

// We need this typedef in order to be able to pass this hash as an argument
// to QMetaObject::invokeMethod
using PositionMap = QHash< unsigned int, Position2D >;
Q_DECLARE_METATYPE(PositionMap)


/// A struct holding information about a stale entry in the
/// manual plugin's position window
struct StaleEntry {
	/// The time point since when this entry is considered stale
	std::chrono::time_point< std::chrono::steady_clock > staleSince;
	/// The pointer to the stale item
	QGraphicsItem *staleItem;
};

class Manual : public QDialog, public Ui::Manual {
	Q_OBJECT
public:
	Manual(QWidget *parent = 0);

	static void setSpeakerPositions(const QHash< unsigned int, Position2D > &positions);


public slots:
	void on_qpbUnhinge_pressed();
	void on_qpbLinked_clicked(bool);
	void on_qpbActivated_clicked(bool);
	void on_qdsbX_valueChanged(double);
	void on_qdsbY_valueChanged(double);
	void on_qdsbZ_valueChanged(double);
	void on_qsbAzimuth_valueChanged(int);
	void on_qsbElevation_valueChanged(int);
	void on_qdAzimuth_valueChanged(int);
	void on_qdElevation_valueChanged(int);
	void on_qleContext_editingFinished();
	void on_qleIdentity_editingFinished();
	void on_buttonBox_clicked(QAbstractButton *);
	void on_qsbSilentUserDisplaytime_valueChanged(int);

	void on_speakerPositionUpdate(PositionMap positions);

	void on_updateStaleSpeakers();

protected:
	QGraphicsScene *m_qgsScene;
	QGraphicsItem *m_qgiPosition;

	std::atomic< bool > updateLoopRunning;

	QHash< unsigned int, QGraphicsItem * > speakerPositions;
	QHash< unsigned int, StaleEntry > staleSpeakerPositions;

	bool eventFilter(QObject *, QEvent *);
	void changeEvent(QEvent *e);
	void updateTopAndFront(int orientation, int azimut);
};

MumblePlugin *ManualPlugin_getMumblePlugin();
MumblePluginQt *ManualPlugin_getMumblePluginQt();


/// A built-in "plugin" for positional data gathering allowing for manually placing the "players" in a UI
class ManualPlugin : public LegacyPlugin {
	friend class Plugin; // needed in order for Plugin::createNew to access LegacyPlugin::doInitialize()
private:
	Q_OBJECT
	Q_DISABLE_COPY(ManualPlugin)

protected:
	virtual void resolveFunctionPointers() Q_DECL_OVERRIDE;
	ManualPlugin(QObject *p = nullptr);

public:
	virtual ~ManualPlugin() Q_DECL_OVERRIDE;
};

#endif
