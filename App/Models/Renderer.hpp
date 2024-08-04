#pragma once

#include <QPolygonF>
#include <GL/Texture.hpp>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>

namespace Models::SurfaceDetails {

class Renderer
{
public:
    Renderer();

    void render(
        const Library::GL::Texture& tex,
        const QPolygonF& poly,
        const QTransform& matrix,
        GLenum minFilter,
        GLenum magFilter,
        GLenum wrapping
    );

private:
    QOpenGLShaderProgram _glProgram;
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
    int uMatrix;
};

}