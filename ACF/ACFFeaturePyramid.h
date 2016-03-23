/*
 * ACFFeaturePyramid.h
 *
 *  Created on: Feb 3, 2015
 *      Author: fds
 */

#ifndef ACFFEATUREPYRAMID_H_
#define ACFFEATUREPYRAMID_H_

#include "ChannelFeatures.h"
#include "ACFDetector.h"

class ACFFeaturePyramid {
public:
    ACFFeaturePyramid(const cv::Mat& Image, int scalesPerOct, cv::Size minSize, const ACFDetector& ADet);

    // Constructor for single scale
    ACFFeaturePyramid(const cv::Mat& Image, float sc, cv::Size minSize, const ACFDetector& ADet);

    virtual ~ACFFeaturePyramid();

    int getAmount()
    {
        return this->Layers.size();
    }

    ChannelFeatures* getLayer(int L)
    {
        if (L < this->Layers.size()) {
            return this->Layers[L];
        }
        else {
            std::cerr << "Requesting unknown layer ..." << std::endl;
            exit(1);
        }
    }

    float getScale(int L)
    {
        return this->scales[L];
    }

protected:
    std::vector<ChannelFeatures*> Layers;

    // amount of scales in each octave (so between halving each image dimension )
    int scalesPerOct;

    // Minimum size an image can have (size of the model)
    cv::Size minSize;

    // Amount of layer is approximate for each real calculated one, normaly one per octave is correctly calculated
    int approxAmount;

    std::vector<float> scales;
};

#endif /* ACFFEATUREPYRAMID_H_ */
