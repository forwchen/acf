//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "detectormodel.h"
/*!
	\file The implementation of the member functions for the DetectorModel class


*/

/*!
	\brief Constructor for the DetectorModel class

	In this DetectorModel-constructor, only the size of the model is set. This forms the base for detector-specific models
*/
DetectorModel::DetectorModel(int width, int height)
    : m_width(width)
    , m_height(height)
    , mean_value(0)
    , standard_deviation(1)
{
}

int DetectorModel::getWidth() const
{
    return this->m_width;
}

int DetectorModel::getHeight() const
{
    return this->m_height;
}

std::string DetectorModel::getModelName() const
{
    return this->m_name;
}

cv::Scalar DetectorModel::getModelColor() const
{
    return this->m_color;
}

void DetectorModel::setWidth(int w)
{
    this->m_width = w;
}

void DetectorModel::setHeight(int h)
{
    this->m_height = h;
}

void DetectorModel::setColor(cv::Scalar color)
{
    this->m_color = color;
}
