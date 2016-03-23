//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "FileWriter.h"

using namespace std;

typedef std::string str; // string
const str e0 = ">\n";
const str e1 = "/>\n";

template <typename T>
str attr(const str& key, const T& value, const str unit = "")
{
    std::stringstream ss;
    ss << key << "=\"" << value << unit << "\" ";
    return ss.str();
}
str st(const str& name)
{
    return "\t<" + name + " ";
}
str ed(const str& name)
{
    return "</" + name + ">\n";
}

/*!
        \author F. De Smedt
        \date 2014
*/
FileWriter::FileWriter(std::string filename)
{
    /*Open the file*/
    m_file.open(filename.c_str());

    /*Write header of file*/
    /*
    m_file << "<?xml version=\"1.0\"?>" << std::endl;
    m_file << "<Detections>" << std::endl;
    */
    xml << "<?xml version=\"1.0\" encoding=\"utf-8\"?>\n";
    xml << st("dataset") << attr("name", "campus") << e0;
    first = true;
    pframe = -1;
}

void FileWriter::writeDetection(int frameID, Detection* det)
{

    if (frameID != pframe) {
        if (!first) {
            xml << ed("objectlist") << ed("frame");
        }
        if (first)
            first = false;
        xml << st("frame") << attr("number", frameID) << e0;
        xml << st("objectlist") << e0;
    }

    if (det != NULL) {
        xml << st("object") << attr("confidence", det->getScore()) << e0;
        xml << st("box")
            << attr("h", det->getHeight())
            << attr("w", det->getWidth())
            << attr("xc", det->getX() + det->getWidth() / 2)
            << attr("yc", det->getY() + det->getHeight() / 2) << e1;
        xml << ed("object");
    }
    pframe = frameID;
}

FileWriter::~FileWriter()
{
    xml << ed("objectlist") << ed("frame");
    xml << ed("dataset");

    m_file << xml.str();
    m_file.close();
}
