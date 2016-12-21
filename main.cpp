#include <QCoreApplication>

#include <main.h>

void drawRedRectangleAroundPlate(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point2f p2fRectPoints[4];

    // get 4 vertices of rotated rect
    licPlate.rrLocationOfPlateInScene.points(p2fRectPoints);

    // draw 4 red lines
    for (int i = 0; i < 4; i++) {
        cv::line(imgOriginalScene, p2fRectPoints[i], p2fRectPoints[(i + 1) % 4], SCALAR_RED, 2);
    }
}

void writeLicensePlateCharsOnImage(cv::Mat &imgOriginalScene, PossiblePlate &licPlate) {
    cv::Point ptCenterOfTextArea;                   // this will be the center of the area the text will be written to
    cv::Point ptLowerLeftTextOrigin;                // this will be the bottom left of the area that the text will be written to

    int intFontFace = CV_FONT_HERSHEY_SIMPLEX;                              // choose a plain jane font
    double dblFontScale = (double)licPlate.imgPlate.rows / 30.0;            // base font scale on height of plate area
    int intFontThickness = (int)std::round(dblFontScale * 1.5);             // base font thickness on font scale
    int intBaseline = 0;

    cv::Size textSize = cv::getTextSize(licPlate.strChars, intFontFace, dblFontScale, intFontThickness, &intBaseline);      // call getTextSize

    ptCenterOfTextArea.x = (int)licPlate.rrLocationOfPlateInScene.center.x;         // the horizontal location of the text area is the same as the plate

    if (licPlate.rrLocationOfPlateInScene.center.y < (imgOriginalScene.rows * 0.75)) {      // if the license plate is in the upper 3/4 of the image
                                                                                            // write the chars in below the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) + (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    } else {                                                                                // else if the license plate is in the lower 1/4 of the image
                                                                                            // write the chars in above the plate
        ptCenterOfTextArea.y = (int)std::round(licPlate.rrLocationOfPlateInScene.center.y) - (int)std::round((double)licPlate.imgPlate.rows * 1.6);
    }

    // calculate the lower left origin of the text area
    ptLowerLeftTextOrigin.x = (int)(ptCenterOfTextArea.x - (textSize.width / 2));
    // based on the text area center, width, and height
    ptLowerLeftTextOrigin.y = (int)(ptCenterOfTextArea.y + (textSize.height / 2));

    // write the text on the image
    cv::putText(imgOriginalScene, licPlate.strChars, ptLowerLeftTextOrigin, intFontFace, dblFontScale, SCALAR_YELLOW, intFontThickness);
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    if (argc < 2) {
        std::cout << "Usage ./this_app image_file_name" << std::endl;
        return(0);
    }

    // attempt KNN training
    bool blnKNNTrainingSuccessful = loadKNNDataAndTrainKNN();

    if (blnKNNTrainingSuccessful == false) {                    // if KNN training was not successful
        std::cout << std::endl << std::endl << "error: error: KNN traning was not successful" << std::endl << std::endl;
        return(0);                                              // exit
    }

    // input image
    cv::Mat imgOriginalScene;
    imgOriginalScene = cv::imread(argv[1]);

    if (imgOriginalScene.empty()) {                             // if unable to open image
        std::cout << "error: image not read from file\n\n";     // show error message on command line
        return(0);                                              // exit
    }

    // detect plates
    std::vector<PossiblePlate> vectorOfPossiblePlates = detectPlatesInScene(imgOriginalScene);
    // detect chars in plates
    vectorOfPossiblePlates = detectCharsInPlates(vectorOfPossiblePlates);
    // show scene image
    cv::imshow("imgOriginalScene", imgOriginalScene);

    if (vectorOfPossiblePlates.empty()) {                                               // if no plates were found
        std::cout << std::endl << "no license plates were detected" << std::endl;       // inform user no plates were found
        return(0);
    }

    // if we get in here vector of possible plates has at leat one plate
    // sort the vector of possible plates in DESCENDING order (most number of chars to least number of chars)
    std::sort(vectorOfPossiblePlates.begin(), vectorOfPossiblePlates.end(), PossiblePlate::sortDescendingByNumberOfChars);
    // suppose the plate with the most recognized chars (the first plate in sorted by string length descending order) is the actual plate
    PossiblePlate licPlate = vectorOfPossiblePlates.front();
    // show crop of plate and threshold of plate
    cv::imshow("imgPlate", licPlate.imgPlate);
    cv::imshow("imgThresh", licPlate.imgThresh);

    if (licPlate.strChars.length() == 0) {                                                      // if no chars were found in the plate
        std::cout << std::endl << "no characters were detected" << std::endl << std::endl;      // show message
        return(0);                                                                              // and exit program
    }

    // draw red rectangle around plate
    drawRedRectangleAroundPlate(imgOriginalScene, licPlate);
    std::cout << std::endl << "license plate read from image = " << licPlate.strChars << std::endl;     // write license plate text to std out
    std::cout << std::endl << "-----------------------------------------" << std::endl;
    // write license plate text on the image
    writeLicensePlateCharsOnImage(imgOriginalScene, licPlate);
    // re-show scene image
    cv::imshow("imgOriginalScene", imgOriginalScene);
    // write image out to file
    cv::imwrite("imgOriginalScene.png", imgOriginalScene);
    // hold windows open until user presses a key
    cv::waitKey(0);

    return a.exec();
}
