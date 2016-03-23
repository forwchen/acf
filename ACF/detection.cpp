//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "detection.h"
/*!
	\file The implementation of functions used to work with the detection class

*/

/*!
	\brief Helper function to sort detections

	This is a helper function to sort detections based on the score they have
	\param a a pointer to a detection
	\param b a pointer to an other detection
	\return 1 of the score of a is higher, 0 otherwise

*/
bool compareByScore(const Detection* a, const Detection* b)
{
    return a->getScore() > b->getScore(); //sort from high to low
}

Detection::Detection(const Detection& D)
{
    this->m_x = D.getX();
    this->m_y = D.getY();
    this->m_width = D.getWidth();
    this->m_height = D.getHeight();
    this->m_filename = D.getFilename();
    this->m_color = D.getColor();
    this->m_modelName = D.getModelName();
    this->m_level = D.getLevel();
    this->m_score = D.getScore();
}

/*!
	\brief Sort the detections based on score

	This function sorts the detection based on score they have. The compareByScore function is used here to evaluate which score is higher.

	\author F. De Smedt
        \date 2014
*/
void SortDetections(std::vector<Detection*>& Dets)
{
    std::sort(Dets.begin(), Dets.end(), compareByScore);
}

/*! \brief Detection constructor

	The constructor of the Detection-class. Nothing has to be done at construction time

	\author F. De Smedt
        \date 2014
*/
Detection::Detection()
{
}

float Detection::getX() const
{
    return this->m_x;
}
float Detection::getY() const
{
    return this->m_y;
}
float Detection::getWidth() const
{
    return this->m_width;
}
float Detection::getHeight() const
{
    return this->m_height;
}
float Detection::getScore() const
{
    return this->m_score;
}
std::string Detection::getFilename() const
{
    return this->m_filename;
}
cv::Scalar Detection::getColor() const
{
    return this->m_color;
}
std::string Detection::getModelName() const
{
    return this->m_modelName;
}
int Detection::getLevel() const
{
    return this->m_level;
}
void Detection::setX(float x)
{
    this->m_x = x;
}
void Detection::setY(float y)
{
    this->m_y = y;
}
void Detection::setWidth(float width)
{
    this->m_width = width;
}
void Detection::setHeight(float height)
{
    this->m_height = height;
}
void Detection::setScore(float score)
{
    this->m_score = score;
}
void Detection::setFilename(std::string filename)
{
    this->m_filename = filename;
}

void Detection::setColor(cv::Scalar color)
{
    this->m_color = color;
}
void Detection::setModelName(std::string modelname)
{
    this->m_modelName = modelname;
}
