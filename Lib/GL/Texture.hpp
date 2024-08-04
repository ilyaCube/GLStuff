#pragma once

#include <memory>
#include "Imaging/Image.hpp"
#include "Impls/Texture.hpp"

namespace Library::GL {

using Texture = std::shared_ptr<Impls::Texture>;

inline Texture createTexture(int width, int height, GLenum format)
{
    return std::make_shared<Impls::Texture>(width, height, format);
}

}