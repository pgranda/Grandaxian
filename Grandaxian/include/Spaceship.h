#ifndef _SPACESHIP_H_
#define _SPACESHIP_H_

#include "AlienAndSpaceshipBase.h"

class Spaceship : public AlienAndSpaceshipBase
{
public:
	Spaceship();

	short GetMoveLeftKey();
	void SetMoveLeftKey(short a_leftKey);

	short GetMoveRightKey();
	void SetMoveRightKey(short a_rightKey);

	int GetLives();
	void SetLives(int a_lives);

	int GetLivesWidth();
	int GetLivesHeight();

private:
	int iLivesHeight;
	int iLivesWidth;
	int iLives;
	short moveLeftKey;
	short moveRightKey;
};

#endif