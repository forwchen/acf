//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef IMAGE_HPP_
#define IMAGE_HPP_

#include <iostream>
#include <string>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

/*!
	class around an image to hold extra information about the image
*/
class Image {
public:
    Image(std::string path);

    Image(const cv::Mat image)
        : m_image(image)
    {
        //this->m_image = image.clone();
        this->m_name = "Camera Image";
    }

    const cv::Mat getImage() const;
    std::string getPath() const;
    int getWidth() const
    {
        return this->m_image.cols;
    }
    int getHeight() const
    {
        return this->m_image.rows;
    }

private:
    std::string m_name;
    const cv::Mat m_image;
};

#endif /* IMAGE_HPP_ */
