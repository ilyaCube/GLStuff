#include "Helpers.hpp"

namespace Library::Imaging {

Image load(const QString& filename, int flags)
{
    return Image{cv::imread(filename.toStdString(), flags)};
}

}