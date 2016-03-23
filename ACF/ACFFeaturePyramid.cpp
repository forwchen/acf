/*
 * ACFFeaturePyramid.cpp
 *
 *  Created on: Feb 3, 2015
 *      Author: fds
 */

#include "ACFFeaturePyramid.h"
#include <cmath>
//#include <ctgmath>

ACFFeaturePyramid::ACFFeaturePyramid(const cv::Mat& Image, int scalesPerOct, cv::Size minSize, const ACFDetector& ADet)
    : scalesPerOct(scalesPerOct)
    , minSize(minSize)
{

    this->approxAmount = scalesPerOct - 1;
    //	this->approxAmount = 0;
    int nOctUp = 0; //Should be one for evaluation (boils down to do a scale-up of the image)

    int nScales = std::floor(static_cast<float>(scalesPerOct * (nOctUp + log2f(std::min((float)Image.cols / minSize.width, (float)Image.rows / minSize.height))) + 1));

    //set the size of the feature-pyramid at the nScales amount
    Layers.resize(nScales);

    //std::vector<float> scales;
    std::vector<int> approxReal(nScales); // to store the layer to use for approximation...

    // Generate scales
    for (int s = 0; s < nScales; s++) {
        float sc = std::pow(2.0, (static_cast<float>(-s) / scalesPerOct + nOctUp));
        scales.push_back(sc);
    }

    float sz1 = Image.rows;
    float sz2 = Image.cols;

    //std::cout << sz1 << "  " << sz2 << std::endl;
    float d0, d1;

    if (sz1 < sz2) {
        d0 = sz1;
        d1 = sz2;
    }
    else {
        d0 = sz2;
        d1 = sz1;
    }

    // Should come from model ...
    float shrink = ADet.getShrinking();
    float eps = 2.2204e-16;

    //for(int i=0;i<nScales;i++){
    for (int i = 0; i < 1; i++) {
        float s = scales[i];
        float s0 = (round((float)d0 * s / shrink) * shrink - 0.25 * shrink) / d0;
        float s1 = (round((float)d0 * s / shrink) * shrink + 0.25 * shrink) / d0;

        //std::cout << d0 << " " << s0 << "   " << s1 << std::endl;
        std::vector<double> ss, sst;
        for (double sss = 0; sss <= 1 - eps; sss += 0.01) {
            float stemp = sss * (s1 - s0) + s0;
            ss.push_back(stemp);
            float es0 = stemp * d0;
            float es0T = es0 - round(es0 / shrink) * shrink;
            if (es0T < 0)
                es0 = -es0T;
            else
                es0 = es0T;

            /*
		std::cout << es0-round(es0/shrink)*shrink << std::endl;
		std::cout << "es0X: " << round(es0/shrink)*shrink << std::endl;
		std::cout << "es0: " << es0 << std::endl;
		std::cout << "es0T: " << es0T << std::endl;
		*/
            float es1 = d1 * stemp;
            float es1T = es1 - round(es1 / shrink) * shrink;

            if (es1T < 0)
                es1 = -es1T;
            else
                es1 = es1T;

            //save the max of both
            sst.push_back(std::max(es0, es1));
        }
        //	std::cout << "Size: " << ss.size() << std::endl;
        //	for(int iGG=0;iGG<ss.size();iGG++)
        //		std::cout << "SS: " << ss[iGG] << std::endl;

        // find the minimum
        int index = 0;
        float val = sst[0];
        for (int iGG = 1; iGG < sst.size(); iGG++) {
            if (sst[iGG] < val) {
                val = sst[iGG];
                index = iGG;
            }
        }
        scales[i] = ss[index];
        //std::cout << index << ": " << ss[index] << std::endl;
        //	float ss=(0:.01:1-eps)*(s1-s0)+s0;
    }

    std::vector<bool> kp(scales.size());
    for (int i = 0; i < scales.size() - 1; i++) {
        if (scales[i] == scales[i + 1])
            kp[i] = false;
        else
            kp[i] = true;
    }
    kp[scales.size() - 1] = true;

    //prune when scale already exists
    for (int i = scales.size() - 1; i >= 0; i--) {
        if (!kp[i])
            scales.erase(scales.begin() + i);
    }

    std::vector<float> scalesh, scalesw;

    for (int i = 0; i < scales.size(); i++) {
        float h = round(sz1 * scales[i] / shrink) * shrink / sz1;
        float w = round(sz2 * scales[i] / shrink) * shrink / sz2;

        scalesh.push_back(h);
        scalesw.push_back(w);
    }

    /*
for(int i=0;i<scales.size();i++){
	std::cout << scalesh[i] << "  " << scalesw[i] << std::endl;
}
*/
    // End of getScales

    std::vector<int> isR;
    for (int i = 1; i <= nScales; i += approxAmount + 1) {
        isR.push_back(i);
    }

    std::vector<int> isA(nScales);
    for (int i = 1; i <= nScales; i++) {
        isA[i - 1] = i;
    }
    /*
for(int i=0;i<isA.size();i++)
	std::cout << "B: " << isA[i] << std::endl;
*/

    for (int i = 0; i < isR.size(); i++)
        isA[isR[i] - 1] = -1;

    std::vector<int> j;
    j.push_back(0);
    for (int i = 0; i < isR.size() - 1; i++) {
        j.push_back(std::floor((isR[i] + isR[i + 1]) / 2.0));
    }
    j.push_back(nScales);

    /*
for(int QQ=0;QQ<j.size();QQ++)
		std::cout << j[QQ] << std::endl;
*/

    std::vector<int> isN(nScales);
    for (int i = 1; i <= nScales; i++) {
        isN[i - 1] = i;
    }

    for (int i = 0; i < isR.size(); i++) {
        for (int Q = j[i] + 1; Q <= j[i + 1]; Q++)
            isN[Q - 1] = isR[i];
    }

    //Real Scales
    for (int i = 0; i < isR.size(); i++) {
        int ii = isR[i] - 1; //go to C-indexing

        float sc = scales[ii];
        float w1 = round(sz2 * sc / shrink) * shrink;
        float h1 = round(sz1 * sc / shrink) * shrink;

        // Possible change to imResample as in original implementation
        cv::Mat Scaled;
        cv::resize(Image, Scaled, cv::Size(w1, h1));

        Layers[ii] = new ChannelFeatures(Scaled, ADet.getShrinking());
        //std::cout << "Real filled in: " << ii << std::endl;
    }

    // Approx scales
    /*
for(int i=0;i<isA.size();i++){

//	std::cout << isA[i] << std::endl;


	int ii = isA[i]-1;

	if(ii < 0)
		continue; //covered by real ...
	//int ii = isA[i]-1;
	//int iR = 0; //index of the real layer to approximate from
	int iR = isN[ii]-1; //index of the real layer to approximate from

	std::cout << "Indeces for approximation: " << ii << "   " << iR << std::endl;

	Layers[ii] = Layers[iR]->ApproxChannel(Image, ADet.getShrinking(),scales[ii],scales[iR]);

}
*/

    for (int i = 0; i < Layers.size(); i++) {
        //	std::cout << "isA: " << isA[i] << std::endl;
        if (isA[i] == -1) //Real layer
            continue;

        int iR = isN[i] - 1;
        //	std::cout << "Approximating: " << i << " from  " << iR << std::endl;
        Layers[i] = Layers[iR]->ApproxChannel(Image, ADet.getShrinking(), scales[i], scales[iR]);
    }

    //exit(0);

    /////// OLD
    /*
for(int s=0;s<nScales;s++){
	float sc = std::pow(2.0,(static_cast<float>(-s)/scalesPerOct+nOctUp));
	scales.push_back(sc);
	std::cout << "Scale: " << sc << std::endl;

	//to store the rescaled image;
	cv::Mat Scaled;

	if(s%scalesPerOct == 0){ //we should calculate the real one

		approxReal[s] = -1; //set this layer as to NOT approximate

		//Rescale the image
		if(s != 0)
			cv::resize(Image,Scaled,cv::Size(),sc,sc);
		else
			Scaled = Image.clone();

		Layers[s] = new ChannelFeatures(Scaled, ADet.getShrinking());

	}
	else{
		//store the number of the layer to use for approximation
		approxReal[s] = std::floor(static_cast<float>(s)/scalesPerOct)*scalesPerOct;

	}

}
*/

    /*
for(int l=0;l<approxReal.size();l++){
	if(approxReal[l] != -1){
//		std::cout << "Need to approx from layer " << approxReal[l] << std::endl;
		Layers[l] = Layers[approxReal[l]]->ApproxChannel(Image, ADet.getShrinking(),scales[l],scales[approxReal[l]]);

	}
}
*/

    //Smoothing
    for (int l = 0; l < Layers.size(); l++) {
        Layers[l]->SmoothChannels();
    }
}

