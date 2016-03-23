//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "filelocator.h"

using namespace std;

/*! \file implementations to read files from disk

*/

/*!
	\brief compare strings to order them

	This is a helper function to sort the filenames (or strings).

*/
bool compareNoCase(string first, string second)
{
    int i = 0;
    while ((i < first.length()) && (i < second.length())) {
        if (tolower(first[i]) < tolower(second[i]))
            return true;
        else if (tolower(first[i]) > tolower(second[i]))
            return false;
        i++;
    }

    if (first.length() < second.length())
        return true;
    else
        return false;
}

/*!
	Constructor for the FileLocator class, nothing special to be done

*/
FileLocator::FileLocator()
{
}

/*!
	\brief Function to generate a list of filenames based on the path and an extention

	This function will look on the path location and return all the filenames ending on the extention as a vector.
	\param path The path to search for files, an ending / should be ignored
	\param extension The extention of the files to be search for, included the dot (eg. ".png", ".jpg")

*/
vector<string> FileLocator::listFile(string path, string extension)
{
    /*The vector to return*/
    vector<string> files;
    DIR* pDIR;
    struct dirent* entry;
    /*Open the directory*/
    if (pDIR = opendir(path.c_str())) {
        while (entry = readdir(pDIR)) {
            /*look if the filename matches,exclude the . and ..*/
            if (strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0 && strcmp(entry->d_name + strlen(entry->d_name) - strlen(extension.c_str()), extension.c_str()) == 0) {
                if (path[strlen(path.c_str()) - 1] == '/')
                    files.push_back(string(path) + string(entry->d_name));
                else
                    files.push_back(string(path) + string("/") + string(entry->d_name));
            }
        }
        /*close the direcory*/
        closedir(pDIR);
    }
    //sort the filenames based on their alphabeticac order
    std::sort(files.begin(), files.end(), compareNoCase);

    /*return the vector with filenames*/
    return files;
}
