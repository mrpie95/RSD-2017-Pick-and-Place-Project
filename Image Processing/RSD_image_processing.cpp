#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace cv;
using namespace std;


enum Colour { Red, Green, Blue, Brown, Black, White, Yellow, Blue, None };
//hide the local functions in an anon namespace
namespace {
	const int SCREEN_WIDTH = 640;
	const int SCREEN_HEIGHT = 480;
	const int XTRANSLATION = (SCREEN_WIDTH - SCREEN_HEIGHT);
	const int GRIDSIZE = (SCREEN_HEIGHT / 3);

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

	class Block {
		int position;
		double xPos, yPos;
		std::string colour;
		Color clr;

	public:
		void set_values(double, double, std::string);
		double get_xPos();
		double get_yPos();
		int get_pos();
		string get_colour();
		Color get_clr();

		void set_XandY(double, double);
		void set_colour(std::string);
		void set_position(int);
		void set_clr(Color);

	};
	int Block::get_pos() { return pos; }
	double Block::get_xPos() { return xPos; }
	double Block::get_yPos() { return yPos; }
	string Block::get_colour() { return colour; }
	Color Block::get_clr() { return clr; }

	void Block::set_clr(Colour value) {clr = value; }

	void Block::set_values(double x, double y, std::string col) {
		xPos = x;
		yPos = y;
		colour = col;
	}

	void Block::set_XandY(double x, double y) {
		xPos = x;
		yPos = y;
	}

	void Block::set_colour(std::string col) {
		colour = col;
	}

	void Block::set_position(int pos) {
		position = pos;
	}

	Point getCellXandY(int position){
		//used to count up to the positon
		int counter = 0;
		for (int y = 0; y < 2; y++) {
			for (int x = 0; x < 2; x++) {
					if(position == counter)
						return Point(XTRANSLATION+GRIDSIZE*x, GRIDSIZE*y);	
				}
			}
		}		
	}

	Scalar getColourScalar(Color colour) {

		switch(colour){
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

	void drawHUD(Mat &imag, vecter<Block> blks;) {

		//draw the vertical lines
		for (int x = 0; x < 4; x++) {
			line(image, Point((x*GRIDSIZE)+XTRANSLATION, 0), Point((x*GRIDSIZE) + XTRANSLATION, SCREEN_HEIGHT), Scalar(20, 255, 255), 3, 8, 0);
		}
		//draw the horizontal lines
		for (int y = 0; y < 3; y++) {
			line(image, Point(XTRANSLATION, (y*GRIDSIZE + GRIDSIZE)), Point((3*GRIDSIZE) + XTRANSLATION, (y*GRIDSIZE+ GRIDSIZE)), Scalar(20, 255, 255), 3, 8, 0);
		}


		for (Block blk : blks) {
			Point pos = getCellXandY(blk.get_pos());
			rectangle(image, pos, Point(pos.x+GRIDSIZE, pos.y+GRIDSIZE), getColourScalar(blk.get_clr), 3, 8, 0)
		}
	}

	//checks if the blocks are in a 3x3 grid and assings them according to 
	//the relative frame and size of the camera image. - the camera must be aligned correctely
	void AssignPositions(vector<Block> &blocks) {

		for (Block blk : blocks) {
			for (int y = 0; y < 2; y++) {
				for (int x = 0; x < 2; x++) {
					if ((blk.get_xPos() >= XTRANSLATION + (x*GRIDSIZE)) && (blk.get_yPos() < XTRANSLATION + (x*GRIDSIZE)) &&
						(blk.get_xPos()*y >= y) && (blk.get_yPos() < XTRANSLATION + (y*GRIDSIZE))) {
						blk.set_position(x + y);
					}
				}
			}
		}
	}

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
		for (i = 0; i < contours.size(); i++)
		{

			Moments moment = moments((cv::Mat)contours[i]);
			area = contourArea(contours[i]);

			double xPos = moment.m10 / area;
			double yPos = moment.m01 / area;

			Block block;
			block.set_values(xPos, yPos, "Yellow");
			result.push_back(block);

			//if (area > areaSize)
			//circle(original, Point(xPos, yPos), 50, Scalar(255, 255, 255), CV_FILLED, 8, 0);					
		}
		return result;
	}

	//get the binary image based on the asking colour
	Mat getBinaryImage(Mat image, std::string colour, Color clr) {

		Mat result;

		//default settings for each colour value - whole specturm
		int H_lower = 0;
		int S_lower = 0;
		int V_lower = 0;

		int H_higher = 179;
		int S_higher = 255;
		int V_higher = 255;

		//define colour range based on the asking colour
		if (clr == Yellow) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (clr == Red) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}
		else if (colour == Blue) {
			H_lower = 0;
			S_lower = 50;
			V_lower = 50;

			H_higher = 30;
			S_higher = 0;
			V_higher = 0;
		}

		/*//sliders used for calibrating the colours
		createTrackbar("Blue H - Lower", "Controls", &blue_H_l, 179);
		createTrackbar("Blue S - Lower", "Controls", &blue_S_l, 255);
		createTrackbar("Blue V - Lowre", "Controls", &blue_V_l, 255);

		createTrackbar("Blue H - Upper ", "Controls", &blue_H_u, 179);
		createTrackbar("Blue S - Upper", "Controls", &blue_S_u, 255);
		createTrackbar("Blue V - Upper", "Controls", &blue_V_u, 255);*/

		//Threshold the image based on the colour
		inRange(image, Scalar(H_lower, S_lower, V_lower), Scalar(H_higher, S_higher, V_higher), result);
		return result;
	}

	

	void setShapeColour(Mat image, vector<Block> &blocks, Color color){
		
		int area_size = 100;
		vector<Block> tempBlocks;
		tempBlocks = getShapes(image, area_size);

		for (Block block : tempBlocks)
			block.set_clr(color)
			blocks.push_back(block);
	} 

	int process(VideoCapture& capture) {
		int n = 0;
		char filename[200];
		string window_name = "video | q or esc to quit";
		cout << "press space to save a picture. q or esc to quit" << endl;


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
			cvtColor(frame, frame, CV_BGR2HSV);

			//morphological opening (removes small contours from the image)
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			erode(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));
			dilate(frame, frame, getStructuringElement(MORPH_ELLIPSE, Size(5, 5)));

			//list of images one for each colour being detected

			vector<Block> blocks;

			
			//gets the binary image of each colour and then find all the 
			//contours for that given colour
			setShapeColour(getBinaryImage(frame, Yellow), blocks, Yellow);
			setShapeColour(getBinaryImage(frame, Yellow), blocks, Blue);
			setShapeColour(getBinaryImage(frame, Yellow), blocks, Green);
			setShapeColour(getBinaryImage(frame, Yellow), blocks, Red);

			//Assign x and y positions for each block
			AssignPositions(blocks);

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
