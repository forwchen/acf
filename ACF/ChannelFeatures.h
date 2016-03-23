/*
 * ChannelFeatures.h
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#ifndef CHANNELFEATURES_H_
#define CHANNELFEATURES_H_

#include <vector>
#include <iostream>

//OpenCV headers
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/gpu/gpu.hpp"
#include "opencv2/highgui/highgui.hpp"

// Channel-classes
#include "ColorChannel.h"
#include "GradMagChannel.h"
#include "GradHistChannel.h"

/*
 * This class will be used to generate the features. By hiding the implementation details of the channels,
 * we can avoid having memory-leaks due to users who are not familiar with the channel-functions (which are
 *  build around malloc's/calloc's and free's)
*/
class ChannelFeatures {
public:
    ChannelFeatures(const cv::Mat& Image, int shrinking);
    ChannelFeatures* ApproxChannel(const cv::Mat& Image, int shrinking, float scaleN, float scaleR);
    virtual ~ChannelFeatures();

    void printFeatures(int index) const
    {

        for (int y = 0; y < Channelheight; y++) {
            for (int x = 0; x < Channelwidth; x++) {
                std::cout << Features[index][x * Channelheight + y] << " ";
            }
            std::cout << std::endl;
        }
    }

    int getChannelWidth() const
    {
        return this->Channelwidth;
    }

    int getChannelHeight() const
    {
        return this->Channelheight;
    }

    int getnChannels() const
    {
        return this->Features.size();
    }

    float getFeatureValue(int channel, int location) const;

    void SmoothChannels();

private:
    ChannelFeatures(int width, int height, std::vector<float*> F)
        : Channelwidth(width)
        , Channelheight(height)
    {
        this->Features = F;
    } //only used in approximation

    void addChannelFeatures(Channel& ch);

    std::vector<float*> Features;
    int Channelwidth, Channelheight;
};

#endif /* CHANNELFEATURES_H_ */
