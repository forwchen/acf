//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "ScaleSpacePyramid.h"

float getScale(cv::Mat& Orimage, cv::Mat& image)
{
    return std::min((float)image.cols / Orimage.cols, (float)image.rows / Orimage.rows);
}

ScaleSpacePyramid::ScaleSpacePyramid(const cv::Mat& image2, float rescaleratio, cv::Size modelSize, float Upscale)
{
    int smallestWidth = modelSize.width;
    int smallestHeight = modelSize.height;
    cv::Mat image = image2.clone();
    cv::Mat orImage = image.clone();

    float scale = 1;
    int layer = 0;

    while (image.cols > smallestWidth && image.rows > smallestHeight) {
        cv::Mat R;
        this->scales.push_back(1 / pow(rescaleratio, (layer)));
        cv::Size sz = cv::Size(cvRound(orImage.cols * (Upscale / pow(rescaleratio, (layer)))), orImage.rows * (Upscale / pow(rescaleratio, (layer))));
        layer++;
        cv::resize(orImage, image, sz);

        this->Images.push_back(image.clone());
    }
}

ScaleSpacePyramid::~ScaleSpacePyramid()
{
}
