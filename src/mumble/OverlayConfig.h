/* Copyright (C) 2005-2011, Thorvald Natvig <thorvald@natvig.com>

   All rights reserved.

   Redistribution and use in source and binary forms, with or without
   modification, are permitted provided that the following conditions
   are met:

   - Redistributions of source code must retain the above copyright notice,
     this list of conditions and the following disclaimer.
   - Redistributions in binary form must reproduce the above copyright notice,
     this list of conditions and the following disclaimer in the documentation
     and/or other materials provided with the distribution.
   - Neither the name of the Mumble Developers nor the names of its
     contributors may be used to endorse or promote products derived from this
     software without specific prior written permission.

   THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
   ``AS IS'' AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
   LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
   A PARTICULAR PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL THE FOUNDATION OR
   CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
   EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
   PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
   PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF
   LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING
   NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
   SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#ifndef MUMBLE_MUMBLE_OVERLAYCONFIG_H_
#define MUMBLE_MUMBLE_OVERLAYCONFIG_H_

#include "OverlayText.h"
#include "ConfigWidget.h"

#include "ui_Overlay.h"

class OverlayUserGroup;
struct OverlayAppInfo;

class OverlayConfig : public ConfigWidget, public Ui::OverlayConfig {
	private:
		Q_OBJECT
		Q_DISABLE_COPY(OverlayConfig)

		void initDisplay();
		void refreshFpsDemo();
		void refreshFpsLive();
		void refreshTimeLive();
	protected:
		QPixmap qpScreen;
		QGraphicsPixmapItem *qgpiScreen;
		QGraphicsScene qgs;
		QGraphicsScene qgsFpsPreview;
		BasepointPixmap bpFpsDemo;
		BasepointPixmap bpTimeDemo;
		QGraphicsPixmapItem *qgpiFpsDemo;
		QGraphicsPixmapItem *qgpiFpsLive;
		QGraphicsPixmapItem *qgpiTimeLive;
		OverlayUserGroup *oug;
		QGraphicsTextItem *qgtiInstructions;

		float fViewScale;

		bool eventFilter(QObject *, QEvent *) Q_DECL_OVERRIDE;

		bool supportsInstallableOverlay();
		bool isInstalled();
		bool needsUpgrade();
		bool installFiles();
		bool uninstallFiles();
		bool supportsCertificates();
		bool installerIsValid();
		void showCertificates();

		QString applicationIdentifierForPath(const QString &path);
		OverlayAppInfo applicationInfoForId(const QString &identifier);
	protected slots:
		void on_qpbInstall_clicked();
		void on_qpbUninstall_clicked();
		void on_qpbAdd_clicked();
		void on_qpbRemove_clicked();
		void on_qrbBlacklist_toggled(bool);
		void on_qcbEnable_stateChanged(int);
		void on_qcbShowFps_stateChanged(int);
		void on_qcbShowTime_stateChanged(int);
		void on_qpbFpsFont_clicked();
		void on_qpbFpsColor_clicked();
		void on_qpbLoadPreset_clicked();
		void on_qpbSavePreset_clicked();
		void resizeScene(bool force=false);
	public:
		OverlayConfig(Settings &st);
		virtual QString title() const Q_DECL_OVERRIDE;
		virtual QIcon icon() const Q_DECL_OVERRIDE;
	public slots:
		void accept() const Q_DECL_OVERRIDE;
		void save() const Q_DECL_OVERRIDE;
		void load(const Settings &r) Q_DECL_OVERRIDE;
		bool expert(bool) Q_DECL_OVERRIDE;
};

#endif
