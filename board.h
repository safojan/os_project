#ifndef BOARD_H
#define BOARD_H
#include "util.h"
#include "globals.h"
#include "cordinate.h"
#include "token.h"
#include "player.h"
#include <iomanip>
#include <iostream>
#include <string>
#include <semaphore.h>
#include <cmath> 
#include <cstdlib>
#include <pthread.h>
#include <ctime>
using namespace std;

struct Board
{

	Cordinate grid[gridsize][gridsize];
	player Player_s[Players];
	int Routes[Players][RouteSize];
	int stops[StopSize];

	// Default Constructor for the board Class
	Board():Routes{{20, 21, 22, 23, 24, 16, 13, 10, 07, 04, 01, 02, 03, 06,  9, 12, 15, 18,
					25, 26, 27, 28, 29, 30, 42, 54, 53, 52, 51, 50, 49, 57, 60, 63, 66, 69, 
					72, 71, 70, 67, 64, 61, 58, 55, 48, 47, 46, 45, 44, 43, 31, 32, 33, 34, 35, 36,0,19},
				   { 6,  9, 12, 15, 18, 25, 26, 27, 28, 29, 30, 42, 54, 53, 52, 51, 50, 49,
				    57, 60, 63, 66, 69, 72, 71, 70, 67, 64, 61, 58, 55, 48, 47, 46, 45, 44, 
				    43, 31, 19, 20, 21, 22, 23, 24, 16, 13, 10, 7, 4, 1, 2, 5, 8, 11, 14, 17,0,3},
				   {53, 52, 51, 50, 49, 57, 60, 63, 66, 69, 72, 71, 70, 67, 64, 61, 58, 55,
				    48, 47, 46, 45, 44, 43, 31, 19, 20, 21, 22, 23, 24, 16, 13, 10, 7, 4, 1,
				     2,  3, 06,  9, 12, 15, 18, 25, 26, 27, 28, 29, 30, 42, 41, 40, 39, 38, 37,0,54},
				    {67, 64, 61, 58, 55, 48, 47, 46, 45, 44, 43, 31, 19, 20, 21, 22, 23, 24,
				    16, 13, 10, 7, 4, 1, 2, 3, 06,  9, 12, 15, 18, 25, 26, 27, 28, 29, 30, 42,
				    54, 53, 52, 51, 50, 49, 57, 60, 63, 66, 69, 72, 71, 68, 65, 62, 59, 56,0,70}},stops{20,7,6,28,53,66,67,45}
	{
		intializeGrid();
		intialize_Token_Pos();
	}

	// This function simple intialize the x,y location of each box
	// Assign the weight of each location in the board
	void intializeGrid()
	{
		int x= 100; 
		int y= 750;
		int count=1;
		for(int i=0; i<gridsize; i++ )
		{
			x=100;
			for(int j=0; j<gridsize; j++ )
			{
				grid[i][j].x= x;
				grid[i][j].y= y;
				if(i==j)
				{
					grid[i][j].route=0;
				}
				else if(i<6 && j<6)
				grid[i][j].route=-1;
				else if(i<6 && j>8)
				grid[i][j].route=-2;
				else if(i>8 && j<6)
				grid[i][j].route=-3;
				else if(i>8 && j>8)
				grid[i][j].route=-4;
				else if(i==6 && (j==7||j==8))
				{
					grid[i][j].route=0;
				}
				else if(i==7 && (j==6||j==8))
				{
					grid[i][j].route=0;
				}
				else if(i==8 && (j==6||j==7))
				{
					grid[i][j].route=0;
				}
				else if(i==6 || j==6 || i==7 || j==7 || i==8 || j==8)
				{
					grid[i][j].route=count;
					count++;	
				}
				else
				grid[i][j].route=0;

				cout<<setw(2)<<grid[i][j].route<<"  ";
				x+=50;
			}
			cout<<endl;
			y-=50;
		}
	}

	void intialize_Token_Pos()
	{
		int I_index[]= {1,1,10,10};
		int J_index[]= {2,11,11,2};
		int tem1,tem2;
		int check=0;
		for(int i=0; i<Players; i++ )
		{
			tem1=I_index[i];
			tem2=J_index[i];
			check=0;
			for(int j=0 ; j<TotalTokens ;j++ )
			{
				Player_s[i].Tokens[j].Position= &grid[tem1][tem2];
				check++;
				tem1+=2;
				if(check==2)
				{
					tem1=I_index[i];
					tem2+=2;
				}
			}
		}
	}

