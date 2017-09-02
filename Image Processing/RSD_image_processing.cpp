#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;

//hide the local functions in an anon namespace
namespace {
	enum Color { Red, Green, Blue, Brown, Black, White, Yellow, None };

	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int XTRANSLATION = (SCREEN_WIDTH - SCREEN_HEIGHT);
	const int GRIDSIZE = (SCREEN_HEIGHT / 3);
	const int MORTH_SIZE = 6;
	const Size MORTH = (MORTH_SIZE, MORTH_SIZE);

	int H_lower;
	int S_lower;
	int V_lower;

	int H_higher;
	int S_higher;
	int V_higher;

    //block class, represents a block in the cameras view
	class Block {
		int position;
		double xPos, yPos;
		Color clr;

	public:
		void set_values(double, double);
		double get_xPos();
		double get_yPos();
		int get_pos();
		Color get_clr();

		void set_XandY(double, double);
		void set_position(int);
		void set_clr(Color);
	};

	int Block::get_pos() { return position; }
	double Block::get_xPos() { return xPos; }
	double Block::get_yPos() { return yPos; }
	Color Block::get_clr() { return clr; }

	void Block::set_clr(Color value) { clr = value; }

	void Block::set_values(double x, double y) {
		xPos = x;
		yPos = y;
	}

	void Block::set_XandY(double x, double y) {
		xPos = x;
		yPos = y;
	}

	void Block::set_position(int value) {
		position = value;
	}

	//get the cell top left hand corner used in block area detection
	Point getCellXandY(int position) {
		//used to count up to the positon

		int counter = 0;
		for (int y = 1; y <= 3; y++) {
			for (int x = 1; x <= 3; x++) {
				counter++;
				if (position == counter)
					return Point(XTRANSLATION + (GRIDSIZE*x), GRIDSIZE*y);
			}
		}
		return NULL;
	}

	//gets a scalar value based on the enu color
	Scalar getColourScalar(Color colour) {

		switch (colour) {
		case Yellow:
			return Scalar(20, 255, 225);
		case Blue:
			return Scalar(20, 255, 225);
		case Green:
			return Scalar(20, 255, 225);
		case Red:
			return Scalar(20, 255, 225);
		default:
			return Scalar(0, 255, 225);
		}
	}

	//draw the detection grid and block colours
	void drawHUD(Mat &image, vector<Block> blks) {

		//draw the vertical lines
		for (int x = 0; x < 4; x++) {
			line(image, Point((x*GRIDSIZE) + XTRANSLATION, 0), Point((x*GRIDSIZE) + XTRANSLATION, SCREEN_HEIGHT), Scalar(20, 255, 255), 3, 8, 0);
		}
		//draw the horizontal lines
		for (int y = 0; y < 3; y++) {
			line(image, Point(XTRANSLATION, (y*GRIDSIZE + GRIDSIZE)), Point((3 * GRIDSIZE) + XTRANSLATION, (y*GRIDSIZE + GRIDSIZE)), Scalar(20, 255, 255), 3, 8, 0);
		}

		//draws colour reprentation in coresponding grid postion in HUD
		for (Block blk : blks) {
			Point startCorner = getCellXandY(blk.get_pos());
			Point endCorner = Point(startCorner.x + GRIDSIZE, startCorner.y + GRIDSIZE);
			rectangle(image, startCorner, endCorner, Scalar(0,255,255), CV_FILLED, 8, 0);
		}
	}

