// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TOKENS_H_
#define MUMBLE_MUMBLE_TOKENS_H_

#include "ui_Tokens.h"

class Tokens : public QDialog, public Ui::Tokens {
private:
	Q_OBJECT
	Q_DISABLE_COPY(Tokens)
protected:
	QByteArray qbaDigest;

public:
	Tokens(QWidget *p = nullptr);
public slots:
	void accept() Q_DECL_OVERRIDE;
public slots:
	void on_qpbAdd_clicked();
	void on_qpbRemove_clicked();
};

#endif
