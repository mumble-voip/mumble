#ifndef MANUAL_H
#define MANUAL_H

#include <QDialog>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include "ui_manual.h"

class Manual : public QDialog, public Ui::Manual {
		Q_OBJECT
	public:
		Manual(QWidget *parent = 0);

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
	protected:
		QGraphicsScene *qgsScene;
		QGraphicsItem *qgiPosition;

		bool eventFilter(QObject *, QEvent *);
		void changeEvent(QEvent *e);
		void updateTopAndFront(int orientation, int azimut);
};

#endif // MANUAL_H
