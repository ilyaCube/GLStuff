#include "GPULoaders.hpp"

namespace Library::GL {

GL::Texture uploadImagePooled(const Imaging::Image& image, const GL::TexturePool& pool, GLenum dataFormat, GLenum dataType)
{
    if (image.isEmpty()) {
        return nullptr;
    }

    auto downscaled = downscaleImage(image);
    auto mat = downscaled.mat();
    auto w = mat.cols;
    auto h = mat.rows;

    pool->matchDimensions(w, h);
    auto tex = pool->allocate();
    tex->setDataABR(dataFormat, dataType, mat.data);
    return tex;
}

Imaging::Image downscaleImage(const Imaging::Image& image)
{
    // If image is too large we need to downscale it prior upload to GPU
    auto size = image.size();
    auto w = size.width();
    auto h = size.height();
    int maxTextureSize = 0;
    GL::functions()->glGetIntegerv(GL_MAX_TEXTURE_SIZE, &maxTextureSize);
    auto downscaleFactor = std::max(1, (int) std::ceil(std::max((double) w / maxTextureSize, (double) h / maxTextureSize)));

    if (downscaleFactor > 1) {
        w = qRound((double) w / downscaleFactor);
        h = qRound((double) h / downscaleFactor);

        cv::Mat tmp;
        cv::resize(image.mat(), tmp, cv::Size(w, h));
        return tmp;
    }
    return image;
}

}