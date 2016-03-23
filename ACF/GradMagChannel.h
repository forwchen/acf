/*
 * GradMagChannel.h
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#ifndef GRADMAGCHANNEL_H_
#define GRADMAGCHANNEL_H_

#include "Channel.h"
#include "ColorChannel.h"

class GradMagChannel : public Channel {
public:
    GradMagChannel(const ColorChannel& CC);
    virtual ~GradMagChannel();

    float* getMagnitude() const;
    float* getOrientation() const;

    static float getLambda()
    {
        return 0.1158;
    }

private:
    float* mag;
    float* orientation;
};

#endif /* GRADMAGCHANNEL_H_ */
