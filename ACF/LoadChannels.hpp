//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef _H_LOADCHANS
#define _H_LOADCHANS

#include <stdio.h>
#include <stdlib.h>
#include "imPadMex.hpp"

#include "Functions.h"
#include "wrappers.hpp"

#include <vector>
#include <iostream>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "channelold.h"

class LUVChannel : public ChannelOld {
public:
    LUVChannel()
        : ChannelOld(3)
    {
        this->setName("luv");
    }
    void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking);
};

class HSVChannel : public ChannelOld {
public:
    HSVChannel()
        : ChannelOld(3)
    {
        this->setName("hsv");
    }
    void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking);
};

class GrayChannel : public ChannelOld {
public:
    GrayChannel()
        : ChannelOld(1)
    {
        this->setName("gray");
    }
    void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking);
};

class GradMagChannelOld : public ChannelOld {
public:
    GradMagChannelOld()
        : ChannelOld(2)
    {
        this->setName("gradmag");
        this->m_dependencies.push_back("luv");
    }
    void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking);
};

class HogChannel : public ChannelOld {
public:
    HogChannel()
        : ChannelOld(6)
    {
        this->setName("hog");
        this->m_dependencies.push_back("gradmag");
    }
    void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking);
};

std::vector<ChannelOld*> LoadChannels(std::vector<std::string> Names);
std::vector<cv::Mat> getFeaturesFromChannels(std::vector<ChannelOld*> Chans, cv::Mat& image, int shrinking);
#endif
