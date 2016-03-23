//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "detector.h"
/*! \file The implementation of the Detector-class

*/

/*! Constructor of the Detector class

	This implementation of the constructor of the Detector class. Since Detector is a base class for other detectors, nothing specific has to be performed here.
        \author F. De Smedt
        \date 2014
*/
Detector::Detector()
{
    this->mean = 0;
    this->std = 1;
    this->confidence = 1;
    this->complementarity = 1;
}
