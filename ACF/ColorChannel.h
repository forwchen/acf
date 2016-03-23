/*
 * ColorChannel.h
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#ifndef COLORCHANNEL_H_
#define COLORCHANNEL_H_

#include <string>
#include "Channel.h"

#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/highgui/highgui.hpp"

float* Convert_ToMatlab(const cv::Mat& image, float scale);

class ColorChannel : public Channel {
public:
    ColorChannel(const cv::Mat& Image);
    virtual ~ColorChannel();

    static float getLambda()
    {
        return 0.0;
    }

private:
    void setColorSpace(std::string cspace)
    {
        if (cspace == "luv") {
            this->colorSpace = cspace;
        }
        else {
            std::cerr << "Unknown colorspace!!" << std::endl;
        }
    }
    std::string colorSpace;
};

#endif /* COLORCHANNEL_H_ */
