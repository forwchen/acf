/*
 * GradMagChannel.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */
//#include "imResampleMex.hpp"
//#include "rgbConvertMex.hpp"

#include "GradMagChannel.h"
#include "Functions.h"

GradMagChannel::~GradMagChannel()
{
    //free(mag);
    //mag = NULL; //avoids an error on double free
    free(orientation);
}

float* GradMagChannel::getMagnitude() const
{
    return this->mag;
}

float* GradMagChannel::getOrientation() const
{
    return this->orientation;
}
