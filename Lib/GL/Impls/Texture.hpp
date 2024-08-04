#pragma once

#include <QObject>
#include <QRect>
#include "Common/NonCopyable.hpp"
#include "GL/GL.hpp"

namespace Library::GL::Impls {

// QObject to capture thread affinity (context preserving destructor)
class Texture : private Common::NonCopyable, private QObject
{
    static constexpr int TEMP_SLOT{15};

public:
    const GLuint id;
    const int width, height;
    const GLenum format;

    Texture(int width, int height, GLenum format);

    ~Texture() override;

    static GLuint generateTexture();

    inline void bind(int slot) const
    {
        _bindCount++;

        auto f = GL::functions();
        f->glActiveTexture(GL_TEXTURE0 + slot);
        f->glBindTexture(GL_TEXTURE_2D, id);
        GL_CHECK();
    }

    inline void bindTemp() const
    {
        bind(TEMP_SLOT);
    }

    inline void release(int slot) const
    {
        Q_ASSERT(_bindCount.fetch_sub(1) > 0);

        auto f = GL::functions();
        f->glActiveTexture(GL_TEXTURE0 + slot);
        f->glBindTexture(GL_TEXTURE_2D, 0);
        GL_CHECK();
    }

    inline void releaseTemp() const
    {
        release(TEMP_SLOT);
    }

    // Automatic bindTemp/releaseTemp
    inline void allocateABR()
    {
        setDataABR(GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    }

    inline void resetMipmaps()
    {
        _mipmapLevel = 0;
    }

    void setMinMagFilter(GLenum minFilter, GLenum magFilter) const;

    void setWrapping(GLenum wrapping) const;

    // Automatic bindTemp/releaseTemp
    void setDataABR(GLenum dataFormat, GLenum dataType, const void *data, int skipPixels = 0, int rowLength = 0);

    // Automatic bindTemp/releaseTemp
    void getDataABR(GLenum dataFormat, GLenum dataType, void *data) const;

    // Automatic bindTemp/releaseTemp
    // 1000 means default max mipmap level
    void assureMipmapABR(unsigned int mipmapLevel = 1000) const;

private:
    mutable std::atomic_int _bindCount{0};
    mutable unsigned int _mipmapLevel{0U};
};

}