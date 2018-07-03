//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef DETECTIONLIST_H_
#define DETECTIONLIST_H_

#include <vector>
#include "detection.h"
#include "FileWriter.h"
#include <string>
#include "ctime"

// Class to protect from memory-leaks when not releasing detections
class DetectionList {
    // Some classes that should have direct access to the detections
    friend class NonMaximumSuppression;
    friend class Combinator;

public:
    DetectionList();
    virtual ~DetectionList();

    // Add a DetectionList
    void addList(const DetectionList& DL, cv::Scalar C = cv::Scalar(0, 0, 255))
    {
        for (int d = 0; d < DL.getSize(); d++) {
            this->addDetection(DL.Ds[d]->getX(), DL.Ds[d]->getY(), DL.Ds[d]->getWidth(), DL.Ds[d]->getHeight(), DL.Ds[d]->getScore(), C);
        }
    }

    // Normalise the scores based on Mean and Std (standard normalisation)
    void normaliseScore(float mean, float std)
    {
        for (int d = 0; d < this->Ds.size(); d++) {
            Ds[d]->setScore((Ds[d]->getScore() - mean) / std);
        }
    }

    void addDetection(float x, float y, float w, float h, float score);
    void addDetection(float x, float y, float w, float h, float score, cv::Scalar C);

    // obtain the number of detections in the list
    int getSize() const
    {
        return this->Ds.size();
    }

    void tostring(int value, std::string& buf)
    {
        int i = 30;
        buf = "";
        for (; value && i; --i, value /= 10)
            buf = "0123456789abcdef"[value % 10] + buf;
    }

    // Draw the detections on Frame
    void Draw(cv::Mat& Frame, std::string dir, int cnt)
    {
        srand(time(NULL));
        std::string fn1, fn2;
        tostring(cnt, fn1);
        //return;
	for (int d = 0; d < Ds.size(); d++) {
            cv::Rect myROI(Ds[d]->getX(),
                Ds[d]->getY(),
                Ds[d]->getWidth(),
                Ds[d]->getHeight());
            cv::Mat croppedRef(Frame, myROI);
            tostring(d + 1, fn2);
            cv::imwrite(dir + fn1 + "-" + fn2 + ".jpg", croppedRef);
            cv::rectangle(Frame,cv::Point(Ds[d]->getX(), Ds[d]->getY()), cv::Point(Ds[d]->getX()+Ds[d]->getWidth(), Ds[d]->getY()+Ds[d]->getHeight()), Ds[d]->getColor(),3);
        }
    }

    // Resize the detections (required when working on an upscaled image)
    void resizeDetections(float scale)
    {
        for (int d = 0; d < this->Ds.size(); d++) {
            Ds[d]->resize(scale);
        }
    }

    // Write detections in XML-format to a file coupled by FW
    void WriteDetections(int frameID, FileWriter* FW, std::string filename, float rescale = 1.0) const
    {
        for (int d = 0; d < this->Ds.size(); d++) {
            Ds[d]->setFilename(filename);
            FW->writeDetection(frameID, Ds[d]);
        }
        if (this->Ds.size() == 0) {
            FW->writeDetection(frameID, NULL);
        }
    }

    DetectionList(const DetectionList& DL)
    {
        std::cout << "COPY" << std::endl;
        Ds.clear();
        //addList(DL);

        for (int d = 0; d < DL.getSize(); d++) {
            this->addDetection(DL.Ds[d]->getX(), DL.Ds[d]->getY(), DL.Ds[d]->getWidth(), DL.Ds[d]->getHeight(), DL.Ds[d]->getScore());
        }
    }

    //DetectionList& DetectionList::operator=( const DetectionList& other ) {
    DetectionList& operator=(const DetectionList& DL)
    {
        this->Ds.clear();
        for (int d = 0; d < DL.getSize(); d++) {
            this->addDetection(DL.Ds[d]->getX(), DL.Ds[d]->getY(), DL.Ds[d]->getWidth(), DL.Ds[d]->getHeight(), DL.Ds[d]->getScore());
        }

        return *this;
    }

private:
    // The detections
    std::vector<Detection*> Ds;
    // the path to the image
    std::string path;
};

#endif /* DETECTIONLIST_H_ */
