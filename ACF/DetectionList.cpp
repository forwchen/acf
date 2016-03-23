//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "DetectionList.h"

DetectionList::DetectionList()
{
}

DetectionList::~DetectionList()
{
    // delete the detections from the list
    for (int d = 0; d < this->Ds.size(); d++) {
        delete Ds[d];
    }
}

void DetectionList::addDetection(float x, float y, float w, float h, float score, cv::Scalar C)
{
    Detection* D = new Detection(x, y, w, h, score);
    D->setColor(C);
    this->Ds.push_back(D);
}

void DetectionList::addDetection(float x, float y, float w, float h, float score)
{
    Detection* D = new Detection(x, y, w, h, score);
    D->setColor(cv::Scalar(255, 0, 0));
    this->Ds.push_back(D);
}
