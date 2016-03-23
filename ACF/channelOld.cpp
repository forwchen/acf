//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "channelold.h"

//#include "integralfeature.h"

/*!
	\file The implementation of Channel member-functions

*/

/*!
	Constructor of the Channel-class
	\param numChans The amount of image-channels this channel will produce
*/
ChannelOld::ChannelOld(const int numChans);

/*!
	This is a very basic implementation to calculate features. Since the Channel class is only used as a base-class for useable channels, the functioning of this implementation justs need to work
        \author F. De Smedt
        \date 2014

*/
void ChannelOld::getFeatOfChannel(const cv::Mat& image, std::vector<cv::Mat>& Feat, cv::vector<cv::Mat>& FeatOriginal, int shrinking = 1)
{
    cv::Mat FeatureImage;
    FeatureImage = image;

    Feat.push_back(FeatureImage);
}

/*!
	Return the name of the channel.

        \author F. De Smedt
        \date 2014
	Return the name of the channel
*/
std::string ChannelOld::getName()
{
    return this->m_name;
}

/*!
	This basic version just returns the image as feature-image
	\author F. De Smedt
        \date 2014

*/
/*cv::Mat Channel::featureImage(const cv::Mat &image) {
    return image;
}
*/
/*!
	This is a basic version for returning the score of a feature on a certain position. This basic version will be called a lot, only if a specialised version of a channel (for example size, ...) will be used this function will be overloaded

	\param F The feature to evaluate
	\param Image The feature image (possible multi-channel). The structure of this image is dependend on the channel, so this allows to use specialised versions of images (such as 1 pixel large containing the layer-number)
	\param offsetX the X-position in the image
	\param offsetY the Y-position in the image
	\param shrinking The shrinking used (not useable yet)
 	\param offset The channel inside the multi-channel image
	\param method The method is default CHNFTRS, this is the only option currently available, but could later be extended with convolution


        \author F. De Smedt
        \date 2014

*/
float ChannelOld::giveScore(const Feature& F, const cv::Mat& Image, int offsetX, int offsetY, int shrinking, int offset, int method)
{
    /*
    Simple check if the offset is inside the available channels
    */
    if (offset >= this->getChannelAmount()) {
        std::cout << "Offset lies outside of number in internal channels" << std::endl;
        exit(1);
    }

    /*
    	Check if the featureis inside the feature image. A fail is most probably due to a mismatch in model size and evaluation positions

    */
    int FeaturePositionX = F.getX() + F.getWidth() + offsetX;
    int FeaturePositionY = F.getY() + F.getHeight() + offsetY;

    if (FeaturePositionX >= Image.cols || FeaturePositionY >= Image.rows) {
        std::cout << "The features lies outside the image!!" << std::endl;
        std::cout << "ImageSize: " << Image.cols << " x " << Image.rows << " Feature: X" << F.getX() << " width:" << F.getWidth() << "  offsetX:" << offsetX << "   Y:" << F.getY() << " height" << F.getHeight() << "  offsetY" << offsetY << std::endl;
        exit(4);
    }

    //Get the score of the feature out ofthe integral image
    if (method == CHNFTRS) {
        float a = Image.at<float>(F.getY() + offsetY, F.getX() + offsetX);
        float b = Image.at<float>(F.getY() + offsetY, F.getX() + F.getWidth() + offsetX);
        float d = Image.at<float>(F.getY() + F.getHeight() + offsetY, F.getX() + offsetX);
        float c = Image.at<float>(F.getY() + F.getHeight() + offsetY, F.getX() + F.getWidth() + offsetX);

        return (float)(a - b + c - d);
    }
    else {
        std::cout << "Evaluation method not supported yet" << std::endl;
        exit(3);
    }

    return 0;
}

void ChannelOld::setName(std::string name)
{
    this->m_name = name;
}

int ChannelOld::getChannelAmount()
{
    return this->m_numchans;
}
