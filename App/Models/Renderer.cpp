#include "Renderer.hpp"

#include <QTransform>
#include "GL/Helpers.hpp"

namespace Models::SurfaceDetails {

namespace {

const char *_vertexShaderSrc = R"__EOF__(
#version 130

uniform mat3 uMatrix;
in vec2 aPosition;
in vec2 aTexCoords;
out vec2 texCoords;

void main()
{
    gl_Position = vec4(uMatrix * vec3(aPosition, 1.0), 1.0);
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

}

Renderer::Renderer()
{
    auto f = Library::GL::functions();
    Library::GL::compileProgram(_glProgram, _vertexShaderSrc, _fragmentShaderSrc);
    _glProgram.bind();
    _glProgram.setUniformValue(_glProgram.uniformLocation("uTex"), 0);
    uMatrix = _glProgram.uniformLocation("uMatrix");
    _glProgram.release();

    _vbo.create();
    _vbo.bind();

    _vao.create();
    _vao.bind();
    auto aPosition = _glProgram.attributeLocation("aPosition");
    _glProgram.enableAttributeArray(aPosition);
    f->glVertexAttribPointer(aPosition, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (const void *) 0);
    auto aTexCoords = _glProgram.attributeLocation("aTexCoords");
    _glProgram.enableAttributeArray(aTexCoords);
    f->glVertexAttribPointer(aTexCoords, 2, GL_FLOAT, GL_FALSE, 2 * 2 * sizeof(float), (const void *) (2 * sizeof(float)));

    GL_CHECK();
    _vao.release();
    _vbo.release();
}

void Renderer::render(
    const Library::GL::Texture& texture,
    const QPolygonF& poly,
    const QTransform& matrix,
    GLenum minFilter,
    GLenum magFilter,
    GLenum wrapping
)
{
    auto f = Library::GL::functions();
    float texW = texture->width;
    float texH = texture->height;
    auto N = poly.length();
    QVector<float> vertexAttributes(N * 4);
    auto j = 0;
    for (int i = 0; i < N; i++) {
        auto p = poly.at(i);
        vertexAttributes[j++] = p.x();
        vertexAttributes[j++] = p.y();
        vertexAttributes[j++] = p.x() / texW;
        vertexAttributes[j++] = p.y() / texH;
    }

    _glProgram.bind();
    _glProgram.setUniformValue(uMatrix, matrix);

    texture->bind(0);
    texture->setMinMagFilter(minFilter, magFilter);
    texture->setWrapping(wrapping);

    _vbo.bind();

    _vbo.allocate(vertexAttributes.data(), vertexAttributes.size() * sizeof(float));
    _vao.bind();

    f->glDrawArrays(GL_POLYGON, 0, N);

    GL_CHECK();

    _vao.release();
    _vbo.release();
    _glProgram.release();

    texture->release(0);
}

}