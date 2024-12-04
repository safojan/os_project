/* 
M USMAN GHANI (22I-8796)
RAYYAN AHMED  (22I-2489)
SAFDAR JAN    (22I-8804)
*/
#ifndef GLOBALS_H
#define GLOBALS_H

#include <iostream>
#include <string>
#include <semaphore.h>
#include <pthread.h>

using namespace std;

// Forward declare the Board class (no need to include "board.h")
struct Board;

// Constants (Use const for compile-time constants)
const int gridsize = 15;
const int Players = 4;
const int RouteSize = 58;
const int StopSize = 8;
const int Max_Con_turns = 3;      // Maximum consecutive rolls
const int TurnConsective = 3;     // Max limit for player if they do not roll six or kill another player

// Global variables (Declare using extern to avoid multiple definitions)
extern int TotalTokens;
extern pthread_t Master;
extern pthread_t Player[Players];
extern bool check;
extern int Dice;
extern int CurScore;
extern int playerNo;             // Indicates which player is rolling the dice
extern string playerName[Players];
extern sem_t Dice_Roll, Grid_Acess;
extern sem_t RoundTurns;
extern sem_t winCheck;
extern int countturn;
extern bool To_Cancel[Players];
extern int CurScreen;
extern bool c1;
extern int totalp;

// Forward declaration of the Board object
extern Board* b; 

// Function prototypes (to be defined elsewhere)
void HitCheck(int Num);
int RollDice();
void GridAccessChecks(int score);
bool Cancellation_Check(int No);
void* PlayerTurn(void* arg);
void* Master_Thread(void* arg);
void GameDisplay();
void SetCanvasSize(int width, int height);
void NonPrintableKeys(int key, int x, int y);
void performCheck(int gotiNo);
void PrintableKeys(unsigned char key, int x, int y);
void Timer(int m);
void MousePressedAndMoved(int x, int y);
void MouseMoved(int x, int y);
void MouseClicked(int button, int state, int x, int y);


#endif // GLOBALS_H
