//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef DETECTORMODEL_H
#define DETECTORMODEL_H

#include <iostream>
#include <vector>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

#include "detection.h"
//#include "featurelayer.h"

/*!
	The base-class of a model for a detector. Currently the models between DPM and ChnFtrs are very different, but in the future these should become more alike.
*/

class DetectorModel {
public:
    /*!
    	Initialisation of the DetectorModel. Setting height and width of the model
    */
    DetectorModel(int width, int height);

    /*!
    	Get the model width
    */
    int getWidth() const;

    /*!
    	Get the model height
    */
    int getHeight() const;

    /*!
    	Get the model name
    */
    std::string getModelName() const;
    /*!
    	Get the color of the model for drawing model-specifics such as detections/scores/...
    */
    cv::Scalar getModelColor() const;

    /*!
    	Setting the width of the model
    */
    void setWidth(int w);
    /*!
    	setting the height of the model
    */
    void setHeight(int h);

    /*!
    	Set the color of the model
    */
    void setColor(cv::Scalar color);
    /*!
    	Apply the model on the features of an image. This function has to be overloaded by child classes.
    */
    //   virtual std::vector<Detection*> applyModel(FeatureLayer *FeatLay, int offsetX, int offsetY,float ScaleFactorX, float ScaleFactorY, bool Soft, float threshold, int padding=0) = 0;

protected:
    //! Width of the model
    int m_width;
    //! Height of the model
    int m_height;
    //! Color for the model-specific drawing results
    cv::Scalar m_color;
    //! Name of the model
    std::string m_name;

    void setMean(float m)
    {
        this->mean_value = m;
    }
    void setSTD(float s)
    {
        this->standard_deviation = s;
    }
    float getMean()
    {
        return this->mean_value;
    }
    float getSTD()
    {
        return this->standard_deviation;
    }

    float normaliseScore(float score)
    {
        return (score - this->getMean()) / (this->getSTD());
    }

private:
    float mean_value;
    float standard_deviation;
};

#endif // DETECTORMODEL_H
