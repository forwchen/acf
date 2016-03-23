/*
 * Channel.cpp
 *
 *  Created on: Jan 15, 2015
 *      Author: fds
 */

#include "Channel.h"
#include <cstdlib>

Channel::Channel()
    : data(NULL)
    , width(0)
    , height(0)
    , nChns(0)
{
}

Channel::~Channel()
{
    /*
	if(data){
		free(data);
		data = NULL; //avoids an error on double free
	}
	*/
}

void Channel::setChanneldata(float* data, int width, int height, int nChns)
{
    if (data) {
        this->data = data;
        this->width = width;
        this->height = height;
        this->nChns = nChns;
    }
}

void Channel::PrintChanneldata() const
{

    for (int c = 0; c < this->nChns; c++) {
        for (int y = 0; y < this->height; y++) {
            for (int x = 0; x < this->width; x++) {
                std::cout << data[this->height * this->width * c + x * this->height + y] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "press key for next channel ... " << std::endl;

        std::cin.ignore();
    }
}

void Channel::PrintChanneldata(int w, int h) const
{

    for (int c = 0; c < this->nChns; c++) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                std::cout << data[this->height * this->width * c + x * this->height + y] << " ";
            }
            std::cout << std::endl;
        }
        std::cout << "press key for next channel ... " << std::endl;

        std::cin.ignore();
    }
}

int Channel::getWidth() const
{
    return this->width;
}

int Channel::getHeight() const
{
    return this->height;
}
int Channel::getnChns() const
{
    return this->nChns;
}

float* Channel::getData() const
{
    return this->data;
}
