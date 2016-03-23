//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef __H_CHANLIST
#define __H_CHANLIST

#include <vector>
#include <string>

#include "channelold.h"
#include "LoadChannels.hpp"

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

/*! 
	ChannelList is a wrapper for the channels. This wrapper tries to ensure memoryleak-free code by  allocating and deallocating memory for the end-user
*/

class ChannelList {
public:
    ChannelList(std::vector<std::string> names)
    {
        m_channels = LoadChannels(names);
    }

    ChannelList()
    {
    }

    void FillChannels(std::vector<std::string> names)
    {

        freeChannels(); //make sure channels is empty
        m_channels = LoadChannels(names);
    }

    void freeChannels()
    {
        for (int i = 0; i < m_channels.size(); i++)
            delete m_channels[i];
        m_channels.clear();
    }

    ~ChannelList()
    {
        freeChannels();
    }

    /*!
        Calculate the features   => This should become a member function of the detector
        \author F. De Smedt
        \date 2014

    */
    std::vector<cv::Mat> getFeaturesFromChannels(const cv::Mat& image, int shrinking = 1)
    {
        std::vector<cv::Mat> Features;
        std::vector<cv::Mat> FeatTemp;
        std::vector<cv::Mat> FeatOriginal; //used if shrinking is on
        FeatTemp.clear();

        if (image.channels() != 3) {
            std::cout << "Amount of channels in image is NOT ok, we expect a 3-channel image as input!" << std::endl;
            exit(3);
        }

        /*Convert to double image ...*/
        cv::Mat II_RGB, II_Conv;
        cv::cvtColor(image, II_RGB, CV_BGR2RGB);

        II_RGB.convertTo(II_Conv, CV_32FC3, 1 / 255.0);

        for (int c = 0; c < m_channels.size(); c++) {
            m_channels[c]->getFeatOfChannel(II_Conv, FeatTemp, FeatOriginal, shrinking);
        }

        for (int c = 0; c < FeatTemp.size(); c++) {

            std::vector<cv::Mat> F;
            cv::split(FeatTemp[c], F);
            std::copy(F.begin(), F.end(), back_inserter(Features));
        }

        return Features;
    }

private:
    std::vector<ChannelOld*> m_channels;
};
#endif
