#pragma once

#include <QString>
#include "OpenCV.hpp"
#include "Image.hpp"

namespace Library::Imaging {

Image load(const QString& filename, int flags = cv::IMREAD_COLOR);

}