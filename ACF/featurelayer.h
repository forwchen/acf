//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FEATURELAYER_H
#define FEATURELAYER_H

#include <iostream>
#include <vector>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"

/*!
	The base class for a feature layer. Just like the DetectorModel-class we have the issue that ChnFtrs and DPM featurelayers are not alike yet, which makes this class not yet that useable
*/

class FeatureLayer {
public:
    FeatureLayer();
    std::vector<cv::Mat> FeatLayers;
};

#endif // FEATURELAYER_H
