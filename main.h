#ifndef MAIN_H
#define MAIN_H

#include<opencv2/imgproc/imgproc.hpp>

#include <detectplates.h>
#include <possibleplate.h>
#include <detectchars.h>
#include <scalar.h>

#include<iostream>



//#define SHOW_STEPS            // un-comment or comment this line to show steps or not

void drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate);
void writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate);
int main(int argc, char *argv[]);

///////////////////////////////////////////////////////////////////////////////////////////////////

#endif // MAIN_H
