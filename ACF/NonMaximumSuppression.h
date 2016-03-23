//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef NONMAXIMUMSUPPRESSION_H_
#define NONMAXIMUMSUPPRESSION_H_

#include "DetectionList.h"
#include "nms.h"
#include <vector>

class NonMaximumSuppression {

public:
    NonMaximumSuppression();
    virtual ~NonMaximumSuppression();

    DetectionList standardNMS(const DetectionList& DL);
    DetectionList dollarNMS(const DetectionList& DL);
};

#endif /* NONMAXIMUMSUPPRESSION_H_ */
