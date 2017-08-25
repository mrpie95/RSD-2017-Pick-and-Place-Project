#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;



//hide the local functions in an anon namespace
namespace {
	const SCREEN_WIDTH 640;
	const SCREEN_HEIGHT 480;
	const XTRANSLATION (SCREEN_WIDTH - SCREEN_HEIGHT);
	const GRIDSIZE (SCREEN_WIDTH/3);


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

	class Block
	{
		int xPos, yPos, position;
		std::string colour;

	public:
		void set_XandY(int, int);
		void set_colour(std::string);
		void set_position(int);


	void Block::set_XandY(int x, int y){
		xPos = x;
		yPos = y;
	}

	void Block::set_colour(std::string col) {
		colour = col;
	}

	void Block::set_position(int pos)
	{
		position = pos;
	}

	void drawHUD(Mat &image) {
		//draw the vertical lines
		for (int x = 0; x<2; y++ ){
					line(image, Point(x*XTRANSLATION, 0) PointPoint(x*XTRANSLATION, GRIDSIZE), Scalar(20, 255, 255), 3, 8, 0)
		}
		//draw the horizontal lines
		for (int y = 0; y<2; y++ ){
					line(image, Point(y*XTRANSLATION, GRIDSIZE) PointPoint(y*XTRANSLATION=GRIDSIZE, GRIDSIZE), Scalar(20, 255, 255), 3, 8, 0)
		}
	}

	//checks if the blocks are in a 3x3 grid and assings them according to 
	//the relative frame and size of the camera image. - the camera must be aligned correctely
	void AssignPositions(vector<Blocks> &blocks){
		for (Block &blk : blocks){
			for (int y = 0; y<2; y++ ){
				for (int x = 0; x<2; x++ ){ 
					if ((blk.xPos>=XTRANSLATION+(x*GRIDSIZE))&&(blk.xPos<XTRANSLATION+(x*GRIDSIZE))&&
			    		(blk.y*yPos>=y)&&(blk.yPos<XTRANSLATION+(y*GRIDSIZE))){
						blk.position = x+y;
					}
				}
			}
		}
	}

	vector<Block> getShapes(Mat image,int areaSize) {
		
		vector<Block> result;

		//vectors used for stroing the contours (the shape binary images - white blobs)
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;
          
		//find all the shapes in the image
		findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0) );
		int i, area = 0;

		//goes through all the contours and draws all the ones larger then the threshold area
		for( i= 0; i< contours.size(); i++ )
		{
				
			Moments moment = moments((cv::Mat)contours[i]);
			area = contourArea(contours[i]);

			int xPos = moment.m10/area;
			int yPos = moment.m01/area;

			Block block;
			block.set_values(xPos, yPos, "Yellow");
			result.push_back(block);

			//if (area > areaSize)
				//circle(original, Point(xPos, yPos), 50, Scalar(255, 255, 255), CV_FILLED, 8, 0);					
		}   
		return result;
	}

	//get the binary image based on the asking colour
	Mat getBinaryImage(Mat image, String colour) {

		Mat result;

		//default settings for each colour value - whole specturm
		int H_lower = 0;
		int S_lower = 0;
		int V_lower = 0;

		int H_higher = 179;
		int S_higher = 255;
		int V_higher = 255;

		//define colour range based on the asking colour
		if (colour == "Yellow") {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (colour == "Red") {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (colour == "Blue") {

		}
		else {

		}

		/*//sliders used for calibrating the colours
        createTrackbar("Blue H - Lower", "Controls", &blue_H_l, 179);
        createTrackbar("Blue S - Lower", "Controls", &blue_S_l, 255);
        createTrackbar("Blue V - Lowre", "Controls", &blue_V_l, 255);

        createTrackbar("Blue H - Upper ", "Controls", &blue_H_u, 179);
        createTrackbar("Blue S - Upper", "Controls", &blue_S_u, 255);
        createTrackbar("Blue V - Upper", "Controls", &blue_V_u, 255);*/

		//Threshold the image based on the colour
		inRange(image, Scalar(H_lower,S_lower,V_lower), Scalar(H_higher,S_higher,V_higher), result); 
		return result;
	}


    int process(VideoCapture& capture) {
        int n = 0;
        char filename[200];
        string window_name = "video | q or esc to quit";
        cout << "press space to save a picture. q or esc to quit" << endl;

		double area_size = 100;

		//window definitions
        namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("original", CV_WINDOW_KEEPRATIO); //resizable window;
        namedWindow("Controls", CV_WINDOW_KEEPRATIO); //resizable window;
		createTrackbar("Area", "Controls", &area_size, 2000);
        
        //Mat frame = imread("C:\MyPic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
        Mat frame;
		Mat original;
        for (;;) {
            capture >> frame;
			capture >> original;

			if (frame.empty())
                break;

			//converts the from BGR colour space to the HSV colour space      
			cvtColor(frame,frame, CV_BGR2HSV);

			//morphological opening (removes small contours from the image)
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//list of images one for each colour being detected
			vector<Mat> images;
			images.push_back(getBinaryImage(frame, "yellow"));
			images.push_back(getBinaryImage(frame, "blue"));
			images.push_back(getBinaryImage(frame, "green"));
			images.push_back(getBinaryImage(frame, "red"));
			images.push_back(getBinaryImage(frame, "other"));
			
			//list of blocks
			vector<Block> blocks;

			//find all blocks in all images
			for (Mat image : images){
				blocks.push_back(getShapes(image, area_size));
			}

			//Assign Positions
			AssignPositions(blocks);

			/*for (vector<Mat>::const_iterator it = images.begin(); it != images.end(); ++it) {
					
				blocks.push_back(getShapes());
				i++;
			}*/
			//cout << *it << endl;

			drawHUD(frame);

            imshow(window_name, frame);
			imshow("original", original);

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