// Constructor for single scale ...
ACFFeaturePyramid::ACFFeaturePyramid(const cv::Mat& Image, float sc, cv::Size minSize, const ACFDetector& ADet)
    : scalesPerOct(scalesPerOct)
    , minSize(minSize)
{
    this->approxAmount = 0;

    std::cout << "SINGLE SCALE" << std::endl;

    int nScales = 1;

    //set the size of the feature-pyramid at the nScales amount
    Layers.resize(nScales);

    for (int s = 0; s < nScales; s++) {
        //	float sc = std::pow(2.0,-(static_cast<float>(s)/scalesPerOct));
        scales.push_back(sc);
        std::cout << "Single Scale: " << sc << std::endl;

        //to store the rescaled image;
        cv::Mat Scaled;

        //Rescale the image
        if (sc != 1.0)
            cv::resize(Image, Scaled, cv::Size(), sc, sc);
        else
            Scaled = Image.clone();

        Layers[s] = new ChannelFeatures(Scaled, ADet.getShrinking());
    }

    //Smoothing
    for (int l = 0; l < Layers.size(); l++) {
        Layers[l]->SmoothChannels();
    }
}

ACFFeaturePyramid::~ACFFeaturePyramid()
{
    // TODO Auto-generated destructor stub
    for (int l = 0; l < Layers.size(); l++)
        delete Layers[l];
}
