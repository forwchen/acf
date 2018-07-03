## [NOTE]

This repo is a part of: https://bitbucket.org/fdesmedt/openframework/overview

I made some modifications to it for my own use.
And I used it a long time ago, **code for training the model is lost** ...

## [Usage]
Compile with the standard cmake and make procedure

Detect person in image and crop them

    ./ped_acf path/to/image/dir output/path

The arguments must be directories, not individual files

## [Citations]
*  Open Framework for combined pedestrian detection
    F. De Smedt and T. Goedemé, VISAPP 2015, Berlin

*  On-board real-time tracking of pedestrians on a UAV
   F. De Smedt, D. Hulens and T. Goedemé, EVW 2015, Boston, USA

## [Common Bugs]
You may need to replace cv::vector with std::vector if using OpenCV 3.
It may also be necessary to add "using namespace std;" into the gpu.hpp file inside OpenCV.
