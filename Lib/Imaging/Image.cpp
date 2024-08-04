#include "Image.hpp"

namespace Library::Imaging {

Image::Image(const cv::Mat& mat) :
    _mat(mat)
{}

Image::Image() :
    Image(cv::Mat())
{}

Image::Image(const Image& other) :
    Image(other._mat)
{}

}