#pragma once

#include <QPolygonF>
#include <QOpenGLShaderProgram>
#include <QOpenGLVertexArrayObject>
#include <QOpenGLBuffer>
#include <qvector.h>

#include "GL/Texture.hpp"
#include "GL/Types.hpp"

namespace Models::SurfaceDetails {

class TextureRenderer
{
public:
    static constexpr int COUNT_VERTEXES = 4;


    struct Attribute
    {
        int id;
        int count;
        int glType;
        int offset;
        bool isNormalize;
    };

    TextureRenderer(int texSlot);

    void render(
        const Library::GL::Texture& tex,
        const QMatrix4x4& model,
        const QMatrix4x4& view,
        const QMatrix4x4& proj,
        GLenum minFilter,
        GLenum magFilter,
        GLenum wrapping
    );

private:
    int _texSlot;
    QOpenGLShaderProgram _glProgram;
    QOpenGLVertexArrayObject _vao;
    QOpenGLBuffer _vbo;
    QOpenGLBuffer _ebo;
    int _uModel;
    int _uProj;
    int _uView;

    void uploadAttributes();

    void allocVertexBuffer(const QVector<GLfloat>& vertices, const QVector<GLfloat>& texCoords) const;

    template <typename T>
    void addAttribute(const char* attribName, int size, bool isNormalize = false)
    {
        Attribute attribute;
        attribute.id = _glProgram.attributeLocation(attribName);
        attribute.count = size;
        attribute.glType = Library::GL::GLType<T>::value;
        attribute.offset = currentAttribSize;
        attribute.isNormalize = isNormalize;
        currentAttribSize += attribute.count * sizeof(T);
        attributes.push_back(attribute);
    }
    QVector<Attribute> attributes;
    int currentAttribSize = 0;
};

}