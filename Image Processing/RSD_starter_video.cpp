/*
* RSD_starter_video.cpp
*
* This code was developed for students studying RME40003 Robot Systems Design at Swinburne
* University of Technology in Melbourne, Australia
* 
* This code is largely based on the 'starter_video.cpp' code created by Ethan Rublee, Nov 23, 
* 2010 for use with OpenCV software. Ethan Rublee's code comes packaged with the OpenCV files 
* and can be found in ..\OpenCV\sources\samples\cpp\
*
* This is a simple starter sample  for using opencv, getting a video stream, displaying images,
* reading and outputting pixel values, and modifying pixel values.
* 
*/

/*
* CODE MODIFICATIONS AND BUG FIXES
* 24/07/2014 Code added by Michelle Dunn to read, output and modify pixel values
* 21/08/2014 Comments added by Michelle Dunn for Swinburne students
*/

#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;



//hide the local functions in an anon namespace
namespace {
    void help(char** av) {
        cout << "\nThis program justs gets you started reading images from video\n"
            "Usage:\n./" << av[0] << " <video device number>\n"
            << "q,Q,esc -- quit\n"
            << "space   -- save frame\n\n"
            << "\tThis is a starter sample, to get you up and going in a copy pasta fashion\n"
            << "\tThe program captures frames from a camera connected to your computer.\n"
            << "\tTo find the video device number, try ls /dev/video* \n"
            << "\tYou may also pass a video file, like my_vide.avi instead of a device number"
            << endl;
    }

    int process(VideoCapture& capture) {
        int n = 0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        cout << "press space to save a picture. q or esc to quit" << endl;
        

        //image recognition variables
        int blue_H_l = 0;
        int blue_S_l = 0;
        int blue_V_l = 0;

        int blue_H_u = 180;
        int blue_S_u = 255;
        int blue_V_u = 255;

        namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
        namedWindow("Controls", CV_WINDOW_KEEPRATIO); //resizable window;

        createTrackbar("Blue H - Lower", "Controls", &blue_H_l, 179);
        createTrackbar("Blue S - Lower", "Controls", &blue_S_l, 255);
        createTrackbar("Blue V - Lowre", "Controls", &blue_V_l, 255);

        createTrackbar("Blue H - Upper ", "Controls", &blue_H_u, 179);
        createTrackbar("Blue S - Upper", "Controls", &blue_S_u, 255);
        createTrackbar("Blue V - Upper", "Controls", &blue_V_u, 255);
        
        //Mat frame = imread("C:\MyPic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        Mat frame;
		Mat original;
        for (;;) {
            capture >> frame;
            
			capture >> original;

			if (frame.empty())
                break;
            
            
			cvtColor(frame,frame, CV_BGR2HSV);

			//morphological opening (removes small objects from the foreground)
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		
           inRange(frame, Scalar(blue_H_l, blue_S_l, blue_V_l), Scalar(blue_H_u, blue_S_u, blue_V_u), frame); //Threshold the image


            vector<vector<Point> > contours;
            vector<Vec4i> hierarchy;
            //RNG rng(12345);
            
			findContours( frame, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
			int i, j = 0;
            for( i= 0; i< contours.size(); i++ )
            {
				double contourAre = contourArea(contours[i]);
				
                //double contourArea = (contours[i], false); 
				cout << contourAre << endl;
				
				
				
                // Scalar color = Scalar( rng.uniform(0, 255), rng.uniform(0,255), rng.uniform(0,255) );
                // drawContours( drawing, contours, i, color, 2, 8, hierarchy, 0, Point() );
            }   

			//cout << "TEST" << endl;
			//cout << j << endl;

            //Calculate the moments of the thresholded image
            //Moments oMoments = moments(frame); */




           // frame = &processedFrame; 
        

            imshow(window_name, frame);
            char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input
            switch (key) {
        case 'q':
        case 'Q':
        case 27: //escape key
            return 0;

        case ' ': //Save an image
            sprintf(filename,"filename%.3d.jpg",n++);
            imwrite(filename,frame);
            cout << "Saved " << filename << endl;
            break;
        default:
            break;
            }
        }
        return 0;
    }

}

int main(int ac, char** av) {

    if (ac != 2) {
        help(av);
        return 1;
    }
    std::string arg = av[1];
    VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file

    if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
        capture.open(atoi(arg.c_str()));
    if (!capture.isOpened()) {
        cerr << "Failed to open a video device or video file!\n" << endl;
        help(av);
        return 1;
    }
    return process(capture);
}
