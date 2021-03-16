#include <iostream>
#include <opencv/cv.hpp>
#include <opencv/highgui.h>

using namespace cv;

int main()
{
	Mat image;
	image = imread("donov.jpg", CV_LOAD_IMAGE_COLOR); // Read the file

	if (!image.data)
	{
		std::cout << "Could not open or find the image" << std::endl;
		return -1;
	}

	namedWindow("Display window", WINDOW_AUTOSIZE); // Create a window for display
	imshow("Display window", image); // Show our image inside it

	waitKey(0); // Wait for the keystroke in the window
	return 0;
}