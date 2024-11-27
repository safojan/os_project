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
