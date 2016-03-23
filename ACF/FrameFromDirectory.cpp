//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "filelocator.h"
#include "FrameFromDirectory.h"

FrameFromDirectory::FrameFromDirectory()
{
    FileLocator FL;
    // Open the local directory to seach for files
    Files = FL.listFile(".", ".jpg");
    std::cout << "We have " << Files.size() << " files to process" << std::endl;
    start = 0;
    end = Files.size();
    position = start;
}

FrameFromDirectory::FrameFromDirectory(std::string directory)
{
    FileLocator FL;
    Files = FL.listFile(directory, ".jpg");
    start = 0;
    end = Files.size();
    position = start;
}

FrameFromDirectory::~FrameFromDirectory()
{
}

cv::Mat FrameFromDirectory::giveFrame()
{
    std::cout << "Giving file " << position << "/" << Files.size() << "\t" << Files[position] << std::endl;
    cv::Mat Frame = cv::imread(Files[position++]);

    return Frame;
}

cv::Mat FrameFromDirectory::giveFrame(int index)
{
    //std::cout << "Giving file " << position << "/" << Files.size() << "\t" << Files[index]<< std::endl;
    cv::Mat Frame = cv::imread(Files[index]);

    return Frame;
}

bool FrameFromDirectory::isend()
{
    return (position >= Files.size()) || (position >= end);
}
