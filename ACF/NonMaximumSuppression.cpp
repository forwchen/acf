//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include "NonMaximumSuppression.h"

NonMaximumSuppression::NonMaximumSuppression()
{
}

NonMaximumSuppression::~NonMaximumSuppression()
{
}

DetectionList NonMaximumSuppression::standardNMS(const DetectionList& DL)
{

    DetectionList DL2;
    DL2.path = "";
    DL2.Ds = nms(DL.Ds, 0.4);

    return DL2;
}

bool sortDetFunc(Detection* i, Detection* j)
{
    return (i->getScore() > j->getScore());
}

DetectionList NonMaximumSuppression::dollarNMS(const DetectionList& DL)
{
    DetectionList DL2;
    float overlap = 0.65;

    std::vector<Detection*> FDs;
    std::vector<Detection*> Ds = DL.Ds;

    std::sort(Ds.begin(), Ds.end(), sortDetFunc);

    int n = Ds.size();
    std::vector<bool> kp(n, true);
    std::vector<float> as(n);

    //calculate areas for each detection
    for (int i = 0; i < n; i++) {
        as[i] = Ds[i]->getWidth() * Ds[i]->getHeight();
    }

    for (int i = 0; i < n; i++) {
        if (kp[i] == false) { //greedy pruning
            continue;
        }

        for (int j = i + 1; j < n; j++) {
            if (kp[j] == false) { //greedy pruning
                continue;
            }

            float iw = std::min(Ds[i]->getX() + Ds[i]->getWidth(), Ds[j]->getX() + Ds[j]->getWidth()) - std::max(Ds[i]->getX(), Ds[j]->getX());
            float ih = std::min(Ds[i]->getY() + Ds[i]->getHeight(), Ds[j]->getY() + Ds[j]->getHeight()) - std::max(Ds[i]->getY(), Ds[j]->getY());
            if (iw <= 0 || ih <= 0) {
                continue;
            }
            //area of intersection
            float o = iw * ih;

            float u = std::min(as[i], as[j]);
            if (o / u > overlap) {
                kp[j] = false;
            }
        }
    }

    for (int i = 0; i < n; i++) {
        if (kp[i] == true) {
            FDs.push_back(new Detection(Ds[i]));
        }
    }

    DL2.Ds = FDs;

    return DL2;
}
