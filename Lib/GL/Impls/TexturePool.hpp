#pragma once

#include <mutex>
#include "Common/SafeContainer.hpp"
#include "Common/NonCopyable.hpp"
#include "GL/Texture.hpp"

namespace Library::GL::Impls {

class TexturePool : private Common::NonCopyable
{
public:
    const GLenum format;

    explicit TexturePool(GLenum format, int width = 0, int height = 0);
    ~TexturePool();

    void matchDimensions(int width, int height);
    void matchDimensions(const GL::Texture& tex);

    GL::Texture allocate();
    void clear();

private:
    std::recursive_mutex _recursiveMutex;
    std::vector<Impls::Texture *> _pool;
    int _width{0};
    int _height{0};

    // To be deleted first
    Common::SafeContainer<std::function<void(Impls::Texture *)>> _releaseTexture;
};

}
