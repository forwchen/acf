//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FILELOCATOR_H
#define FILELOCATOR_H

#include <stdio.h>
#include <cstdlib>
#include <iostream>
#include <string.h>
#include <fstream>
#include <dirent.h>
#include <vector>
#include <algorithm>

using namespace std;

/*!
	This class is used to read filenames of images (or other files) from disk based on the extention they have. Note, these are not the actual files but the filenames, which could be passed to an actual fileread-function. For platform independency the functionality could be changed to boost-based libraries
*/
class FileLocator {
public:
    FileLocator();
    vector<string> listFile(string path, string extension);

private:
};

#endif // FILELOCATOR_H
