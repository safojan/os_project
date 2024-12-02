#include "globals.h"
#include "board.h"  // Include the full header only in the .cpp file

// Define and initialize global variables (defined here to avoid multiple definitions)
int TotalTokens = 4;
pthread_t Master;
pthread_t Player[Players];
bool check = true;
int Dice;
int CurScore;
int playerNo = -1; // Player currently rolling the dice (initialized to -1, indicating no player)
string playerName[Players];
sem_t Dice_Roll, Grid_Acess;
sem_t RoundTurns;
sem_t winCheck;
int countturn = 0;
bool To_Cancel[Players] = { false, false, false, false };
int CurScreen = 0;
bool c1 = false;
int totalp = 4;

// Define the Board object here
Board* b=new Board();  // Now you can define the Board object after including board.h

//Functions 

// This function Will Check the Dice for Hit Of Token With Rival Tokens
// This will not kill the player present on the save spot
void HitCheck(int Num)
{
	int index1, index2;
	for(int i=0; i<Players; i++)
	{
		if(i==Num)
		continue;
		for(int j=0; j<TotalTokens; j++)
		{
			index1= b->Player_s[Num].Tokens[j].Index;
			if(index1!=-1)
			{
				for(int k=0; k<TotalTokens; k++ )
				{
					index2=b->Player_s[i].Tokens[k].Index;
					if(b->Player_s[i].compare(b->Player_s[i].Tokens[k],k)){

					}
					if(index2!=-1 && b->Routes[Num][index1]==b->Routes[i][index2])
					{
						// this is check for the certain Save Zone on the Games
						bool c1=true;
						for (int i = 0; i<StopSize ;i++)
						{
							if(b->stops[i]==b->Routes[Num][index1])
							c1=false;
						}
						if(c1)
						{
							b->Player_s[Num].HitRates++; 
							b->Player_s[i].Tokens[k].Index= -1;
							sleep(1);
						}
					}
				}
			}
		}
	}	

}



// Threading start heres
// this Function Rolls the Dice
int RollDice()
{
	int score=0;
	for(int i=0; i<Max_Con_turns; i++)
	{
		score+=GetRandInRange(1,7);
		if(score%6!=0)
		break;
	}
	return score; 	
}


void GridAccessChecks(int score)
{
	while(score>0)
	{
		if(score<6)
		{
			Dice=score;
			cout<<"Player "<<playerName[playerNo]<<" ::"<<Dice<<endl;
			int count=0,index=0;
			for(int i=0; i<TotalTokens;  i++) 
			{
				if(b->Player_s[playerNo].Tokens[i].Index==-1)
				count++;
				else
				index=i;
			}
			if(count==TotalTokens-1)
			{	
				CurScore=Dice; 
				int temp= b->Player_s[playerNo].Tokens[index].Index+CurScore;
				if(temp<51 || b->Player_s[playerNo].HitRates!=0)
				{
					if(temp<56)
					{
						while(CurScore!=0)
						{
							b->Player_s[playerNo].Tokens[index].Index++;
							CurScore--;
						}
					}
					else if(temp==56)
					{
						b->Player_s[playerNo].Tokens[index].Finish=true;
						CurScore=0;	
					}
				}
				else if(b->Player_s[playerNo].HitRates==0)// this is check for double role of same goti if it has hitrate= 0
				{
					b->Player_s[playerNo].Tokens[index].Index=(temp+6)%RouteSize;
					CurScore=0;
				}
				HitCheck(playerNo);
			}
			else if(count==TotalTokens)
			{
				cout<<"Dice Roll Ignored All Token at Home"<<endl;

			}
			else// what to do When we have 2 or more token on the board
			{
				CurScore=Dice;
				cout<<"Enter a token Number To Move it"<<endl;
				while(CurScore!=0){}
				HitCheck(playerNo);
			}
		}
		else if(score<18)
		{
			Dice= 6;
			b->Player_s[playerNo].SixCount++;
			cout<<"Player "<<playerName[playerNo]<<" ::"<<Dice<<endl;
			CurScore=Dice;
			cout<<"Enter a Token(0-3) Number to Move or Place"<<endl;
			while(CurScore!=0){}
			HitCheck(playerNo);
		}
		else
		{
			Dice  = 6;
			sleep(1);
			Dice  = 0;
			sleep(1);
			Dice  =6;
			sleep(1);
			Dice  = 0;
			sleep(1);
			Dice=6;
			score-=18;
		}
		score-=Dice;
	}
	return;
}

// this function will check wheather a player Win or Not
// Also this will checks wheather a player get six 
bool Cancellation_Check(int No)
{
	bool check=true;
	for(int i=0; i<TotalTokens; i++)
	{
		if(b->Player_s[No].Tokens[i].Finish==false)
		check=false;
	}
	if(check==true)
	{
		c1=true;
		return true;
	}
	if(b->Player_s[No].Noofturns==TurnConsective)
	{
		if(b->Player_s[No].SixCount==0 && b->Player_s[No].HitRates==0)
		check=true;
	}	
	if(check)
	{
		b->Player_s[No].cancelstate=1;
		c1=true;
	}
	return check;
}


