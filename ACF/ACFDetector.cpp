/*
 * ACFDetector.cpp
 *
 *  Created on: Jan 16, 2015
 *      Author: fds
 */

#include "ACFDetector.h"
#include <sstream>

#include "ACFFeaturePyramid.h"

void ACFDetector::getChild(const ChannelFeatures* features, int& k0, int& k, int c, int r, int s, int channelwidth, int channelheight, int modelwidth, int modelheight) const
{

    int featind = Fid[s][k];

    int channel = featind / (modelwidth * modelheight);
    int offset = featind % (modelwidth * modelheight);
    int posX = offset / modelheight;
    int posY = offset % modelheight;

    int pos = (c + posX) * channelheight + (r + posY);

    float ftr = features->getFeatureValue(channel, pos);

    k = (ftr < Thresholds[s][k]) ? 1 : 2;
    k0 = k += k0 * 2;
}

DetectionList ACFDetector::applyDetector(const cv::Mat& Frame) const
{
    DetectionList DL;
    std::vector<Detection*> Ds;

    float Upscale = 1.0;
    cv::Mat U;
    cv::resize(Frame, U, cv::Size(), Upscale, Upscale);

    ACFFeaturePyramid ACFP(U, 8, cv::Size(41, 100), *this);

    for (int L = 0; L < ACFP.getAmount(); L++) {
        std::vector<Detection*> Detections = Detect(ACFP.getLayer(L));
        if (Detections.size() > 0) {

            for (int D = 0; D < Detections.size(); D++) {

                float rescaleValue = 1.0 / ACFP.getScale(L);
                //float rescaleValue = 1/sc;
                Detections[D]->setX(Detections[D]->getX() * rescaleValue);

                Detections[D]->setY(Detections[D]->getY() * rescaleValue);
                Detections[D]->setWidth(Detections[D]->getWidth() * rescaleValue);
                Detections[D]->setHeight(Detections[D]->getHeight() * rescaleValue);

                Detections[D]->setColor(cv::Scalar(0, 0, 255));
            }

            Ds.insert(Ds.end(), Detections.begin(), Detections.end());
        }
    }

    for (int D = 0; D < Ds.size(); D++) {
        DL.addDetection(Ds[D]->getX(), Ds[D]->getY(), Ds[D]->getWidth(), Ds[D]->getHeight(), Ds[D]->getScore());
    }

    DL.resizeDetections(Upscale);

    return DL;
}

std::vector<Detection*> ACFDetector::Detect(const ChannelFeatures* features) const
{
    std::vector<Detection*> Ds;

    float cascThr = -1; //could also come from model
    int stride = shrinking;

    int treeDepth = this->ModelDepth;
    int chnWidth = features->getChannelWidth();
    int chnHeight = features->getChannelHeight();

    int nTrees = this->Fid.size();
    int nTreeNodes = this->Fid[0].size();

    // Should be kept in the model
    int modelWd = this->modelwidthpad;
    int modelHt = this->modelheightpad;

    //Height and width of the area to cover with the sliding window-detector
    int height1 = static_cast<int>(std::ceil(static_cast<float>(chnHeight * shrinking - modelHt + 1) / stride));
    int width1 = static_cast<int>(std::ceil(static_cast<float>(chnWidth * shrinking - modelWd + 1) / stride));

    int nChns = features->getnChannels();
    // apply classifier to each patch
    std::vector<int> rs, cs;
    std::vector<float> hs1;

    ///////////////////////////
    for (int c = 0; c < width1; c++) {
        for (int r = 0; r < height1; r++) {
            float h = -30;
            for (int t = 0; t < nTrees; t++) {

                int k = 0, k0 = 0;

                if (treeDepth != 0) {
                    for (int i = 0; i < treeDepth; i++) {
                        getChild(features, k0, k, c, r, t, chnWidth, chnHeight, modelWd / shrinking, modelHt / shrinking);
                    }
                    if (t == 0)
                        h = Values[t][k];
                    else
                        h += Values[t][k];
                }
                else { //variable tree depth ...

                    while (Child[t][k]) {
                        int featind = Fid[t][k];
                        int S = ((modelWd / shrinking) * (modelHt / shrinking)); //size of the model
                        int channel = featind / S;
                        int offset = featind % S;

                        int posX = offset / (modelHt / shrinking);
                        int posY = offset % (modelHt / shrinking);

                        int pos = (c + posX) * chnHeight + (r + posY);
                        float ftr = features->getFeatureValue(channel, pos);

                        k = (ftr < Thresholds[t][k]) ? 1 : 0;
                        k0 = k = Child[t][k0] - k;
                    }

                    if (t == 0)
                        h = Values[t][k];
                    else
                        h += Values[t][k];
                }
                if (h <= cascThr)
                    break;
            }

            if (h > cascThr) {
                cs.push_back(c);
                rs.push_back(r);
                hs1.push_back(h);
            }
        }
    }

    int nDets = cs.size();

    float shiftw = (this->modelwidthpad - this->modelwidth) / 2.0; // when padding is used, this should also be subtracted ...
    float shifth = (this->modelheightpad - this->modelheight) / 2.0; // "

    for (int Q = 0; Q < nDets; Q++) {
        Detection* D = new Detection();
        D->setX(cs[Q] * shrinking + shiftw);
        D->setY(rs[Q] * shrinking + shifth);
        D->setWidth(this->modelwidth);
        D->setHeight(this->modelheight);
        D->setScore(hs1[Q]);
        Ds.push_back(D);
    }

    //////////////////

    return Ds;
}

