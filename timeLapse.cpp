/* boneCV.cpp
 *
 * Copyright Derek Molloy, School of Electronic Engineering, Dublin City University
 * www.derekmolloy.ie
 *
 * Redistribution and use in source and binary forms, with or without modification, are permitted
 * provided that source code redistributions retain this notice.
 *
 * This software is provided AS IS and it comes with no warranties of any type.
 */


/*
* to Compile
* Run this
* g++ -O2 -Wall -c boneCV.cpp -lopencv_core -lopencv_imgproc -lopencv_objdetect -lopencv_highgui -o boneCV
*
* or
* cmake .
* make
*/

#include <string>
#include <iostream>

//#include <pthread.h>
//#include <time.h>
//#include <unistd.h>

#include <ctime>
#include <iomanip>
#include <iomanip>
#include <chrono>

#include <future>
#include <thread>


#include <opencv2/opencv.hpp>

using namespace std;
using namespace cv;

// function declaration
void TakeImage();

int main()
{
    std::chrono::seconds dura( 60 );

    for(;;)
    {
	    auto handle = std::async(std::launch::async, TakeImage);

	    //take image every minute
        std::this_thread::sleep_for(dura);
    }

    return 0;
}

void TakeImage()
{
    string filePrefix = std::string("./images/capture");
    string fileSufix = std::string(".png");

    char buf[80];
    std::time_t timeNow = std::time(nullptr);

    std::strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", std::localtime(&timeNow));
    string fileName = filePrefix + buf + fileSufix;

    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&timeNow));
    string displayText = std::string(buf);

    cout << "Taking Image: " + fileName << endl;

	cv::Mat frame;
	VideoCapture capture(1);
    //capture.set(CV_CAP_PROP_FRAME_WIDTH,1920);
    //capture.set(CV_CAP_PROP_FRAME_HEIGHT,1080);
    //cout << "width: " << capture.get(CV_CAP_PROP_FRAME_WIDTH) << endl;
    //cout << "height: " << capture.get(CV_CAP_PROP_FRAME_HEIGHT) << endl;

    if(!capture.isOpened()){
	    cout << "Failed to connect to the camera." << endl;
	    return;
    }

    // it takes few frames for the camera to worm up and get the proper contrast
    std::chrono::seconds dura(1);

    capture >> frame;
    std::this_thread::sleep_for(dura);
    capture >> frame;
    std::this_thread::sleep_for(dura);
    capture >> frame;
    std::this_thread::sleep_for(dura);
    capture >> frame;
    std::this_thread::sleep_for(dura);

    capture >> frame;

    if(frame.empty()){
		cout << "Failed to capture an image" << endl;
		return;
    }

    int fontFace = FONT_HERSHEY_PLAIN;
    double fontScale = 1;
    int thickness = 1.5;
    int baseline = 0;
    Size textSize = getTextSize(displayText, fontFace, fontScale, thickness, &baseline);
    baseline += thickness;

    Scalar fontColor = Scalar::all(200);
    Scalar boxColor = Scalar::all(0);
    // center the text
    Point textOrg(10, (frame.rows - 10 ));
    Point pt1 = textOrg; // + Point(0, baseline);
    Point pt2 = textOrg + Point(textSize.width, -textSize.height);

    rectangle(frame, pt1, pt2, boxColor, CV_FILLED);

    putText(frame, displayText, textOrg, fontFace, fontScale, fontColor, thickness);

    imwrite(fileName, frame);
}
