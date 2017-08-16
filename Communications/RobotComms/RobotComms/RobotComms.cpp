// RobotComms.cpp : main project file.

#include "stdafx.h"

using namespace System;
using namespace System::IO::Ports;

int main(array<System::String ^> ^args)
{
	array<unsigned char>^ texBufArray = gcnew array<unsigned char>(1); 
	int baudRate=9600;
	// robot interpreter box settings
	SerialPort^ robot_int;
	robot_int = gcnew SerialPort("COM4", baudRate);
	// open port
	try
	{
		while (true) {
			for (int i = 0; i < 256; i++)
			{
				// Open port to robot interpreter box
				robot_int->Open();

				// Set number to send to the port
				texBufArray[0] = i;

				// Write number to the port
				robot_int->Write(texBufArray, 0, 1);

				// close port to robot interpreter box
				robot_int->Close();
			}

			for (int i = 256; i > 0; i--)
			{
				// Open port to robot interpreter box
				robot_int->Open();

				// Set number to send to the port
				texBufArray[0] = i;

				// Write number to the port
				robot_int->Write(texBufArray, 0, 1);

				// close port to robot interpreter box
				robot_int->Close();
			}
		}
	}
	catch (IO::IOException^ e  ) 
	{ 
		Console::WriteLine(e->GetType()->Name+": Port is not ready");
	}
	// end program
	Console::Write("Press enter to close the program");
	Console::Read();
    return 0;
}


