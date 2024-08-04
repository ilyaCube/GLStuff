#include "TexturePool.hpp"

#include "Texture.hpp"

namespace Library::GL::Impls {

TexturePool::TexturePool(GLenum format, int width, int height) :
    format(format), _width(width), _height(height),
    _releaseTexture([this](Impls::Texture *texPtr) {
        std::lock_guard<std::recursive_mutex> lock(_recursiveMutex);

        if (texPtr->width == _width && texPtr->height == _height && texPtr->format == this->format) {
            texPtr->resetMipmaps();
            _pool.emplace_back(texPtr);
        } else {
            delete texPtr;
        }
    })
{}

TexturePool::~TexturePool()
{
    std::lock_guard<std::recursive_mutex> lock(_recursiveMutex);
    clear();
}

void TexturePool::matchDimensions(int width, int height)
{
    std::lock_guard<std::recursive_mutex> lock(_recursiveMutex);

    if (width == _width && height == _height) {
        return;
    }

    clear();

    _width = width;
    _height = height;
}

void TexturePool::matchDimensions(const GL::Texture& tex)
{
    if(tex != nullptr) {
        matchDimensions(tex->width, tex->height);
    } else {
        matchDimensions(0, 0);
    }
}

GL::Texture TexturePool::allocate()
{
    std::lock_guard<std::recursive_mutex> lock(_recursiveMutex);

    if (_width == 0 || _height == 0) {
        return nullptr;
    }

    Impls::Texture *texPtr;
    if (!_pool.empty()) {
        texPtr = _pool.back();
        _pool.pop_back();
    } else {
        texPtr = new Impls::Texture(_width, _height, format);
        texPtr->allocateABR();
    }

    // Release texture or delete it. Safe to use even if texture pool is already destroyed.
    return std::shared_ptr<Impls::Texture>(texPtr, [weak = _releaseTexture.weakPtr()](auto *texPtr) {
        if (auto release = weak.lock()) {
            (*release)(texPtr);
        } else {
            delete texPtr;
        }
    });
}

void TexturePool::clear()
{
    std::lock_guard<std::recursive_mutex> lock(_recursiveMutex);

    while (!_pool.empty()) {
        auto texPtr = _pool.back();
        _pool.pop_back();

        delete texPtr;
    }
}

}