//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "FrameFromVideo.h"

FrameFromVideo::FrameFromVideo()
    : cap(cv::VideoCapture("Video.avi"))
{
}

FrameFromVideo::~FrameFromVideo()
{
}

cv::Mat FrameFromVideo::giveFrame()
{
    cv::Mat frame;

    if (cap.get(CV_CAP_PROP_POS_FRAMES) < cap.get(CV_CAP_PROP_FRAME_COUNT)) {
        cap >> frame;
    }

    return frame;
}

bool FrameFromVideo::isend()
{
    return cap.get(CV_CAP_PROP_POS_FRAMES) >= cap.get(CV_CAP_PROP_FRAME_COUNT);
}
