//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#include <iostream>
#include <vector>
#include <algorithm>
#include <stdio.h>
#include "nms.h"

using namespace std;

/*!
	A general compare function to compare elements based on the score. Used as a helper-function to sort the elements (in this case detections).

*/
bool CompareFunction(nmsHelperStruct a, nmsHelperStruct b)
{
    return (a.score < b.score);
}

/*! \brief NMS-function for limiting the detections in Dets by limiting the allowed overlap.

	This function performs non-maximum supression. It will order the detections and remove overlapping detections (the amount of allowed overlap is restricted by overlap argument) with a lower score.
	A limited allowed overlap forms a problem in situations where occlusion is common
	\param Dets A vector of detections to restruct
	\param overlap The amount of allowed overlap
	\return A vector of detections which holds to the overlap-rules

*/
vector<Detection*> nms(vector<Detection*> Dets, float overlap)
{

    vector<Detection*> pick;
    vector<int> suppress;
    float area[Dets.size()];

    if (Dets.size() == 0) {
        pick.clear();
    }
    else {

        vector<nmsHelperStruct> Temp;
        nmsHelperStruct tijd;

        for (int i = 0; i < Dets.size(); i++) {
            area[i] = (Dets[i]->getWidth()) * (Dets[i]->getHeight());
            tijd.score = Dets[i]->getScore();
            tijd.index = i;
            Temp.push_back(tijd);
        }

        sort(Temp.begin(), Temp.end(), CompareFunction);

        pick.clear();
        while (Temp.size() != 0) {
            int last = Temp.size() - 1;
            int i = Temp[last].index;

            /// Make real copy
            Detection* DD = new Detection(*(Dets[i]));
            pick.push_back(DD); //make real copy ...

            suppress.clear();
            suppress.push_back(last);
            for (int pos = 0; pos < last; pos++) {
                int j = Temp[pos].index;
                float xx1 = max(Dets[i]->getX(), Dets[j]->getX());
                float yy1 = max(Dets[i]->getY(), Dets[j]->getY());
                float xx2 = min(Dets[i]->getX() + Dets[i]->getWidth(), Dets[j]->getX() + Dets[j]->getWidth());
                float yy2 = min(Dets[i]->getY() + Dets[i]->getHeight(), Dets[j]->getY() + Dets[j]->getHeight());
                float w = xx2 - xx1 + 1;
                float h = yy2 - yy1 + 1;

                if (w > 0 && h > 0) {
                    //compute overlap
                    float o = w * h / area[j];
                    if (o > overlap) {
                        suppress.push_back(pos);
                    }
                }
            }

            sort(suppress.begin(), suppress.end());
            for (int W = 0; W < suppress.size(); W++) {
                Temp.erase(Temp.begin() + suppress[W] - W);
            }
        }
    }

    return pick;
}
