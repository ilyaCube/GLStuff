#pragma once

#include <QSize>
#include "Imaging/OpenCV.hpp"

namespace Library::Imaging {

class Image
{
public:
    Image();

    Image(const cv::Mat& mat);

    Image(const Image& other);

    inline operator cv::Mat() const
    {
        return _mat;
    }

    inline bool operator==(const Image& other) const
    {
        return _mat.data == other._mat.data;
    }

    inline bool operator!=(const Image& other) const
    {
        return !(*this == other);
    }

    inline cv::Mat mat() const
    {
        return _mat;
    }

    inline bool isEmpty() const
    {
        return _mat.empty();
    }

    inline QSize size() const
    {
        return QSize(_mat.cols, _mat.rows);
    }

    inline size_t totalSpace() const
    {
        if(_mat.empty()) {
            return 0;
        }
        return _mat.total() * _mat.elemSize();
    }

protected:
    cv::Mat _mat;
};

}