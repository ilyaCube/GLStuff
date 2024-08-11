#include "TextureRenderer.hpp"

#include <QTransform>
#include "GL/Helpers.hpp"
#include "GL/Types.hpp"

namespace Models::SurfaceDetails {

namespace {

const char *_vertexShaderSrc = R"__EOF__(
#version 130

uniform mat4 uModel;
uniform mat4 uView;
uniform mat4 uProj;
in vec3 aPosition;
in vec2 aTexCoords;

out vec2 texCoords;

void main()
{
    gl_Position = uProj * uView * uModel * vec4(aPosition, 1.0);
    texCoords = aTexCoords;
}
)__EOF__";

const char *_fragmentShaderSrc = R"__EOF__(
#version 130

uniform sampler2D uTex;
in vec2 texCoords;

out vec4 oColor;

void main()
{
    vec3 rgb = texture(uTex, texCoords).rgb;
    oColor = vec4(rgb, 255.0);
}
)__EOF__";

// QVector<QVector3D> vertices{
//     QVector3D(1.0f, 1.0f, 0.0f),
//     QVector3D(1.0f, -1.0f, 0.0f),
//     QVector3D(-1.0f, -1.0f, 0.0f),
//     QVector3D(-1.0f, 1.0f, 0.0f),
// };
// QVector<QPointF> textureCoords{
//     QPointF(1.0f, 1.0f),
//     QPointF(1.0f, 0.0f),
//     QPointF(0.0f, 0.0f),
//     QPointF(0.0f, 1.0f)
// };

// QVector<GLuint> eboData{
//     // front
//     0, 2, 6,
//     2, 4, 6,
//     // // right
//     0, 2, 3,
//     0, 1, 3,
//     // left
//     5, 7, 6,
//     5, 4, 6,
// };

}

TextureRenderer::TextureRenderer(int texSlot) :
    _vbo(QOpenGLBuffer::Type::VertexBuffer),
    _ebo(QOpenGLBuffer::Type::IndexBuffer),
    _texSlot(texSlot)
{
    auto f = Library::GL::functions();
    Library::GL::compileProgram(_glProgram, _vertexShaderSrc, _fragmentShaderSrc);

    _glProgram.bind();
    _glProgram.setUniformValue(_glProgram.uniformLocation("uTex"), _texSlot);
    _uModel  = _glProgram.uniformLocation("uModel");
    _uProj   = _glProgram.uniformLocation("uProj");
    _uView   = _glProgram.uniformLocation("uView");
    _glProgram.release();

    // _ebo.create();
    // _ebo.bind();
    // _ebo.allocate(eboData.data(), eboData.size() * sizeof(GLuint));
    // _ebo.release();

    _vbo.create();
    _vbo.bind();

    _vao.create();
    _vao.bind();
    addAttribute<GLfloat>("aPosition", 3);
    addAttribute<GLfloat>("aTexCoords", 2);
    uploadAttributes();
    _vao.release();

    _vbo.release();

    GL_CHECK();
}

void TextureRenderer::render(
    const Library::GL::Texture& texture,
    const QVector<GLfloat>& vertices,
    const QMatrix4x4& model,
    const QMatrix4x4& view,
    const QMatrix4x4& proj,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapping
)
{
    auto f = Library::GL::functions();

    float texW = texture->width;
    float texH = texture->height;

    _glProgram.bind();
    _glProgram.setUniformValue(_uModel, model);
    _glProgram.setUniformValue(_uView, view);
    _glProgram.setUniformValue(_uProj, proj);

    texture->bind(_texSlot);
    texture->setMinMagFilter(minFilter, magFilter);
    texture->setWrapping(wrapping);

    Library::GL::functions()->glEnable(GL_DEPTH_TEST);

    _vbo.bind();
    _vbo.allocate(vertices.data(), vertices.size() * sizeof(GLfloat));

    QOpenGLVertexArrayObject::Binder bind(&_vao);
    // _ebo.bind();

    // tmp
    f->glDrawArrays(GL_TRIANGLES, 0, 36);
    GL_CHECK();

    // _ebo.release();
    _vbo.release();
    texture->release(_texSlot);
    _glProgram.release();
}

void TextureRenderer::uploadAttributes()
{
    auto f = Library::GL::functions();
    for(const auto& attribute : attributes) {
        _glProgram.enableAttributeArray(attribute.id);
        f->glVertexAttribPointer(
            attribute.id,
            attribute.count,
            attribute.glType,
            Library::GL::glBool(attribute.isNormalize),
            currentAttribSize,
            reinterpret_cast<const GLvoid*>(attribute.offset)
        );
    }
}

}