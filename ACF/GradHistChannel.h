/*
 * GradHistChannel.h
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#ifndef GRADHISTCHANNEL_H_
#define GRADHISTCHANNEL_H_

#include "Channel.h"
#include "GradMagChannel.h"

class GradHistChannel : public Channel {
public:
    GradHistChannel(const GradMagChannel& GMC);
    virtual ~GradHistChannel();

    static float getLambda()
    {
        return 0.1158;
    }

private:
    float* histogram;
};

#endif /* GRADHISTCHANNEL_H_ */
