//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FRAMEFROMVIDEO_H_
#define FRAMEFROMVIDEO_H_

#include "FrameProducer.h"

class FrameFromVideo : public FrameProducer {
public:
    FrameFromVideo();
    virtual ~FrameFromVideo();

    cv::Mat giveFrame();
    bool isend();

    std::string getFilename()
    {
        return "video";
    }

private:
    cv::VideoCapture cap;
};

#endif /* FRAMEFROMVIDEO_H_ */
