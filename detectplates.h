#ifndef DETECTPLATES_H
#define DETECTPLATES_H

#include<opencv2/imgproc/imgproc.hpp>

#include <possibleplate.h>
#include <possiblechar.h>
#include <scalar.h>
#include <preprocess.h>
#include <detectchars.h>
// global constants ///////////////////////////////////////////////////////////////////////////////
const double PLATE_WIDTH_PADDING_FACTOR = 1.3;
const double PLATE_HEIGHT_PADDING_FACTOR = 1.5;

// function prototypes ////////////////////////////////////////////////////////////////////////////
std::vector<PossiblePlate> detectPlatesInScene(cv::Mat &imgOriginalScene);

std::vector<PossibleChar> findPossibleCharsInScene(cv::Mat &imgThresh);

PossiblePlate extractPlate(cv::Mat &imgOriginal, std::vector<PossibleChar> &vectorOfMatchingChars);

#endif // DETECTPLATES_H