// this Section Contain master other threads
void* PlayerTurn(void* arg)
{
	int score;
	int p1=0;
	while(true)
	{
		// Roll the Dice
		sem_wait(&Dice_Roll);
			score=RollDice();
		sem_post(&Dice_Roll);

		// Move the token oven the Grid
		sem_wait(&Grid_Acess);
		playerNo=*(int*)arg;
		sleep(1);
		GridAccessChecks(score);
		p1=playerNo;
		playerNo=-1;
		sem_post(&Grid_Acess);

		// Make all number of player currently playing wait untill everyone completes
		sem_wait(&RoundTurns);
		countturn++; 
		if(countturn==totalp)
		{
			cout<<"\nAll Player Have Got turn in this Round\n\n";
			countturn=0;
			sleep(1);
		}
		sem_post(&RoundTurns);
		while(countturn!=0){}

		// Check wheather a thread Has satisfy Win Condition or not
		
		sem_wait(&winCheck);
			p1 = *(int*)arg;
			b->Player_s[p1].Noofturns++;
			To_Cancel[p1]= false;
			//To_Cancel[p1]= Cancellation_Check(p1);
			if(b->Player_s[p1].Noofturns==TurnConsective)
			{
				b->Player_s[p1].Noofturns=0;
				b->Player_s[p1].SixCount=0;
			}
			if(To_Cancel[p1])
			totalp--;
		sem_post(&winCheck);
		while(c1){}
		

	}
	pthread_exit(NULL);
}


// This is Master thread who can cancel and end game at any stage
void* Master_Thread(void* arg)
{
	int count=0; 
	int Arr[]  = {0,1,2,3};
	bool flag[]= {true, true, true, true};
	//threads created 
	for(int i=0; i< Players; i++ )
	pthread_create(&Player[i], NULL,&PlayerTurn,&Arr[i]);
	for(int i=0; i<Players ; i++)
	pthread_detach(Player[i]);
	while(true)
	{
		for(int i=0; i<Players; i++)
		{
			if(To_Cancel[i] && flag[i])
			{
				sem_wait(&winCheck);
					cout<<"Cancellation Signal To Master thread from Player ::"<<playerName[i];
					cout<<"\nVerification of the Request By Master thread\nThread Cancel Sucessfully\n";
					pthread_cancel(Player[i]);
					b->Player_s[i].Position=count +1;
					count++;
					flag[i]=false;
					c1=false;
					sleep(1);
				sem_post(&winCheck);
			}
		}
		if(count==3)
		break;

	}
	for(int i=0; i<Players; i++)
	{
		if(flag[i]==true)
		{

			cout<<"Cancellation Signal To Master thread from Player ::"<<playerName[i];
			cout<<"\nVerification of the Request By Master thread\nThread Cancel Sucessfully\n";
			b->Player_s[i].Position=count +1;
			pthread_cancel(Player[i]);
		}
	}
	// Distory all the semaphores
	sem_destroy(&Dice_Roll);
	sem_destroy(&Grid_Acess);
	sem_destroy(&RoundTurns);
	sem_destroy(&winCheck);
	CurScreen=4;
	pthread_exit(NULL);
}





void GameDisplay()/**/
{
	// set the background color using function glClearColor.
	// to change the background play with the red, green and blue values below.
	// Note that r, g and b values must be in the range [0,1] where 0 means dim rid and 1 means pure red and so on.

	glClearColor(0, 0,0, .3 ); // Red==Green==Blue===1 ,Alpha=0 --> White Colour
	glClear (GL_COLOR_BUFFER_BIT); //Update the colors
	if(CurScreen==0)
	{
		b->MenuDisplay();
	}
	else if(CurScreen==1)
	{
		b->DrawBoard();
		if(check)
		{
			check=false;
			pthread_create(&Master,NULL,&Master_Thread, NULL);
		}
	}
	else if(CurScreen==3)
	{
		b->DisplayPlayerInfo();
	}
	else if(CurScreen==4)
	{
		b->DrawEndScreen();
	}
	glutSwapBuffers(); // do not modify this line..
	glutPostRedisplay();
}



// seed the random numbers generator by current time (see the documentation of srand for further help)...
/* Function sets canvas size(drawing area) in pixels...
 *  that is what dimensions (x and y) your game will have
 *  Note that the bottom-left coordinate has value (0,0) and top-right coordinate has value (width-1,height-1)
 * */

void SetCanvasSize(int width, int height) {
	glMatrixMode (GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0, width, 0, height, -1, 1); // set the screen size to given width and height.
	glMatrixMode (GL_MODELVIEW);
	glLoadIdentity();
}



/*This function is called (automatically) whenever any non-printable key (such as up-arrow, down-arraw)
 * is pressed from the keyboard
 *
 * You will have to add the necessary code here when the arrow keys are pressed or any other key is pressed...
 *
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 *
 * */

