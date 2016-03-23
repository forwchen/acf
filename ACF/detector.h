//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef DETECTOR_H
#define DETECTOR_H

#include <iostream>
#include <string>
#include <vector>

#include "detection.h"
//#include "channel.h"

#include "detectormodel.h"

#include "DetectionList.h"

//#include "boost/program_options.hpp"
//#include "Image.hpp"

/*!
	The base class of a detector.
*/
class Detector {
private:
protected:
    std::string m_name;
    float mean;
    float std;
    float confidence;
    float complementarity;

public:
    /*!Constructor for a detector. Not realy used since Detector forms a base class for other detector classes*/
    Detector();

    ~Detector() {}

    float getConfidence() const
    {
        return this->confidence;
    }
    float getComplementarity() const
    {
        return this->complementarity;
    }

    float getMean()
    {
        return this->mean;
    }

    float getStd()
    {
        return this->std;
    }

    virtual DetectionList applyDetector(const cv::Mat& Frame) const = 0;
};
#endif // DETECTOR_H
