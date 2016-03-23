/*
 * ACFDetector.h
 *
 *  Created on: Jan 16, 2015
 *      Author: fds
 */

#ifndef ACFDETECTOR_H_
#define ACFDETECTOR_H_

#include <vector>
#include <string>
#include <iostream>
#include <sstream>
#include <fstream>

#include "detection.h"

#include "rapidxml-1.13/rapidxml.hpp"
#include "rapidxml-1.13/rapidxml_print.hpp"

#include "ChannelFeatures.h"

#include "DetectionList.h"
#include "detector.h"

using namespace rapidxml;

class ACFDetector : public Detector {
public:
    ACFDetector()
    {
        modifyCascade(-0.01);

        ReadModel("Config/INRIA_ACF.xml");
    }
    virtual ~ACFDetector();

    std::vector<Detection*> Detect(const ChannelFeatures* features) const;

    int getShrinking() const
    {
        return this->shrinking;
    }

    void modifyCascade(float score)
    {
        for (int s = 0; s < this->Values.size(); s++) {
            for (int l = 0; l < this->Values[s].size(); l++) {
                this->Values[s][l] += score;
            }
        }
    }

    ACFDetector(std::string modelfile);

    DetectionList applyDetector(const cv::Mat& Frame) const;

private:
    void getChild(const ChannelFeatures* features, int& k0, int& k, int c, int r, int s, int channelwidth, int channelheight, int modelwidth, int modelheight) const;

    void setWidth(float w)
    {
        this->modelwidth = w;
    }

    void setHeight(float h)
    {
        this->modelheight = h;
    }

    void setWidthPad(float w)
    {
        this->modelwidthpad = w;
    }

    void setHeightPad(float h)
    {
        this->modelheightpad = h;
    }

    void ReadModel(std::string modelfile);

    //! holds indeces to follow through a stage evaluation, normally these are the same in every stage since these represent the decision stump tree
    std::vector<std::vector<int> > Child;

    //! holds indeces to follow through a stage evaluation, normally these are the same in every stage since these represent the decision stump tree
    std::vector<std::vector<int> > depth;

    //! holds the indeces of the features to use, no longer used when features are inside the model
    std::vector<std::vector<int> > Fid;

    //! The thresholds that should be reached for each feature
    std::vector<std::vector<float> > Thresholds;

    //! stores the values to be added/subtracted at the end of a stage (dependent on the leaf reached)
    std::vector<std::vector<float> > Values;

    float modelwidth, modelheight;
    float modelwidthpad, modelheightpad;
    int shrinking;
    int ModelDepth;
};

#endif /* ACFDETECTOR_H_ */
