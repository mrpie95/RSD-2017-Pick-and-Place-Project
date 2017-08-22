#include <iostream>
#include "stdafx.h"

using namespace std;
using namespace System;
using namespace System::IO::Ports;

enum Colour { Red, Green, Blue, Brown, Black, White, Yellow, Blue, None };
class Block {
	public:
		//Position number is the number to move to that position, so if you wanted to move to block 2, you can simply call the block's position number
		int positionNum;
		Colour colour = None;
};

//Constructor
Block::Block(int functionNo, Colour col) {
	positionNum = functionNo;
	colour = col;
}

class Action {
	public:
		int funcNum;
};
//Constructor
Action::Action(int functionNo) {
	funcNum = functionNo;
}

int main(array<System::String ^> ^args) {
	/*
	 * Assigned numbers to positions
	 */
	Action moveToHome = new Action(0);

	Action moveToGrid[8] = {
		new Action(1), new Action(2), new Action(3),
		new Action(4), new Action(5), new Action(6),
		new Action(7), new Action(8), new Action(9)
	};

	Action moveToRedStack[] = {
		new Action(10),
		new Action(11),
		new Action(12)
	};

	Action moveToGreenStack[] = {
		new Action(13),
		new Action(14),
		new Action(15)
	};

	Action moveToBlueStack[] = {
		new Action(16),
		new Action(17),
		new Action(18)
	};

	Action moveToBlackStack[] = {
		new Action(19),
		new Action(20),
		new Action(21)
	};

	/*
	 * Actions
	 */
	Action openGripper = new Action(40);
	Action closeGripper = new Action(41);
	

	/*
	 * Initialising the serial box connection
	 */
	array<unsigned char>^ texBufArray = gcnew array<unsigned char>(1);
	int baudRate = 9600;
	// robot interpreter box settings
	SerialPort^ robot_int;
	robot_int = gcnew SerialPort("COM4", baudRate);


	/*
	 * Image Capture - The following loop assigns the colour to each position on the grid
	 */
	for (int i = 0; i < 9; ++i){
		grid[i].colour = /*Place function to retrieve appropriate colour here*/
	}


	/*
	 * Move from each grid position
	 */
	for (int i = 0; i < 9; ++i){
		try {
			// Open port to robot interpreter box
			robot_int->Open();

			/*
			 * Move to home
			 */
			texBufArray[0] = moveToHome.funcNum;
			robot_int->Write(texBufArray, 0, 1);

			texBufArray[0] = openGripper.funcNum;
			robot_int->Write(texBufArray, 0, 1);

			/*
			* Move to block 1 pick up the block and move it to the black stack
			*/
			texBufArray[0] = moveToGrid[i].funcNum;
			robot_int->Write(texBufArray, 0, 1);
			
			texBufArray[0] = closeGripper.funcNum;
			robot_int->Write(texBufArray, 0, 1);
			
			texBufArray[0] = moveToHome.funcNum;
			robot_int->Write(texBufArray, 0, 1);

			texBufArray[0] = moveToBlackStack[0].funcNum;
			robot_int->Write(texBufArray, 0, 1);

			texBufArray[0] = openGripper.funcNum;
			robot_int->Write(texBufArray, 0, 1);
			// close port to robot interpreter box
			robot_int->Close();
		}
		catch (IO::IOException^ e) {
			Console::WriteLine(e->GetType()->Name + ": Port is not ready");
		}
	}
}