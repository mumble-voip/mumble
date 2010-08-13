#ifndef VoiceRecorderDIALOG_H
#define VoiceRecorderDIALOG_H

#include "ui_VoiceRecorderDialog.h"

class VoiceRecorderDialog : public QDialog, private Ui::VoiceRecorderDialog {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(VoiceRecorderDialog)

		QTimer *qtTimer;
	public:
		explicit VoiceRecorderDialog(QWidget *p);
		~VoiceRecorderDialog();

		void closeEvent(QCloseEvent *evt);
	public slots:
		void on_qpbStart_clicked();
		void on_qpbStop_clicked();
		void on_qtTimer_timeout();
		void on_qpbTargetDirectoryBrowse_clicked();
		void reset();
};

#endif // VoiceRecorderDIALOG_H
