#include <iostream>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

/*
* Task from OpenCV presentation
* 
* Detects faces on a video and puts a rectangle around them
* Uses default camera
* Press "ESC" to close running program
*
* DONE
*/

using namespace cv;

CascadeClassifier face_cascade;

void detectAndDisplay(Mat& frame) {
	std::vector<Rect> faces;
	Mat frame_gray;
	cvtColor(frame, frame_gray, COLOR_BGR2GRAY);
	equalizeHist(frame_gray, frame_gray);

	// Detecting faces
	face_cascade.detectMultiScale(frame_gray, faces, 1.1, 2, 0 | CASCADE_SCALE_IMAGE, Size(30, 30));

	// Iterate through all current elements (detected faces)
	for (const auto& f : faces)
	{
		Point pt1(f.x, f.y);
		Point pt2(f.x + f.height, f.y + f.width);
		rectangle(frame, pt1, pt2, Scalar(0, 255, 0), 2, 8, 0);
	}
}

int main()
{
	// Loading cascade for face recognition
	if (!face_cascade.load("haarcascades/haarcascade_frontalface_alt.xml"))
	{
		printf("Error loading cascade for face recognition\n");
		return -1;
	}

	// Opening default camera device
	VideoCapture cap(0);
	if (!cap.isOpened())
	{
		std::cout << "Failed to open camera" << std::endl;
		return -1;
	}

	// Creating window
	namedWindow("MyWindow", CV_WINDOW_AUTOSIZE);

	while (1)
	{
		Mat frame; // Creating frame
		cap >> frame; // Putting camera picture into frame
		detectAndDisplay(frame); // Detecting faces
		imshow("MyWindow", frame); // Putting frame inside window

		// Checking for "ESC" key
		if (waitKey(30) == 27)
		{
			break;
		}
	}

	return 0;
}