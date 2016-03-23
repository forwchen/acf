//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef SCALESPACEPYRAMID_H_
#define SCALESPACEPYRAMID_H_

#include <vector>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

class ScaleSpacePyramid {
private:
    std::vector<cv::Mat> Images;
    std::vector<float> scales;

public:
    ScaleSpacePyramid(const cv::Mat& image, float rescaleratio, cv::Size modelSize, float Upscale = 1.0);

    virtual ~ScaleSpacePyramid();

    cv::Mat getImage(int Layer)
    {
        if (Layer < this->getNumLayers())
            return this->Images[Layer];
        else
            return cv::Mat();
    }

    float getLayerScale(int Layer)
    {
        if (Layer < this->getNumLayers())
            return this->scales[Layer];
        else
            return 1;
    }

    int getNumLayers()
    {
        return this->Images.size();
    }
};

#endif /* SCALESPACEPYRAMID_H_ */
