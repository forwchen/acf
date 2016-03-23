#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

float* Convert_ToMatlab(const cv::Mat& image, float scale)
{
    int channels = image.channels();
    int width = image.cols;
    int height = image.rows;

    float* Image_data = (float*)malloc(sizeof(float) * channels * width * height);

    for (int x = 0; x < width; x++) {
        for (int y = 0; y < height; y++) {
            if (channels == 3) {
                cv::Vec3f intensity = image.at<cv::Vec3f>(y, x);
                float blue = intensity.val[2] * scale;
                float green = intensity.val[1] * scale;
                float red = intensity.val[0] * scale;
                Image_data[0 * width * height + x * height + y] = red;
                Image_data[1 * width * height + x * height + y] = green;
                Image_data[2 * width * height + x * height + y] = blue;
            }
            else if (channels == 1) {
                float intensity = image.at<float>(y, x);
                Image_data[x * height + y] = intensity * scale;
            }
            else {
                std::cout << "Other as 3 or 1 channels not yet supported!!! so " << channels << "is NOT ok" << std::endl;
            }
        }
    }

    return Image_data;
}

cv::Mat Convert_ToOCV(float* data, int channels, int width, int height, float scale)
{

    cv::Mat Image;
    if (channels == 3)
        Image.create(height, width, CV_32FC3);
    else {
        if (channels == 1) {
            Image.create(height, width, CV_32FC1);
        }
        else {
            std::cout << "In converting to OCV is only 1 and 3 supported!!" << std::endl;
            exit(1);
        }
    }

    if (channels == 3) {
        exit(3);
    }
    else {
        for (int x = 0; x < width; x++) {
            for (int y = 0; y < height; y++) {
                Image.at<float>(y, x) = data[x * height + y];
            }
        }
    }

    return Image;
}
