//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FRAMEFROMCAMERA_H_
#define FRAMEFROMCAMERA_H_

#include "FrameProducer.h"
#include <iostream>

class FrameFromCamera : public FrameProducer {
public:
    FrameFromCamera();
    virtual ~FrameFromCamera();

    cv::Mat giveFrame();
    bool isend();

    std::string getFilename()
    {
        return "camera";
    }

private:
    cv::VideoCapture cap;
};

#endif /* FRAMEFROMCAMERA_H_ */
