//============================================================================
// Author      : F. De Smedt @ EAVISE
// Copyright   : This code is written for the publication of  "Open Framework for Combined Pedestrian detection". This code is for research and educational purposes only. For a different license, please contact the contributors directly. More information about the license can be fount in license.txt
//============================================================================

#ifndef FEATURE_H
#define FEATURE_H

/*!
	The feature class represent one single feature, which is basically a square in a specific channel.
*/
class Feature {
private:
    int m_x, m_y, m_width, m_height;
    int m_channel;

public:
    Feature();
    void setX(int x);
    void setY(int y);
    void setWidth(int width);
    void setHeight(int height);
    void setChannel(int channel);

    int getX() const;
    int getY() const;
    int getWidth() const;
    int getHeight() const;
    int getChannel() const;
};

#endif // FEATURE_H
