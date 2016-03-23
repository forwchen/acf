//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FRAMEPRODUCER_H_
#define FRAMEPRODUCER_H_

//OpenCV headers
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/highgui/highgui.hpp"
#include <string>

// Class to inherit from by classes that return a frame (from directory, camera, network, ...)
class FrameProducer {
public:
    FrameProducer();
    virtual ~FrameProducer();

    virtual cv::Mat giveFrame() = 0;
    virtual std::string getFilename() = 0;

    virtual bool isend() = 0;

private:
};

#endif /* FRAMEPRODUCER_H_ */
