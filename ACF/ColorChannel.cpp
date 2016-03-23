/*
 * ColorChannel.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */
#include "imResampleMex.hpp"
#include "rgbConvertMex.hpp"

#include "ColorChannel.h"
#include "GradMagChannel.h"
#include "GradHistChannel.h"

#include "ChannelFeatures.h"

#include "Functions.h"

cv::Mat clipImage(const cv::Mat& Im, int clip);

ColorChannel::ColorChannel(const cv::Mat& Image)
{
    //	cv::imshow("To Convert", Image);
    //	cv::waitKey(0);

    // Should check on width and height compatible with shrinking ...

    //Convert from bgr to rgb
    cv::Mat I_rgbO = Image.clone();
    cv::Mat I_rgb;
    cv::cvtColor(I_rgbO, I_rgb, CV_BGR2RGB);

    //Convert to float-image
    cv::Mat doubleImage;
    I_rgb.convertTo(doubleImage, CV_32FC3, 1 / 255.0);

    float* MImage = Convert_ToMatlab(doubleImage, 1.0f);

    //r is radius
    int r = 1;

    float RR = 12.0 / (r * (r + 2)) - 2;

    float* rgbImage = rgbConvert(MImage, Image.rows * Image.cols, Image.channels(), 2, 1.0f);
    this->setColorSpace("luv");

    float* rgbImage_smooth = (float*)malloc(sizeof(float) * Image.cols * Image.rows * I_rgb.channels());

    // Last argument was 1 for Inria-model, is 0 for CalltechModel
    convTri1(rgbImage, rgbImage_smooth, Image.rows, Image.cols, Image.channels(), RR, 1);

    free(MImage);
    free(rgbImage);
    setChanneldata(rgbImage_smooth, Image.cols, Image.rows, Image.channels());
}

void ChannelFeatures::SmoothChannels()
{
    //	std::cout << "Channels will be smoothed" << std::endl;

    for (int C = 0; C < this->Features.size(); C++) {
        //Declare new data-space voor smoothed
        float* smoothed = (float*)malloc(sizeof(float) * Channelwidth * Channelheight);
        // Smooth the data
        convTri1(Features[C], smoothed, Channelheight, Channelwidth, 1, 2, 1);

        //Switch the data to the smoothed version
        free(Features[C]);
        Features[C] = smoothed;
    }
}

ColorChannel::~ColorChannel()
{
    //delete data;
}

GradMagChannel::GradMagChannel(const ColorChannel& CC)
{
    //	std::cout << "We work with a " <<  CC.getWidth() << "x" << CC.getHeight() << " image with " << CC.getnChns() << " channels" << std::endl;

    int w = CC.getWidth();
    int h = CC.getHeight();
    int nchns = CC.getnChns();

    this->mag = (float*)malloc(sizeof(float) * w * h); // only one channel deep
    this->orientation = (float*)malloc(sizeof(float) * w * h);

    gradMag(CC.getData(), mag, orientation, h, w, nchns, 0);

    float* S = (float*)malloc(sizeof(float) * w * h);
    convTri(mag, S, h, w, 1, 5, 1);

    gradMagNorm(mag, S, h, w, 0.0050);

    //Smoothing is performed, so  delete the temporary data
    free(S);

    // Set the  magnitude as the data to use for this channel
    setChanneldata(mag, w, h, 1);
}

GradHistChannel::GradHistChannel(const GradMagChannel& GMC)
{
    int w = GMC.getWidth();
    int h = GMC.getHeight();
    int nChns = GMC.getnChns();

    if (nChns != 1) {
        std::cerr << "Number of channels should be 1 for the Magnitudes!!" << std::endl;
    }

    //Parameters, should be configurable
    int binSize = 4; //equals shrinking

    int nw = w / binSize;
    int nh = h / binSize;
    int nOrientations = 6;
    bool full = false;
    int softBin = 0;

    //It is important that this memory is set to zero to avoid random values in the result
    histogram = (float*)calloc(nw * nh * nOrientations, sizeof(float));
    ;

    gradHist(GMC.getMagnitude(), GMC.getOrientation(), histogram, h, w, binSize, nOrientations, softBin, full);

    setChanneldata(histogram, nw, nh, nOrientations);
}

void ChannelFeatures::addChannelFeatures(Channel& ch)
{
    //std::cerr << "Add channel" << std::endl;

    float* data;

    if (ch.getHeight() != this->Channelheight || ch.getWidth() != this->Channelwidth) {
        //need to resize
        data = (float*)malloc(sizeof(float) * this->Channelheight * this->Channelwidth * ch.getnChns());
        resample<float>(ch.getData(), data, ch.getHeight(), this->Channelheight, ch.getWidth(), this->Channelwidth, ch.getnChns(), 1);
        //Free the original data
        free(ch.getData());
    }
    else {
        data = ch.getData();
    }

    for (int c = 0; c < ch.getnChns(); c++) {
        float* D = (float*)malloc(sizeof(float) * this->Channelheight * this->Channelwidth);
        //copy the feature data to a new allocated
        memcpy(D, data + c * this->Channelheight * this->Channelwidth, sizeof(float) * this->Channelheight * this->Channelwidth);

        this->Features.push_back(D);
    }
    free(data);
}

ChannelFeatures* ChannelFeatures::ApproxChannel(const cv::Mat& Image, int shrinking, float scaleN, float scaleR)
{

    std::vector<float*> Feats;

    cv::Mat clippedImage = clipImage(Image, shrinking);

    int width = round(clippedImage.cols * scaleN / shrinking);
    int height = round(clippedImage.rows * scaleN / shrinking);

    float ratioLUV = pow(scaleN / scaleR, ColorChannel::getLambda());
    float ratioGradMag = pow(scaleN / scaleR, GradMagChannel::getLambda());
    float ratioGradHist = pow(scaleN / scaleR, GradHistChannel::getLambda());

    //Color channels
    for (int C = 0; C < 3; C++) {
        float* Data = (float*)malloc(sizeof(float) * height * width);
        resample<float>(Features[C], Data, this->getChannelHeight(), height, this->getChannelWidth(), width, 1, ratioLUV);
        Feats.push_back(Data);
    }

    //Gradient Magnitude
    for (int C = 3; C < 4; C++) {
        float* Data = (float*)malloc(sizeof(float) * height * width);
        resample<float>(Features[C], Data, this->getChannelHeight(), height, this->getChannelWidth(), width, 1, ratioGradMag);
        Feats.push_back(Data);
    }

    // Gradient Histograms
    for (int C = 4; C < 10; C++) {
        float* Data = (float*)malloc(sizeof(float) * height * width);
        resample<float>(Features[C], Data, this->getChannelHeight(), height, this->getChannelWidth(), width, 1, ratioGradHist);
        Feats.push_back(Data);
    }

    //	memcpy(D,data+c*this->Channelheight*this->Channelwidth,sizeof(float)*this->Channelheight*this->Channelwidth);

    ChannelFeatures* R = new ChannelFeatures(width, height, Feats);

    return R;
}
