//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef DETECTORMANAGER_H_
#define DETECTORMANAGER_H_

#include "ACFDetector.h"

/*
 *  This is a broker-class to hide the handling of detector creation, removal, ...
 */

class DetectorManager {
public:
    DetectorManager();
    virtual ~DetectorManager();

    DetectionList applyDetector(cv::Mat& Frame);

private:
    Detector* Det;
};

#endif /* DETECTORMANAGER_H_ */