	void MenuDisplay()
{
    // Background
    DrawRectangle(0, 0, 1020, 840, colors[BLACK]);
    DrawRectangle(20, 20, 980, 800, colors[BLACK]);

    // Moving Rectangle Animation
    static int x = 100, state = 0;
    static float* ptr = colors[ORANGE];
    DrawRectangle(x, 20, 45, 50, ptr);
    switch (state)
    {
    case 0:
        if (x < 960)
            x += 5;
        else
        {
            state = 1;
            ptr = colors[BLUE_VIOLET];
        }
        break;
    case 1:
        if (x > 10)
            x -= 5;
        else
        {
            state = 0;
            ptr = colors[ORANGE];
        }
        break;
    }

    int x1 = 100;

    // Title
    DrawRoundRect(150 + x1, 690, 500, 50, colors[WHITE], 20);
    DrawString(260, 700, "LUDO GAME", colors[RED],GLUT_BITMAP_HELVETICA_18);

    // Menu Options
    
    DrawRoundRect(150 + x1, 630, 500, 40, colors[WHITE], 15);
    DrawString(160 + x1, 640, "1) START NEW GAME       Press  S", colors[BLACK],GLUT_BITMAP_HELVETICA_18);

    DrawRoundRect(150 + x1, 570, 500, 40, colors[WHITE], 15);
    DrawString(160 + x1, 580, "2) PLAYER INFO                  Press  I", colors[BLACK],GLUT_BITMAP_HELVETICA_18);

    DrawRoundRect(150 + x1, 510, 500, 40, colors[WHITE], 15);
    DrawString(160 + x1, 520, "3) SELECT MODE                Press  M", colors[BLACK],GLUT_BITMAP_HELVETICA_18);

    DrawRoundRect(150 + x1, 450, 500, 40, colors[WHITE], 15);    
	DrawString(160 + x1, 460, "4) EXIT                                Press Esc", colors[BLACK],GLUT_BITMAP_HELVETICA_18);

    // Footer
    DrawRoundRect(150 + x1, 380, 500, 40, colors[BLACK], 15);
    DrawString(200 + x1, 390, "Created by Usman , Rayyan and Safdar", colors[WHITE],GLUT_BITMAP_HELVETICA_12);
}
void DrawBoard()
	{
		DrawBoardAstetic();
		DrawStop(6, 1);
		DrawTokens();
		DrawDice();
		DrawSideManu();
	}
	void DisplayPlayerInfo()
	{
		glClearColor(1/*Red Component*/, 1,	//148.0/255/*Green Component*/,
			1/*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
		glClear (GL_COLOR_BUFFER_BIT);   
		DrawRectangle(100,200,700,500,colors[RED]);
		DrawRectangle(120,220,660,460,colors[BLACK]);
		DrawString(200,600,"PLAYER NAMES:: ",colors[RED]);
		DrawString(200,300,"PRESS B TO BACK TO MANU ",colors[RED]);
	}



	void DrawSideManu()
	{
		if(playerNo!=-1)
		{
			DrawString(880,640,playerName[playerNo]+"'s Turn",colors[WHITE]);
			DrawString(880,500,"Hit Rates", colors[WHITE] );
			int y=450;
			for(int i=0; i<Players; i++)
			{
				DrawString(880,y,playerName[i]+" ::"+Num2Str(Player_s[i].HitRates),colors[WHITE]);
				y-=40;
			}
		}
	}
	void DrawDice()             
	{
		int x=880;
		int y=700;
		DrawRoundRect(x,y,60,60,colors[WHITE], 10);
		if(Dice == 1)
		{
			DrawCircle(x+30,y+30,5,colors[BLACK]);
		}
		else if(Dice == 2)
		{
			for(int i=15;i<=45;i+=30){
				DrawCircle(x+i,y+i,5,colors[BLACK]);
			}
		}
		else if(Dice == 3)
		{
			for(int i=15;i<=45;i+=15){
				DrawCircle(x+i,y+i,5,colors[BLACK]);
			}			
		}
		else if(Dice == 4)
		{
			DrawCircle(x+15,y+15,5,colors[BLACK]);
			DrawCircle(x+45,y+45,5,colors[BLACK]);
			DrawCircle(x+15,y+45,5,colors[BLACK]);
			DrawCircle(x+45,y+15,5,colors[BLACK]);
			
		}
		else if(Dice == 5)
		{
			DrawCircle(x+15,y+15,5,colors[BLACK]);
			DrawCircle(x+45,y+45,5,colors[BLACK]);
			DrawCircle(x+15,y+45,5,colors[BLACK]);
			DrawCircle(x+45,y+15,5,colors[BLACK]);
			DrawCircle(x+30,y+30,5,colors[BLACK]);
		}
		else if(Dice == 6)
		{
			DrawCircle(x+15,y+15,5,colors[BLACK]);
			DrawCircle(x+15,y+30,5,colors[BLACK]);
			DrawCircle(x+15,y+45,5,colors[BLACK]);
			DrawCircle(x+45,y+15,5,colors[BLACK]);
			DrawCircle(x+45,y+30,5,colors[BLACK]);
			DrawCircle(x+45,y+45,5,colors[BLACK]);
		}
	}

	// Drawing stops using hard coding approach
	// This function Draw stops of respective color
	void DrawStop(int i,int j)
	{
		DrawSquare( grid[i][j].x, grid[i][j].y ,49,colors[GREEN_YELLOW]);
		DrawSquare( grid[i+2][j+1].x, grid[i+2][j+1].y ,49,colors[GREEN_YELLOW]);
		DrawSquare( grid[i][j+11].x, grid[i][j+11].y ,49,colors[NAVY]);
		DrawSquare( grid[i+2][j+12].x, grid[i+2][j+12].y ,49,colors[NAVY]);
		DrawSquare( grid[j+1][i].x, grid[j+1][i].y ,49,colors[ORANGE]);
		DrawSquare( grid[j][i+2].x, grid[j][i+2].y ,49,colors[ORANGE]);
		DrawSquare( grid[i+7][j+5].x, grid[i+7][j+5].y ,49,colors[RED]);
		DrawSquare( grid[i+6][j+7].x, grid[i+6][j+7].y ,49,colors[RED]);
	}


	
	//####################################################################### 
	// This function Will Draw Each Gotie according to it's Position Value
	// the index is kept to specify Each value at which currently goti it
	// ######################################################################
	
	void DrawTokens()
	{
		float* HomeCol[]= {colors[GREEN], colors[ORANGE], colors[BLUE], colors[RED]};
		float* Goticol2[]= {colors[GREEN_YELLOW], colors[DARK_ORANGE], colors[NAVY], colors[DARK_RED]};
		int x,y;
		for(int i=0 ; i<Players; i++)
		{
			for(int j=0; j<TotalTokens ; j++)
			{
				x= Player_s[i].Tokens[j].Position->x;
				y= Player_s[i].Tokens[j].Position->y;
				DrawCircle(x,y,30,HomeCol[i]);
				if(Player_s[i].Tokens[j].Index== -1)
				{
					DrawCircle(x,y,20,colors[BLACK]);
					DrawCircle(x,y,18,colors[DARK_GRAY]);
					DrawCircle(x,y,15,Goticol2[i]);

					DrawString(x-5,y-10,Num2Str(j), colors[WHITE]);
				}
				else
				{
					Cordinate* p=Compute_Index(Routes[i][Player_s[i].Tokens[j].Index]);
					x= p->x+25;
					y= p->y+25;
					
					if(Player_s[i].compare(Player_s[i].Tokens[j],j)){
						x= p->x+5;
						y= p->y+5;
						DrawRoundRect(x,y,40,40,Goticol2[i]);
						DrawString(x+15,y+10,Num2Str(j), colors[WHITE]);
					}
					else{
						if(Player_s[i].Tokens[j].Finish==true)
						{
							if(i==0)
							{
								x= grid[7][6].x;
								y= grid[7][6].y;
							}
							else if(i==1)
							{
								x= grid[6][7].x;
								y= grid[6][7].y;	
							}
							else if(i==2)
							{
								x= grid[7][8].x;
								y= grid[7][8].y;
							}
							else if(i==3)
							{	
								x= grid[8][7].x;
								y= grid[8][7].y;
							}
							x+=25;
							y+=25;
						}
						DrawCircle(x,y,20,colors[BLACK]);
						DrawCircle(x,y,18,colors[DARK_GRAY]);
						DrawCircle(x,y,15,Goticol2[i]);
						DrawString(x-5,y-10,Num2Str(j), colors[WHITE]);
					}
				}
			}
		}
	}

	// This function map integer route to the Global grid index
	// Calculate int ith and jth index for the given route value
	Cordinate* Compute_Index(int num)
	{
		int i=0, j=0;
		if(num<19)
		{
			i=num/3;
			j=num%3+5;
			if(num%3==0)
			{ 
				j+=3;
				i--;
			}
		}
		else if(num<55)
		{
			i= (num-19)/12+6; 
			j= (num-grid[i][0].route)%15; 
			if(j>5)
			j+=3; 
		}
		else
		{
			i=(num-54)/3+9;
			j=num%3+5;
			if(num%3==0)
			{ 
				j+=3;
				i--;
			}
		}
		return &grid[i][j];
	}

	void DrawEndScreen()
	{
		glClearColor(1/*Red Component*/, 1,	//148.0/255/*Green Component*/,
		1/*Blue Component*/, 1 /*Alpha component*/); // Red==Green==Blue==1 --> White Colour
		glClear (GL_COLOR_BUFFER_BIT);   
		DrawRectangle(100,200,700,500,colors[RED]);
		DrawRectangle(120,220,660,460,colors[BLACK]);
		int y=600;
		string str= "Player ";
		for(int i=0; i<Players; i++)
		{
			str+= playerName[i];
			str+= "   :: ";
			if(Player_s[i].cancelstate==1)
			str+= "Thread Was Cancelled";
			else
			{
				int c=0;
				for(int j=0; j<Players; j++)
				{
					if(Player_s[i].cancelstate==1)
					c++;	
				}
				str+= Num2Str(Player_s[i].Position-c);
			}
			DrawString(200,y,str,colors[RED]);
			y-=50;
			str= "Player ";
		}
		static bool b=true;
		if(b)
		{
			cout<<endl;
			for(int i=0; i<Players; i++)
			cout<<playerName[i]<<"  ::"<<Player[i]<<"-> "<<Player_s[i].cancelstate<<endl;
			b=false;
			cout<<endl;
		}
	}

