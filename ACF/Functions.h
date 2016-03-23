//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef _H_FUNC
#define _H_FUNC

#include <iostream>

#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/objdetect/objdetect.hpp>
#include <opencv2/highgui/highgui.hpp>

// Some general functions which are not yet added to the separate classes

float* Convert_ToMatlab(const cv::Mat& image, float scale);

cv::Mat Convert_ToOCV(float* data, int channels, int width, int height, float scale);
void fhog(float* M, float* O, float* H, int h, int w, int binSize, int nOrients, int softBin, float clip);
void convTri(float* I, float* O, int h, int w, int d, int r, int s);
void convTri1(float* I, float* O, int h, int w, int d, float p, int s);

void grad2(float* I, float* Gx, float* Gy, int h, int w, int d);
void gradMag(float* I, float* M, float* O, int h, int w, int d, bool full);
void gradHist(float* M, float* O, float* H, int h, int w,
    int bin, int nOrients, int softBin, bool full);
void hog(float* M, float* O, float* H, int h, int w, int binSize,
    int nOrients, int softBin, bool full, float clip);
void gradMagNorm(float* M, float* S, int h, int w, float norm);
#endif