	//checks if the blocks are in a 3x3 grid and assings them according to 
	//the relative frame and size of the camera image. - the camera must be aligned correctely
	vector<Block> AssignPositions(vector<Block> &blocks) {
		vector<Block> result;
		
		for (Block blk : blocks) {
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 2; x++) {
					//checks where the position is in the grid based on the x and y of the shape
					if ( (blk.get_xPos() >= XTRANSLATION + x*GRIDSIZE) && (blk.get_xPos() < XTRANSLATION + (x*GRIDSIZE) +GRIDSIZE) &&
						(blk.get_yPos() >= y*GRIDSIZE) && (blk.get_yPos() < (y*GRIDSIZE) + GRIDSIZE) ) {						
						
						blk.set_position(x + y);
						result.push_back(blk);
						cout << blk.get_pos() << endl;
					}
				}
			}
		}
		return result;
	}

	//get all shapes that area greater then the area specificed
	vector<Block> getShapes(Mat image, int areaSize) {
		vector<Block> result;

		//vectors used for stroing the contours (the shape binary images - white blobs)
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//find all the shapes in the image
		findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		int i = 0;
		double area = 0.0;

		//goes through all the contours and draws all the ones larger then the threshold area
		for (i = 0; i < contours.size(); i++){
			Moments moment = moments((cv::Mat)contours[i]);
			area = contourArea(contours[i]);

			double xPos = moment.m10 / area;
			double yPos = moment.m01 / area;

			Block block;
			block.set_values(xPos, yPos);
			result.push_back(block);	
		}
		return result;
	}

	//get the binary image based on the asking colour
	Mat getBinaryImage(Mat image, Color clr) {

		Mat result;

		H_lower = 10;
		S_lower = 75;
		V_lower = 75;

		H_higher = 63;
		S_higher = 255;
		V_higher = 255;

		//default settings for each colour value - whole specturm

		/*
		int H_lower = 0;
		int S_lower = 0;
		int V_lower = 0;

		int H_higher = 179;
		int S_higher = 255;
		int V_higher = 255;

		//define colour range based on the asking colour
		if (clr == Yellow) {
			H_lower = 10;
			S_lower = 75;
			V_lower = 75;

			H_higher = 63;
			S_higher = 255;
			V_higher = 255;
			
		}
		else if (clr == Red) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (clr == Blue) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (clr == Robot) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else {
			int H_lower = 0;
			int S_lower = 0;
			int V_lower = 0;

			int H_higher = 179;
			int S_higher = 255;
			int V_higher = 255;
		}*/

		//sliders used for calibrating the colours
		/*createTrackbar("H - Lower", "Controls", &H_lower, 179);
		createTrackbar("S - Lower", "Controls", &S_lower, 255);
		createTrackbar("V - Lowre", "Controls", &V_lower, 255);

		createTrackbar("H - Upper ", "Controls", &H_higher, 179);
		createTrackbar("S - Upper", "Controls", &S_higher, 255);
		createTrackbar("V - Upper", "Controls", &V_higher, 255);*/

		//Threshold the image based on the colour
		inRange(image, Scalar(H_lower, S_lower, V_lower), Scalar(H_higher, S_higher, V_higher), result);
		return result;
	}

	//get all shapes that area greater then the area specificed
	vector<Block> getShapes(Mat image, int areaSize) {
		vector<Block> result;

		//vectors used for stroing the contours (the shape binary images - white blobs)
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//find all the shapes in the image
		findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		int i = 0;
		double area = 0.0;

		//goes through all the contours and draws all the ones larger then the threshold area
		for (i = 0; i < contours.size(); i++){
			Moments moment = moments((cv::Mat)contours[i]);
			area = contourArea(contours[i]);

			double xPos = moment.m10 / area;
			double yPos = moment.m01 / area;

			Block block;
			block.set_values(xPos, yPos);
			result.push_back(block);	
		}
		return result;
	}


	//sets the colour attribute of a shape
	void setShapeColour(Mat image, vector<Block> &blocks, Color color) {
		int area_size = 100;
		vector<Block> tempBlocks;

		//get all contours in that colour 
		tempBlocks = getShapes(image, area_size);

		for (Block block : tempBlocks) {
			block.set_clr(color);
			blocks.push_back(block);
		}
	}

	//sets the colour attribute of a shape
	bool robotCheck(Mat image) {
		int area_size = 500;

		//vectors used for stroing the contours (the shape binary images - white blobs)
		vector<vector<Point> > contours;
		vector<Vec4i> hierarchy;

		//find all the shapes in the image
		findContours(image, contours, hierarchy, CV_RETR_TREE, CV_CHAIN_APPROX_SIMPLE, Point(0, 0));
		int i = 0;
		double area = 0.0;

		//goes through all the contours and draws all the ones larger then the threshold area
		for (i = 0; i < contours.size(); i++){
			//Moments moment = moments((cv::Mat)contours[i]);
			area = contourArea(contours[i]);
			
			if (area >= area_size)
				return true;	
		}
		return false;		
	}

	//waits until the robot has moved out of the frame
	bool robotWait(VideoCapture& capture) {
		int area_size = 100;

		//window definitions
		namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("original", CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("Controls", CV_WINDOW_KEEPRATIO); //resizable window;
		createTrackbar("Area", "Controls", &area_size, 2000);

		//load picture when camera is unavailable
		//Mat frame = imread("C:\MyPic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
		Mat frame;
		Mat original;
		for (;;) {
			capture >> frame;
			capture >> original;			

			//converts the from BGR colour space to the HSV colour space      
			cvtColor(frame, frame, CV_BGR2HSV);

			//morphological opening (removes small contours from the image)
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			frame = getBinaryImage(frame, Yellow);

			if (robotCheck(frame))
				break;
		}
		return 0;
	}

	
	//looped process function - used for debugging DONT USE
	int process(VideoCapture& capture) {
		int n = 0;
		char filename[200];
		int area_size = 100;

		string window_name = "video | q or esc to quit";
		cout << "press space to save a picture. q or esc to quit" << endl;

		//window definitions
		namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("original", CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("Controls", CV_WINDOW_KEEPRATIO); //resizable window;
		createTrackbar("Area", "Controls", &area_size, 2000);

		//load picture when camera is unavailable
		//Mat frame = imread("C:\MyPic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
		Mat frame;
		Mat original;
		for (;;) {
			capture >> frame;
			capture >> original;

			if (frame.empty())
				break;

			//converts the from BGR colour space to the HSV colour space      
			cvtColor(frame, frame, CV_BGR2HSV);

			//morphological opening (removes small contours from the image)
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			vector<Block> blocks;

			//gets the binary image of each colour and then find all the 
			//contours for that given colour
			frame = getBinaryImage(frame, Yellow);

			//sets the colour of each block
			setShapeColour(frame, blocks, Yellow);
			//setShapeColour(getBinaryImage(frame, Blue), blocks, Blue);
			//setShapeColour(getBinaryImage(frame, Green), blocks, Green);
			//setShapeColour(getBinaryImage(frame, Red), blocks, Red);

			//Assign x and y positions for each block
			blocks = AssignPositions(blocks);	

			for (Block blk : blocks) {
				circle(original, Point(blk.get_xPos(), blk.get_yPos()), 50, Scalar(50, 255, 255), CV_FILLED, 8, 0);
				cout << blk.get_xPos() << endl;
			}

			drawHUD(original, blocks);

			//draw windows
			imshow(window_name, frame);
			imshow("original", original);

			char key = (char)waitKey(5); //delay N millis, usually long enough to display and capture input

			//michelles crap only shit i left
			switch (key) {
			case 'q':
			case 'Q':
			case 27: //escape key
				return 0;

			case ' ': //Save an image
				sprintf(filename, "filename%.3d.jpg", n++);
				imwrite(filename, frame);
				cout << "Saved " << filename << endl;
				break;
			default:
				break;
			}
		}
		return 0;
	}
}

vetor<Blocks> processImage(VideoCapture& capture) {
		int n = 0;
		char filename[200];
		int area_size = 100;
		vector<Block> blocks;

		//window definitions
		namedWindow(window_name, CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("original", CV_WINDOW_KEEPRATIO); //resizable window;
		namedWindow("Controls", CV_WINDOW_KEEPRATIO); //resizable window;
		createTrackbar("Area", "Controls", &area_size, 2000);

		//load picture when camera is unavailable
		//Mat frame = imread("C:\MyPic.jpg",CV_LOAD_IMAGE_GRAYSCALE);
		Mat frame;
		Mat original;

		capture >> frame;
		capture >> original;

		//converts the from BGR colour space to the HSV colour space      
		cvtColor(frame, frame, CV_BGR2HSV);

		//morphological opening (removes small contours from the image)
		erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
		dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

		//gets the binary image of each colour and then find all the 
		//contours for that given colour
		frame = getBinaryImage(frame, Yellow);

		//sets the colour of each block
		setShapeColour(frame, blocks, Yellow);
		//setShapeColour(getBinaryImage(frame, Blue), blocks, Blue);
		//setShapeColour(getBinaryImage(frame, Green), blocks, Green);
		//setShapeColour(getBinaryImage(frame, Red), blocks, Red);

		//Assign x and y positions for each block
		blocks = AssignPositions(blocks);	

		/*for (Block blk : blocks) {
			circle(original, Point(blk.get_xPos(), blk.get_yPos()), 50, Scalar(50, 255, 255), CV_FILLED, 8, 0);
			cout << blk.get_xPos() << endl;
		}*/

		//draws the grid and virtual shapes in the grid for usablity purposes
		drawHUD(original, blocks);

		//draw windows
		imshow(window_name, frame);
		imshow("original", original);

		return blocks;
	}

int main(int ac, char** av) {

	std::string arg = av[1];
	VideoCapture capture(arg); //try to open string, this will attempt to open it as a video file
	if (!capture.isOpened()) //if this fails, try to open as a video camera, through the use of an integer param
		capture.open(atoi(arg.c_str()));

	if (!capture.isOpened()) {
		cerr << "Failed to open a video device or video file!\n" << endl;
		return 1;
	}
	return process(capture);
}
