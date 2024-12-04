/* 
M USMAN GHANI (22I-8796)
RAYYAN AHMED  (22I-2489)
SAFDAR JAN    (22I-8804)
*/
#ifndef PLAYER_H
#define PLAYER_H
#include "token.h"
#include "globals.h"
struct player
{
	int HitRates;
	int Noofturns;
	int SixCount;
	int cancelstate;
	int Position;
	Token* Tokens;
	player()
	{
		Position=0;
		cancelstate=0;
		SixCount=0; 
		Noofturns=0;
		Tokens= new Token[TotalTokens];
		HitRates=0;
	}
	bool compare(Token t, int j){
		for(int i=0;i<TotalTokens;i++){
			if(j != i && t.Index == Tokens[i].Index){
				return true;
			}
		}
		return false;
	}

	~player()
	{
		delete []Tokens;	
	}
}; 


#endif // PLAYER_H
