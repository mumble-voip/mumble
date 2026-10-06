// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoWidget.h"

#include <QtGui/QOpenGLContext>
#include <QtGui/QPainter>

namespace {

// Written for both desktop OpenGL 2 and OpenGL ES 2. QOpenGLShaderProgram makes the precision qualifiers work on
// desktop OpenGL.
const char *VERTEX_SHADER = R"(
attribute highp vec2 position;
attribute highp vec2 texCoordIn;
varying highp vec2 texCoord;
void main() {
	gl_Position = vec4(position, 0.0, 1.0);
	texCoord = texCoordIn;
}
)";

const char *FRAGMENT_SHADER = R"(
uniform sampler2D frame;
varying highp vec2 texCoord;
void main() {
	gl_FragColor = vec4(texture2D(frame, texCoord).rgb, 1.0);
}
)";

constexpr GLfloat POSITIONS[]  = { -1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
constexpr GLfloat TEX_COORDS[] = { 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f };

} // namespace

VideoWidget::VideoWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

VideoWidget::~VideoWidget() {
	makeCurrent();
	cleanup();
	doneCurrent();
}

void VideoWidget::setFrame(const QImage &frame) {
	// Uploaded as RGBA, which every OpenGL version takes
	m_frame        = frame.convertToFormat(QImage::Format_RGBA8888);
	m_frameChanged = true;
	if (isVisible())
		update();
}

void VideoWidget::setPlaceholderText(const QString &text) {
	m_placeholderText = text;
	update();
}

void VideoWidget::initializeGL() {
	initializeOpenGLFunctions();

	// The context is replaced e.g. when the widget moves to a different top-level window
	connect(context(), &QOpenGLContext::aboutToBeDestroyed, this, [this]() {
		makeCurrent();
		cleanup();
		doneCurrent();
	});

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex, VERTEX_SHADER);
	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment, FRAGMENT_SHADER);
	m_program->bindAttributeLocation("position", 0);
	m_program->bindAttributeLocation("texCoordIn", 1);
	m_program->link();

	glGenTextures(1, &m_texture);
	glBindTexture(GL_TEXTURE_2D, m_texture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	m_textureSize  = QSize();
	m_frameChanged = !m_frame.isNull();
}

void VideoWidget::cleanup() {
	if (m_texture) {
		glDeleteTextures(1, &m_texture);
		m_texture = 0;
	}
	delete m_program;
	m_program = nullptr;
}

void VideoWidget::uploadFrame() {
	glBindTexture(GL_TEXTURE_2D, m_texture);
	// Rows of 32 bit pixels are always aligned to 4 bytes
	glPixelStorei(GL_UNPACK_ALIGNMENT, 4);
	if (m_textureSize != m_frame.size()) {
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_frame.width(), m_frame.height(), 0, GL_RGBA, GL_UNSIGNED_BYTE,
					 m_frame.constBits());
		m_textureSize = m_frame.size();
	} else {
		glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_frame.width(), m_frame.height(), GL_RGBA, GL_UNSIGNED_BYTE,
						m_frame.constBits());
	}
	m_frameChanged = false;
}

void VideoWidget::paintGL() {
	glClearColor(0.f, 0.f, 0.f, 1.f);
	glClear(GL_COLOR_BUFFER_BIT);

	if (m_frame.isNull() || !m_program || !m_program->isLinked()) {
		if (!m_placeholderText.isEmpty()) {
			QPainter painter(this);
			painter.setPen(Qt::white);
			painter.drawText(rect(), Qt::AlignCenter, m_placeholderText);
		}
		return;
	}

	if (m_frameChanged)
		uploadFrame();

	// Letterbox the frame, keeping its aspect ratio
	const qreal ratio = devicePixelRatioF();
	const QSize area  = size() * ratio;
	const QSize fit   = m_frame.size().scaled(area, Qt::KeepAspectRatio);
	glViewport((area.width() - fit.width()) / 2, (area.height() - fit.height()) / 2, fit.width(), fit.height());

	m_program->bind();
	m_program->setUniformValue("frame", 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, m_texture);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, POSITIONS);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, TEX_COORDS);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	m_program->release();
}