void NonPrintableKeys(int key, int x, int y) 
{
	if (key
			== GLUT_KEY_LEFT /*GLUT_KEY_LEFT is constant and contains ASCII for left arrow key*/) {
		// what to do when left key is pressed...

	} else if (key
			== GLUT_KEY_RIGHT /*GLUT_KEY_RIGHT is constant and contains ASCII for right arrow key*/) {

	} else if (key
			== GLUT_KEY_UP/*GLUT_KEY_UP is constant and contains ASCII for up arrow key*/) {

	}

	else if (key
			== GLUT_KEY_DOWN/*GLUT_KEY_DOWN is constant and contains ASCII for down arrow key*/) {

	}

	/* This function calls the Display function to redo the drawing. Whenever you need to redraw just call
	 * this function*/

	glutPostRedisplay();

}

/*This function is called (automatically) whenever any printable key (such as x,b, enter, etc.)
 * is pressed from the keyboard
 * This function has three argument variable key contains the ASCII of the key pressed, while x and y tells the
 * program coordinates of mouse pointer when key was pressed.
 */
void performCheck(int gotiNo)
{
	int index= b->Player_s[playerNo].Tokens[gotiNo].Index;
	if(index==-1 && CurScore==6)
	{
		b->Player_s[playerNo].Tokens[gotiNo].Index=0;
		CurScore=0;
		return;
	}
	if(index==-1 && CurScore!=6)
	cout<<"Invalid Move-> You Cann't move token at Home Without Six"<<endl;

	if(index!=-1)
	{
		if(index+CurScore<51 || b->Player_s[playerNo].HitRates!=0)
		{
			if(index+CurScore<56)
			{
				while(CurScore!=0)
				{
					b->Player_s[playerNo].Tokens[gotiNo].Index++;
					CurScore--;
				}
				
			}
			else if(index+CurScore==56)
			{
				b->Player_s[playerNo].Tokens[gotiNo].Finish=true;
				CurScore=0;
				cout<<"This token Finished, Enter a diffent token"<<endl;
			}
			else
			{
				int count=0; 
				for(int i=0; i<TotalTokens; i++)
				{
					if(b->Player_s[playerNo].Tokens[gotiNo].Index==-1)
					count++;
				}
				if(count==0 )
				{
					CurScore=0;	
				}
			}
		}
		else if(b->Player_s[playerNo].HitRates==0)// This if make the token to complete round if hitrate is zero 
		{
			b->Player_s[playerNo].Tokens[gotiNo].Index=(index+6+CurScore)%RouteSize;
			CurScore=0;
		}
		// cout<<"Don't know why the shit if not runned::"<<index+CurScore<<endl;
		return;
	}
	return;
}

void PrintableKeys(unsigned char key, int x, int y) 
{
	if(CurScreen==1)
	{
		if(playerNo>-1)
		{
			if(key=='0')
			{
				performCheck(0);	
			}
			else if(key=='1' && TotalTokens>=2)
			{
				performCheck(1);
			}
			else if(key=='2' && TotalTokens>=3)
			{
				performCheck(2);	
			}
			else if(key=='3' && TotalTokens>=4)
			{
				performCheck(3);
			}
		}
	}

	if (key == 27) 
	{
		exit(1); // exit the program when escape key is pressed.
	}
	if(CurScreen==0)
	{
		if(key=='S' || key=='s')
		CurScreen=1;
		else if(key=='i' || key=='I')
		CurScreen=3;
	}
	if(CurScreen==2 || CurScreen==3)
	{
		if(key=='b' || key=='B')
		{
			CurScreen=0;
		}
	}
	glutPostRedisplay();
}


/*
 * This function is called after every 1000.0/FPS milliseconds
 * (FPS is defined on in the beginning).
 * You can use this function to animate objects and control the
 * speed of different moving objects by varying the constant FPS.
 *
 * */
void Timer(int m) {

	// implement your functionality here

	// once again we tell the library to call our Timer function after next 1000/FPS
	glutPostRedisplay();
	glutTimerFunc(1000.0, Timer, 0);
}

/*This function is called (automatically) whenever your mouse moves witin inside the game window
 *
 * You will have to add the necessary code here for finding the direction of shooting
 *
 * This function has two arguments: x & y that tells the coordinate of current position of move mouse
 *
 * */
void MousePressedAndMoved(int x, int y) 
{
	// cout << x << " " << y << endl;
	glutSwapBuffers();
	
}
void MouseMoved(int x, int y) 
{
	// cout<<"X,Y::"<<x<<" , "<<y<<endl;
	glutPostRedisplay();
}

/*This function is called (automatically) whenever your mouse button is clicked witin inside the game window
 *
 * You will have to add the necessary code here for shooting, etc.
 *
 * This function has four arguments: button (Left, Middle or Right), state (button is pressed or released),
 * x & y that tells the coordinate of current position of move mouse
 *
 * */
void MouseClicked(int button, int state, int x, int y) 
{
	y=840-(y*825/655);
	if (button == GLUT_LEFT_BUTTON && state==0) // dealing only with left button
	{
	}
	glutPostRedisplay();
}
