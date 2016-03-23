//============================================================================
// Name        : CombinationFramework.cpp
// Author      : F. De Smedt @ EAVISE
// Version     :
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
// Description : Source for the default application
//============================================================================

#include <stdio.h>
#include <stdlib.h>
#include <vector>

#include <iomanip>

// for input possibilities
#include "FrameFromDirectory.h"
#include "FrameFromCamera.h"
#include "FrameFromVideo.h"

// detection includes
//#include "src/detection.h"
#include "DetectionList.h"
#include "NonMaximumSuppression.h"

#include "DetectorManager.h"

#include "FileWriter.h"
#include "iostream"
#include "timer.h"
// namespaces
using namespace std;
using namespace cv;
string buf;

inline string to_string(int value)
{
    int i = 30;
    buf = "";
    for (; value && i; --i, value /= 10)
        buf = "0123456789"[value % 10] + buf;
    return buf;
}

int main(int argc, char** argv)
{

    // Initialise the input
    FrameProducer* FP = new FrameFromDirectory(argv[1]);
    FileWriter* FW;
    DetectorManager DM;

    NonMaximumSuppression NMS;
    // Open the file for result writing
    FW = new FileWriter("detection.xml");

    string output(argv[2]);
    output += "/";

    int cnt = 0;
    while (!FP->isend()) {
        // retrieve frame
        cv::Mat Frame = FP->giveFrame();

        cnt++;
        // Timer t; t.start();
        //perform detection
        DetectionList DL = DM.applyDetector(Frame);

        //NMS
        DetectionList NMax = NMS.dollarNMS(DL);

        // cout<<t.count()<<endl;
        //Write detections to file
        NMax.WriteDetections(cnt, FW, FP->getFilename());

        // Draw the detections and show them
        NMax.Draw(Frame, output, cnt);
        //cv::imshow("Frame", Frame);

        //cv::waitKey(0);
    }

    delete FP;

    delete FW;
    return EXIT_SUCCESS;
}