	// This Finction Draw all the non moving section of the board
	void DrawBoardAstetic()
	{		
		// Green Home fixed		
		DrawSquare( 100 , 500 ,300,colors[GREEN_YELLOW]); //BOARD PATTERN
		DrawRoundRect(150,550,200,200,colors[WHITE],0);
		DrawString(210,760,playerName[0],colors[WHITE]);
		// Yellow Home fixed
		DrawSquare(550 , 500 ,300,colors[ORANGE]); 
		DrawRoundRect(600,550,200,200,colors[WHITE],0);
		DrawString(660,760,playerName[1],colors[WHITE]);
		// Blue Home fixed
		DrawSquare( 550 , 50 ,300,colors[NAVY]); //BOARD PATTERN
		DrawRoundRect(600,100,200,200,colors[WHITE],0);
		DrawString(660,310,playerName[2],colors[WHITE]);
		// Red home Fixed  
		DrawSquare( 100 ,50 ,300,colors[RED]); 
		DrawRoundRect(150,100,200,200,colors[WHITE],0);
		DrawString(210,310,playerName[3],colors[WHITE]);

		//CENTER BOX
		DrawSquare( 400 , 350 ,150,colors[FLORAL_WHITE]);
		DrawTriangle( 400, 350 , 475, 425 , 400 , 500, colors[GREEN_YELLOW] );
		DrawTriangle( 400, 500 , 475, 425 , 550 , 500, colors[ORANGE] );
		DrawTriangle( 550, 350 , 475, 425 , 550 , 500, colors[NAVY] );
		DrawTriangle( 550, 350 , 475, 425 , 400 , 350, colors[RED] );

		/* boarder */
		DrawLine( 95 , 45 ,  95 , 802 , 5 , colors[MISTY_ROSE] );    
		DrawLine( 93 , 804 ,  856 , 804 , 5 , colors[MISTY_ROSE] );
		DrawLine( 854 , 804 ,  854 , 45 , 5 , colors[MISTY_ROSE] );
		DrawLine( 93 , 44 ,  856 , 44 , 5 , colors[MISTY_ROSE] );

		// This is drawing the inner grid pattern 
		for(int i=0 ; i<gridsize ; i++ )
		{
			for(int j=0; j<gridsize ; j++)
			{
				if(grid[i][j].route>0 && grid[i][j].route<73)
				{
					DrawSquare( grid[i][j].x, grid[i][j].y ,49,colors[FLORAL_WHITE]);
				}
			}
		}
		// Centeral Colors
		for(int i=1;i<gridsize-1 ; i++ )
		{
			if(i==6 || i==7 || i==8)
			continue;
			if(grid[i][9].route==-2)
			DrawSquare( grid[i][7].x, grid[i][7].y ,49,colors[ORANGE]);
			else 
			DrawSquare( grid[i][7].x, grid[i][7].y ,49,colors[RED]);
			
			if(grid[9][i].route==-3)
			DrawSquare( grid[7][i].x, grid[7][i].y ,49,colors[GREEN_YELLOW]);
			else
			DrawSquare( grid[7][i].x, grid[7][i].y ,49,colors[NAVY]);
		}
	}

};


#endif // BOARD_H
