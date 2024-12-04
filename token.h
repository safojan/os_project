/* 
M USMAN GHANI (22I-8796)
RAYYAN AHMED  (22I-2489)
SAFDAR JAN    (22I-8804)
*/
#ifndef TOKEN_H
#define TOKEN_H
#include "cordinate.h"

// Tokens for each player to show movement
struct Token
{
	Cordinate* Position; 
	int Index;
	bool Finish;
	Token()
	{
		Index=-1;
		Finish =false; 
	}
	
};
#endif // COORDINATE_H
