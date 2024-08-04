#pragma once

#include <QColor>
#include <QOpenGLShaderProgram>

namespace Library::GL {

void fillColorBuffer(const QColor& color = QColor(0, 0, 0, 0));

void compileProgram(QOpenGLShaderProgram& program, const QString& vertexShaderSrc, const QString& fragmentShaderSrc);

}