#include "Spaceship.h"

void Spaceship::SetMoveLeftKey(short a_leftKey)
{
	moveLeftKey = a_leftKey;
}

void Spaceship::SetMoveRightKey(short a_rightKey)
{
	moveRightKey = a_rightKey;
}

short Spaceship::GetMoveLeftKey()
{
	return moveLeftKey;
}

short Spaceship::GetMoveRightKey()
{
	return moveRightKey;
}

void Spaceship::SetLives(int a_lives)
{
	iLives = a_lives;
}

int Spaceship::GetLives()
{
	return iLives;
}

int Spaceship::GetLivesWidth()
{
	return iLivesWidth;
}

int Spaceship::GetLivesHeight()
{
	return iLivesHeight;
}

Spaceship::Spaceship()
{
	iLivesHeight = 75;
	iLivesWidth = 50;
	iHeight = 64;
	iWidth = 50;
	moveLeftKey = -1;
	moveRightKey = -1;
	iLives = 1;
}
