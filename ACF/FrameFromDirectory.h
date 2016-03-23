//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FRAMEFROMDIRECTORY_H_
#define FRAMEFROMDIRECTORY_H_

#include "FrameProducer.h"

class FrameFromDirectory : public FrameProducer {
public:
    FrameFromDirectory();
    FrameFromDirectory(std::string directory);
    virtual ~FrameFromDirectory();
    cv::Mat giveFrame();
    cv::Mat giveFrame(int f);
    bool isend();

    int getNumFiles()
    {
        return Files.size();
    }

    std::string getFilename()
    {
        return Files[position - 1];
    }

    std::string getFilename(int index)
    {
        return Files[index];
    }

private:
    int start, end, position;
    std::vector<std::string> Files;
};

#endif /* FRAMEFROMDIRECTORY_H_ */
