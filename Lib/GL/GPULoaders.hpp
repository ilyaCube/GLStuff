#pragma once

#include "Imaging/Image.hpp"
#include "Texture.hpp"
#include "TexturePool.hpp"

namespace Library::GL {

GL::Texture uploadImagePooled(const Imaging::Image& image, const GL::TexturePool& pool, GLenum dataFormat, GLenum dataType);

Imaging::Image downscaleImage(const Imaging::Image& image);

}