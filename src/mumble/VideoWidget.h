// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#ifndef MUMBLE_MUMBLE_VIDEOWIDGET_H_
#define MUMBLE_MUMBLE_VIDEOWIDGET_H_

#include <QtGui/QImage>
#include <QtGui/QOpenGLFunctions>
#include <QtOpenGL/QOpenGLShaderProgram>
#include <QtOpenGLWidgets/QOpenGLWidget>

/// Shows video frames, scaled to fit the widget while keeping their aspect ratio.
///
/// The frames are drawn with OpenGL, so scaling happens on the GPU instead of costing CPU time on the GUI thread
/// for every frame.
class VideoWidget : public QOpenGLWidget, protected QOpenGLFunctions {
private:
	Q_OBJECT
	Q_DISABLE_COPY(VideoWidget)

public:
	explicit VideoWidget(QWidget *parent = nullptr);
	~VideoWidget() override;

	/// Shows the given frame. It is kept while the widget is hidden and shown once it is visible again.
	void setFrame(const QImage &frame);
	/// Text to show until the first frame arrives
	void setPlaceholderText(const QString &text);

protected:
	void initializeGL() override;
	void paintGL() override;

private:
	/// Uploads m_frame to m_texture.
	void uploadFrame();
	/// Frees all OpenGL resources. The context has to be current.
	void cleanup();

	QImage m_frame;
	/// Set when m_frame has not been uploaded yet
	bool m_frameChanged = false;
	QString m_placeholderText;

	QOpenGLShaderProgram *m_program = nullptr;
	GLuint m_texture                = 0;
	/// Size of m_texture
	QSize m_textureSize;
};

#endif // MUMBLE_MUMBLE_VIDEOWIDGET_H_
