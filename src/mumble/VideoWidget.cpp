// Copyright The Mumble Developers. All rights reserved.
// Use of this source code is governed by a BSD-style license
// that can be found in the LICENSE file at the root of the
// Mumble source tree or at <https://www.mumble.info/LICENSE>.

#include "VideoWidget.h"

#include <QtGui/QGenericMatrix>
#include <QtGui/QOpenGLContext>
#include <QtGui/QPainter>
#include <QtGui/QVector3D>

#include <algorithm>

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

// The planes are uploaded including the padding at the end of their rows, which is cut off by scaling the
// horizontal texture coordinate. Clamping it keeps the padding from bleeding into the last column.
const char *FRAGMENT_SHADER = R"(
uniform sampler2D planeY;
uniform sampler2D planeU;
uniform sampler2D planeV;
uniform highp vec2 scaleY;
uniform highp vec2 scaleU;
uniform highp vec2 scaleV;
uniform highp mat3 yuvToRgb;
uniform highp vec3 yuvOffset;
varying highp vec2 texCoord;
void main() {
	highp vec3 yuv;
	yuv.x = texture2D(planeY, vec2(min(texCoord.x * scaleY.x, scaleY.y), texCoord.y)).r;
	yuv.y = texture2D(planeU, vec2(min(texCoord.x * scaleU.x, scaleU.y), texCoord.y)).r;
	yuv.z = texture2D(planeV, vec2(min(texCoord.x * scaleV.x, scaleV.y), texCoord.y)).r;
	gl_FragColor = vec4(clamp(yuvToRgb * (yuv - yuvOffset), 0.0, 1.0), 1.0);
}
)";

constexpr GLfloat POSITIONS[]  = { -1.f, -1.f, 1.f, -1.f, -1.f, 1.f, 1.f, 1.f };
constexpr GLfloat TEX_COORDS[] = { 0.f, 1.f, 1.f, 1.f, 0.f, 0.f, 1.f, 0.f };

/// The matrix that turns Y, U and V (each 0..1, minus yuvOffset()) into R, G and B.
QMatrix3x3 yuvToRgb(VideoFrame::ColorSpace colorSpace, bool fullRange) {
	// Luma coefficients of red and blue
	const float kr = colorSpace == VideoFrame::ColorSpace::BT709 ? 0.2126f : 0.299f;
	const float kb = colorSpace == VideoFrame::ColorSpace::BT709 ? 0.0722f : 0.114f;
	const float kg = 1.f - kr - kb;

	// Limited range only uses 16..235 for Y and 16..240 for U and V
	const float yScale  = fullRange ? 1.f : 255.f / 219.f;
	const float uvScale = fullRange ? 1.f : 255.f / 224.f;

	const float values[] = {
		yScale,
		0.f,
		2.f * (1.f - kr) * uvScale,
		yScale,
		-2.f * (1.f - kb) * kb / kg * uvScale,
		-2.f * (1.f - kr) * kr / kg * uvScale,
		yScale,
		2.f * (1.f - kb) * uvScale,
		0.f,
	};
	return QMatrix3x3(values);
}

QVector3D yuvOffset(bool fullRange) {
	return QVector3D(fullRange ? 0.f : 16.f / 255.f, 128.f / 255.f, 128.f / 255.f);
}

} // namespace

VideoWidget::VideoWidget(QWidget *parent) : QOpenGLWidget(parent) {
}

VideoWidget::~VideoWidget() {
	makeCurrent();
	cleanup();
	doneCurrent();
}

void VideoWidget::setFrame(const VideoFrame &frame) {
	m_frame        = frame;
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

	glGenTextures(3, m_textures);
	for (GLuint texture : m_textures) {
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	}
	std::fill(std::begin(m_textureSizes), std::end(m_textureSizes), QSize());
	m_frameChanged = !m_frame.isNull();
}

void VideoWidget::cleanup() {
	if (m_textures[0]) {
		glDeleteTextures(3, m_textures);
		std::fill(std::begin(m_textures), std::end(m_textures), 0);
	}
	delete m_program;
	m_program = nullptr;
}

void VideoWidget::uploadFrame() {
	// Single channel textures, which OpenGL ES 2 and desktop OpenGL 2 only have as luminance. Rows are uploaded
	// including their padding, as OpenGL ES 2 can't skip it.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	for (int i = 0; i < 3; ++i) {
		const QSize size(m_frame.strides[i], i == 0 ? m_frame.height : (m_frame.height + 1) / 2);
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
		if (m_textureSizes[i] != size) {
			glTexImage2D(GL_TEXTURE_2D, 0, GL_LUMINANCE, size.width(), size.height(), 0, GL_LUMINANCE, GL_UNSIGNED_BYTE,
						 m_frame.planes[i]);
			m_textureSizes[i] = size;
		} else {
			glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, size.width(), size.height(), GL_LUMINANCE, GL_UNSIGNED_BYTE,
							m_frame.planes[i]);
		}
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
	const QSize fit   = QSize(m_frame.width, m_frame.height).scaled(area, Qt::KeepAspectRatio);
	glViewport((area.width() - fit.width()) / 2, (area.height() - fit.height()) / 2, fit.width(), fit.height());

	// Horizontal texture coordinate scale that cuts off the padding, and the largest coordinate that doesn't
	// sample it (the center of the last column)
	const float widthY  = static_cast< float >(m_frame.width);
	const float widthUV = static_cast< float >((m_frame.width + 1) / 2);
	const float strideY = static_cast< float >(m_frame.strides[0]);
	const float strideU = static_cast< float >(m_frame.strides[1]);
	const float strideV = static_cast< float >(m_frame.strides[2]);

	m_program->bind();
	m_program->setUniformValue("planeY", 0);
	m_program->setUniformValue("planeU", 1);
	m_program->setUniformValue("planeV", 2);
	m_program->setUniformValue("scaleY", widthY / strideY, (widthY - 0.5f) / strideY);
	m_program->setUniformValue("scaleU", widthUV / strideU, (widthUV - 0.5f) / strideU);
	m_program->setUniformValue("scaleV", widthUV / strideV, (widthUV - 0.5f) / strideV);
	m_program->setUniformValue("yuvToRgb", yuvToRgb(m_frame.colorSpace, m_frame.fullRange));
	m_program->setUniformValue("yuvOffset", yuvOffset(m_frame.fullRange));
	for (int i = 0; i < 3; ++i) {
		glActiveTexture(GL_TEXTURE0 + static_cast< GLenum >(i));
		glBindTexture(GL_TEXTURE_2D, m_textures[i]);
	}

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, POSITIONS);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, TEX_COORDS);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glActiveTexture(GL_TEXTURE0);
	m_program->release();
}
