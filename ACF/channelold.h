//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef CHANNEL_H
#define CHANNEL_H

#include <iostream>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "feature.h"

#define CHNFTRS 0
#define CONVOLUTION 1

/*!
	\brief The base class for channels (used in ChnFtrs-based detectors)

	This is the base class which describes a channel. When a new channel has to be defined, just inherit from this class and fill in the virtual functions and maybe the GiveScore-function (when you want to deviate from default sum-feature, such as the layer or size of a model).
*/

class ChannelOld {
private:
    //! number of image-channels
    int m_numchans;
    //! The name of this channel
    std::string m_name;

protected:
    /*!
	Set the name of the channel. This name is used for dependencies
    */
    void setName(std::string name);

public:
    //! The constructor initialises the channel with the number of image-channels
    ChannelOld(const int numChans)
        : m_numchans(numChans)
    {
    }
    //! returns the name of the channel
    std::string getName();

    float giveScore(const Feature& F, const cv::Mat& Image, int offsetX, int offsetY, int shrinking, int offset, int method = CHNFTRS);

    /*!
    Calculate the features of an image using this channel. This functions will be overloaded by each child
    \param image The image to calculate features of
    \param Feat A vector where the resulting FeatureImage will be added to. By passing the features by using a vector by reference allows to reuse channel features that are calculated ealier
    \param FeatOriginal This vector is the same as the Feat vector, bu has the original format of the resulting images (before shrinking)
    \param shrinking The amount of shrinking used. Not implemented yet
    */
    virtual void getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, std::vector<cv::Mat>& FeatOriginal, int shrinking) = 0;

    //! return the amount of channels-images this channel will generate
    int getChannelAmount();

    //! A string that names the dependencies of this channel
    std::vector<std::string> m_dependencies;
    //! This vector holds the index of the dependencies in the vector of feature-images.
    std::vector<int> m_dependencies_index;
};

#endif // CHANNEL_H
