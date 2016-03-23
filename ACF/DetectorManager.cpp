//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "DetectorManager.h"

DetectorManager::DetectorManager()
{

    // uncomment to use Combinator detector
    // Det = new Combinator();

    // uncomment to use DPM detector first version
    // Det = new DPMDetectorOld();

    // uncomment to use new DPM detector.
    // Det = new DPMDetector();

    // uncomment to use ICF detector
    // Det = new ChnFtrsDetector();

    // uncomment to use ICF detector
    Det = new ACFDetector();

    // HOG detector
    // Det = new HOGDetector();
}

DetectorManager::~DetectorManager()
{
    // cleanly delete the created detector
    delete Det;
}

DetectionList DetectorManager::applyDetector(cv::Mat& Frame)
{
    // forward the detection request to the detector
    return Det->applyDetector(Frame);
}
