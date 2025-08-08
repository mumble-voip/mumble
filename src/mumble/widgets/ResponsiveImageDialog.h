// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_WIDGETS_RESPONSIVEIMAGEDIALOG_H_
#define MUMBLE_MUMBLE_WIDGETS_RESPONSIVEIMAGEDIALOG_H_

#include <QtGui/QPixmap>
#include <QtWidgets/QDialog>
#include <QtWidgets/QLabel>
#include <QtWidgets/QScrollArea>
#include <QtWidgets/QVBoxLayout>

class ResponsiveImageDialog : public QDialog {
	Q_OBJECT

private:
	QLabel *m_label;
	QPixmap m_pixmap;

public:
	explicit ResponsiveImageDialog(const QPixmap &pixmap, QWidget *parent = nullptr);

protected:
	void resizeEvent(QResizeEvent *event) override;
};

#endif // MUMBLE_MUMBLE_WIDGETS_RESPONSIVEIMAGEDIALOG_H_