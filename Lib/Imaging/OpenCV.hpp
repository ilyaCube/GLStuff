#pragma once

#include "Common/OsDetection.hpp"
#include <opencv2/opencv.hpp>

#if CV_MAJOR_VERSION >= 4
    #ifndef OS_WINDOWS
        #include <opencv4/opencv2/imgproc/imgproc_c.h>
    #else
        #include <opencv2/imgproc/imgproc_c.h>
    #endif
#endif