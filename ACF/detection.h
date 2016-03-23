//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef DETECTION_H
#define DETECTION_H
#include <iostream>
#include <string>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

/*!
	\file Declaration of the Detection class
*/

/*!
	This class represents a detection. It can also be used for annotations. This class forms the input and output for our detection/training-software. By making this format detector-independend we simplify the switching between different detectors
*/

class Detection {
public:
    Detection();
    Detection(float x, float y, float width, float height, float score)
    {
        this->setX(x);
        this->setY(y);
        this->setWidth(width);
        this->setHeight(height);
        this->setScore(score);
    }

    Detection(float x, float y, float width, float height, float score, std::string name)
    {
        this->setX(x);
        this->setY(y);
        this->setWidth(width);
        this->setHeight(height);
        this->setScore(score);
        this->setModelName(name);
    }

    Detection(Detection* D)
    {
        this->setX(D->getX());
        this->setY(D->getY());
        this->setWidth(D->getWidth());
        this->setHeight(D->getHeight());
        this->setScore(D->getScore());
        this->setColor(D->getColor());
    }

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    float getScore() const;
    std::string getFilename() const;
    cv::Scalar getColor() const;

    std::string getModelName() const;
    int getLevel() const;

    void setX(float x);

    void setY(float y);

    void setWidth(float width);

    void setHeight(float height);

    void setScore(float score);

    void setFilename(std::string filename);

    void setColor(cv::Scalar color);

    void setModelName(std::string modelname);

    void resize(float factor)
    {
        this->setX(this->getX() / factor);
        this->setY(this->getY() / factor);
        this->setWidth(this->getWidth() / factor);
        this->setHeight(this->getHeight() / factor);
    }

    Detection(const Detection& D);

private:
    float m_x, m_y, m_width, m_height;
    float m_score;
    std::string m_filename;
    cv::Scalar m_color;
    std::string m_modelName;
    int m_level;
};

bool compareByScore(const Detection* a, const Detection* b);
void SortDetections(std::vector<Detection*>& Dets);

#endif // DETECTION_H
