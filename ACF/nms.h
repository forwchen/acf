//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include <iostream>
#include <vector>
#include "detection.h"

#ifndef __H_NMS
#define __H_NMS

/*!
	After detection, detections that have to much overlap should be pruned and only the best detections (which should represent the actual object found) will be kept
*/
struct nmsHelperStruct {
    float score;
    int index;
};

/*!
	Perform non-maximum-suppression.
	\param Dets A vector of pointers to detections which could possible overlap.
	\param overlap The amount of overlap that is allowed between detections
	\return A vector of pointers to detections which do not overlap more than the overlap parameter
*/
std::vector<Detection*> nms(std::vector<Detection*> Dets, float overlap);

#endif
