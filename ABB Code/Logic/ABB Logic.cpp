#include <iostream>
using namespace std;

enum Colour { Red, Green, Blue, Brown, Black, White, Yellow, Blue, None };
class Position {
	public:
		int funcNum; //The number you send to the robot to trigger the function
		Colour colour = None;
};

Position::Position(int functionNo) {
	funcNum = functionNo;
}

class Action {
	public:
		int funcNum;
};

Action::Action(int functionNo) {
	funcNum = functionNo;
}

int main() {
	Position grid[8] = {
		new Position(1), new Position(2), new Position(3),
		new Position(4), new Position(5), new Position(6),
		new Position(7), new Position(8), new Position(9)
	};

	Position redStack[] = {
		new Position(10),
		new Position(11),
		new Position(12)
	};

	Position greenStack[] = {
		new Position(13),
		new Position(14),
		new Position(15)
	};

	Position blueStack[] = {
		new Position(16),
		new Position(17),
		new Position(18) 
	};
	
	/*
	 * The following loop assigns the colour to each position on the grid
	 */
	for (int i = 0; i < 9; ++i)
	{
		grid[i].colour = /*Place function to retrieve appropriate colour here*/
	}

	//Move to home position Above center Position
	
	
}