float ConvertString2float(std::string s)
{

    std::istringstream os(s);
    float d;
    os >> d;
    return d;
}

ACFDetector::ACFDetector(std::string modelfile)
{
    ReadModel(modelfile);
}

void ACFDetector::ReadModel(std::string modelfile)
{

    xml_document<> doc;
    xml_node<>* root_node;
    std::ifstream theFile(modelfile.c_str());

    if (!theFile) {
        std::cerr << "The Model-file to read does not seem to exist" << std::endl;
        exit(1);
    }

    std::vector<char> buffer((std::istreambuf_iterator<char>(theFile)), std::istreambuf_iterator<char>());
    buffer.push_back('\0');

    doc.parse<0>(&buffer[0]);
    root_node = doc.first_node("CascadeModel");

    xml_node<>* Nums = root_node->first_node("NumberWeaks");

    //Now these are floats
    this->setWidth(ConvertString2float(root_node->first_node("Width")->value()));
    this->setWidthPad(ConvertString2float(root_node->first_node("WidthPad")->value()));
    this->setHeight(ConvertString2float(root_node->first_node("Height")->value()));
    this->setHeightPad(ConvertString2float(root_node->first_node("HeightPad")->value()));

    this->ModelDepth = atoi(root_node->first_node("ModelDepth")->value());

    xml_node<>* Shri = root_node->first_node("Shrinking");
    this->shrinking = atoi(Shri->value());

    xml_node<>* Childs_node = root_node->first_node("Childs");
    for (xml_node<>* Child_node = Childs_node->first_node("Child"); Child_node; Child_node = Child_node->next_sibling()) {
        std::vector<int> Cs;
        Cs.clear();

        for (xml_node<>* Field_node = Child_node->first_node("Field"); Field_node; Field_node = Field_node->next_sibling()) {
            Cs.push_back(atof(Field_node->value()));
        }
        this->Child.push_back(Cs);
    }

    xml_node<>* Fids_node = root_node->first_node("Fids");
    for (xml_node<>* Child_node = Fids_node->first_node("Fid"); Child_node; Child_node = Child_node->next_sibling()) {
        std::vector<int> Cs;
        Cs.clear();

        for (xml_node<>* Field_node = Child_node->first_node("Field"); Field_node; Field_node = Field_node->next_sibling()) {
            int V = atoi(Field_node->value());
            Cs.push_back(V);
        }
        this->Fid.push_back(Cs);
    }

    xml_node<>* Thresh_node = root_node->first_node("Thrs");
    for (xml_node<>* Child_node = Thresh_node->first_node("threshold"); Child_node; Child_node = Child_node->next_sibling()) {
        std::vector<float> Cs;
        Cs.clear();

        for (xml_node<>* Field_node = Child_node->first_node("Field"); Field_node; Field_node = Field_node->next_sibling()) {
            Cs.push_back(ConvertString2float(Field_node->value()));
        }
        this->Thresholds.push_back(Cs);
    }
    xml_node<>* Value_node = root_node->first_node("hss");
    for (xml_node<>* Child_node = Value_node->first_node("value"); Child_node; Child_node = Child_node->next_sibling()) {
        std::vector<float> Cs;
        Cs.clear();

        for (xml_node<>* Field_node = Child_node->first_node("Field"); Field_node; Field_node = Field_node->next_sibling()) {
            Cs.push_back(ConvertString2float(Field_node->value()));
        }
        this->Values.push_back(Cs);
    }
}

ACFDetector::~ACFDetector()
{
}
