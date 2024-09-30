// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_TEXTTOSPEECH_H_
#define MUMBLE_MUMBLE_TEXTTOSPEECH_H_

#include <QtCore/QObject>

class TextToSpeechPrivate;

class TextToSpeech : public QObject {
	friend class TextToSpeechPrivate;

private:
	Q_OBJECT
	Q_DISABLE_COPY(TextToSpeech)
	Q_PROPERTY(bool enabled READ isEnabled WRITE setEnabled)
protected:
	bool enabled;

public:
	TextToSpeech(QObject *parent = nullptr);
	~TextToSpeech() Q_DECL_OVERRIDE;
	bool isEnabled() const;
public slots:
	void say(const QString &text);
	void setEnabled(bool ena);
	void setVolume(int volume);

private:
	TextToSpeechPrivate *d;
};

#endif
