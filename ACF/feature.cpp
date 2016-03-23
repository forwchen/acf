//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "feature.h"

Feature::Feature()
{
}

void Feature::setX(int x)
{
    this->m_x = x;
}
void Feature::setY(int y)
{
    this->m_y = y;
}
void Feature::setWidth(int width)
{
    this->m_width = width;
}
void Feature::setHeight(int height)
{
    this->m_height = height;
}

void Feature::setChannel(int channel)
{
    this->m_channel = channel;
}

int Feature::getX() const
{
    return m_x;
}
int Feature::getY() const
{
    return m_y;
}
int Feature::getWidth() const
{
    return m_width;
}
int Feature::getHeight() const
{
    return m_height;
}

//void setChannel(channel *ch){this->chan = ch;}
int Feature::getChannel() const
{
    return this->m_channel;
}
