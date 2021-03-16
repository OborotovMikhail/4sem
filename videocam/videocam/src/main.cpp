#include <iostream>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

/*
* Example from OpenCV presentation
* 
* Uses default videocam and puts its video in a window
* Press "ESC" to close running program
*
* DONE
*/

using namespace cv;

int main()
{
	VideoCapture cap(0); // Opens default camera
	if (!cap.isOpened()) // Error message
	{
		std::cout << "Failed to open camera" << std::endl;
		return -1;
	}

	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE); // Creating window

	while (1)
	{
		Mat frame; // Creating frame
		cap >> frame; // Get a new frame from camera
		imshow("MyWindow", frame);
		if (waitKey(30) == 27)
		{
			break;
		}
	}

	// The camera will be deinitialized automatically in VideoCampture destructor

	return 0;
}