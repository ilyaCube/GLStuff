#include "GL/Impls/Texture.hpp"

namespace Library::GL::Impls {

GLuint Texture::generateTexture()
{
    auto f = GL::functions();
    GLuint id = 0;

    f->glGenTextures(1, &id);
    GL_CHECK();

    return id;
}

Texture::Texture(int width, int height, GLenum format) :
    id(generateTexture()),
    width(width),
    height(height),
    format(format)
{
    Q_ASSERT(width > 0);
    Q_ASSERT(height > 0);
}

Texture::~Texture()
{
    Q_ASSERT(_bindCount == 0);

    QMetaObject::invokeMethod(this, [id = id]() {
        auto f = GL::functions();
        f->glDeleteTextures(1, &id);
        if (auto status = f->glGetError()) {
            qCritical("OpenGL error: %i in %s:%i", status, __FILE__, __LINE__);
        }
    });
}

void Texture::setMinMagFilter(GLenum minFilter, GLenum magFilter) const
{
    Q_ASSERT(_bindCount > 0);

    auto f = GL::functions();
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, minFilter);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, magFilter);
    GL_CHECK();
}

void Texture::setWrapping(GLenum wrapping) const
{
    Q_ASSERT(_bindCount > 0);

    auto f = GL::functions();
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
    GL_CHECK();
}

void Texture::setDataABR(GLenum dataFormat, GLenum dataType, const void *data, int skipPixels, int rowLength)
{
    resetMipmaps();

    bindTemp();
    auto f = GL::functions();

    if (skipPixels) {
        f->glPixelStorei(GL_UNPACK_SKIP_PIXELS, skipPixels);
    }
    if (rowLength) {
        f->glPixelStorei(GL_UNPACK_ROW_LENGTH, rowLength);
    }

    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    f->glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, dataFormat, dataType, data);

    if (skipPixels) {
        f->glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    }
    if (rowLength) {
        f->glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    }

    // Set back default value (otherwise Qt font rendering may become corrupted)
    f->glPixelStorei(GL_UNPACK_ALIGNMENT, 4);

    GL_CHECK();
    releaseTemp();
}

void Texture::getDataABR(GLenum dataFormat, GLenum dataType, void *data) const
{
    bindTemp();
    auto f = GL::functions();
    f->glPixelStorei(GL_PACK_ALIGNMENT, 1);
    f->glGetTexImage(GL_TEXTURE_2D, 0, dataFormat, dataType, data);

    GL_CHECK();
    releaseTemp();
}

void Texture::assureMipmapABR(unsigned int mipmapLevel) const
{
    if (mipmapLevel <= _mipmapLevel) {
        return;
    }
    _mipmapLevel = mipmapLevel;

    bindTemp();
    auto f = GL::functions();
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, mipmapLevel);
    f->glGenerateMipmap(GL_TEXTURE_2D);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    f->glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    GL_CHECK();
    releaseTemp();
}

}
