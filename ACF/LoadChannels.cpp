//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include <string>
#include <vector>
#include "channelold.h"
#include "LoadChannels.hpp"

/*!
	Allocate a channel based on a name given. When a new channel-type is created, it should be added to this list to be able to use it

        \author F. De Smedt
        \date 2014

*/
ChannelOld* loadChanByName(std::string Ch_Name)
{
    std::string Name = Ch_Name;
    if (Name == "luv") {
        std::cout << "Loading LUV" << std::endl;
        return new LUVChannel();
    }
    else if (Name == "hsv") {
        std::cout << "Loading HUV" << std::endl;
        return new HSVChannel();
    }
    else if (Name == "gray") {
        std::cout << "Loading gray" << std::endl;
        return new GrayChannel();
    }
    else if (Name == "hog") {
        std::cout << "Loading Gradient Histogram" << std::endl;
        return new HogChannel();
    }
    else if (Name == "gradmag") {
        std::cout << "Loading Gradient Magnitude" << std::endl;
        return new GradMagChannelOld();
    }
    else {
        std::cerr << Name << " is NOT a valid channel" << std::endl;
        exit(1);
    }

    return new LUVChannel();
}

/*!
	Since some channels use eachothers information, it is necessary to control if channels dependend on are also in the list
        \author F. De Smedt
        \date 2014

*/
std::vector<ChannelOld*> LoadDependency(std::string Name)
{
    std::vector<ChannelOld*> Ch_vec;

    ChannelOld* ch = loadChanByName(Name);

    for (int c = 0; c < ch->m_dependencies.size(); c++) {
        std::vector<ChannelOld*> C = LoadDependency(ch->m_dependencies[c]);
        std::copy(C.begin(), C.end(), back_inserter(Ch_vec));
    }

    Ch_vec.push_back(ch);
    return Ch_vec;
}

/*!
	The infterface towards to ouside world to load the channels based on a vector of names
        \author F. De Smedt
        \date 2014

*/

std::vector<ChannelOld*> LoadChannels(std::vector<std::string> ChanNames)
{

    std::vector<ChannelOld*> Chans;
    std::vector<ChannelOld*> ChansTemp;

    /*Order for dependencies*/

    bool ready = false;
    int index = 0;

    for (int c = 0; c < ChanNames.size(); c++) {
        std::vector<ChannelOld*> F = LoadDependency(ChanNames[c]);
        std::copy(F.begin(), F.end(), back_inserter(Chans));
    }

    /*Make a unique list*/
    for (int c = Chans.size() - 1; c >= 0; c--) {
        for (int c2 = 0; c2 < c; c2++) {
            if (Chans[c]->getName() == Chans[c2]->getName()) {
                /*ERASE*/
                std::cout << "Item at index " << c << " is the same as at index " << c2 << std::endl;
                delete Chans[c];
                Chans.erase(Chans.begin() + c);
                break;
            }
        }
    }

    /*Fill in the indeces for dependencies*/
    for (int c = Chans.size() - 1; c >= 0; c--) {
        Chans[c]->m_dependencies_index.resize(Chans[c]->m_dependencies.size());
        for (int d = 0; d < Chans[c]->m_dependencies.size(); d++) {
            std::string Name = Chans[c]->m_dependencies[d];
            for (int c2 = 0; c2 < c; c2++) {
                if (Chans[c2]->getName() == Name) {
                    Chans[c]->m_dependencies_index[d] = c2;
                    break;
                }
            }
        }
    }

    return Chans;
}
