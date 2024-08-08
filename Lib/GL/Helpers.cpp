#include "Helpers.hpp"

#include "GL.hpp"

namespace Library::GL {

void clearDepthBuffer()
{
    auto f = GL::functions();
    f->glClear(GL_DEPTH_BUFFER_BIT);
}

void fillColorBuffer(const QColor& color)
{
    auto f = GL::functions();
    f->glClearColor(color.redF(), color.greenF(), color.blueF(), color.alphaF());
    f->glClear(GL_COLOR_BUFFER_BIT);

    GL_CHECK();
}

void compileProgram(QOpenGLShaderProgram& program, const QString& vertexShaderSrc, const QString& fragmentShaderSrc)
{
    program.removeAllShaders();
    program.create();

    if (!program.addCacheableShaderFromSourceCode(QOpenGLShader::Vertex, vertexShaderSrc)) {
        qFatal("Error compiling OpenGL vertex shader: %s", qUtf8Printable(program.log()));
        return;
    }
    if (!program.addCacheableShaderFromSourceCode(QOpenGLShader::Fragment, fragmentShaderSrc)) {
        qFatal("Error compiling OpenGL fragment shader: %s", qUtf8Printable(program.log()));
        return;
    }
    if (!program.link()) {
        qFatal("Error linking OpenGL program: %s", qUtf8Printable(program.log()));
        return;
    }
}

}