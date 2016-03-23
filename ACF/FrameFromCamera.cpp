//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "FrameFromCamera.h"

FrameFromCamera::FrameFromCamera()
{
    cap.open(0);

    if (!cap.isOpened()) {
        std::cerr << "Failed to open capture device" << std::endl;
        exit(2);
    }
}

FrameFromCamera::~FrameFromCamera()
{
}

cv::Mat FrameFromCamera::giveFrame()
{
    cv::Mat frame;

    // keep hoping to receive a frame
    do {
        cap >> frame;
        std::cout << "." << std::endl;
    } while (frame.empty());

    return frame;
}

bool FrameFromCamera::isend()
{
    return false;
}
