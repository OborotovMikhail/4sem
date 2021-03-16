#include <iostream>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

/*
* Task from OpenCV presentation
*
* Detects faces on a picture and puts a rectangle around them
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

	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display
	imshow("Display window", frame); // Show image inside window
	waitKey(0);
}

int main()
{
	// Loading cascades for face recognition
	if (!face_cascade.load("haarcascades/haarcascade_frontalface_alt.xml"))
	{
		printf("Error loading cascades for face recognition\n");
		return -1;
	}
	
	Mat image = imread("donov.jpg", CV_LOAD_IMAGE_COLOR); // Reading image from file
	detectAndDisplay(image);

	return 0;
}