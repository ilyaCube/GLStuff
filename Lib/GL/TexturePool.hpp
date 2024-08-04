#pragma once

#include <memory>
#include "Impls/TexturePool.hpp"

namespace Library::GL {

using TexturePool = std::shared_ptr<Impls::TexturePool>;

inline TexturePool createTexturePool(GLenum format, int width = 0, int height = 0)
{
    return std::make_shared<Impls::TexturePool>(format, width, height);
}

}