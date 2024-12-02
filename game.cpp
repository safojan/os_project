#ifndef CENTIPEDE_CPP_
#define CENTIPEDE_CPP_
#include "util.h"
#include "globals.h"
#include "cordinate.h"
#include "token.h"
#include "player.h"
#include "board.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <cmath> 
#include <cstdlib>
#include <pthread.h>
#include <ctime>
using namespace std;



int main(int argc, char*argv[]) {

	int width = 1020, height = 840; // i have set my window size to be 800 x 600
	sem_init(&Dice_Roll,0, 1);
	sem_init(&Grid_Acess,0,1);
	sem_init(&RoundTurns,0,1);
	sem_init(&winCheck, 0, 1);
	cout<<"\n\n\t\tWELCOME TO C++ LUDO \n\n";
	cout<<"Enter 1st Player Name : ";
	cin>>playerName[0];
	cout<<"Enter 2nd Player Name : ";
	cin>>playerName[1];
	cout<<"Enter 3rd Player Name : ";
	cin>>playerName[2];
	cout<<"Enter 4th Player Name : ";
	cin>>playerName[3];


	
	//b->InitalizeBoard(width, height);
	InitRandomizer(); // seed the random number generator...
	glutInit(&argc, argv); // initialize the graphics library...
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA); // we will be using color display mode
	glutInitWindowPosition(50, 50); // set the initial position of our window
	glutInitWindowSize(width, height); // set the size of our window
	glutCreateWindow("OS Project By Usman,Rayyan and Safdar"); // set the title of our game window
	SetCanvasSize(width, height); // set the number of pixels...

	// Register your functions to the library,
	// you are telling the library names of function to call for different tasks.
	//glutDisplayFunc(display); // tell library which function to call for drawing Canvas.
	glutMouseFunc(MouseClicked);
	
	glutDisplayFunc(GameDisplay); // tell library which function to call for drawing Canvas.
	glutSpecialFunc(NonPrintableKeys); // tell library which function to call for non-printable ASCII characters
	glutKeyboardFunc(PrintableKeys); // tell library which function to call for printable ASCII characters
	// This function tells the library to call our Timer function after 1000.0/FPS milliseconds...
	glutTimerFunc(1000.0, Timer, 0);

	
	//glutPassiveMotionFunc(MouseMoved); // Mouse
	//glutMotionFunc(MousePressedAndMoved); // Mouse

	// now handle the control to library and it will call our registered functions when
	// it deems necessary...
	glutMainLoop();
	return 1;
}
#endif 