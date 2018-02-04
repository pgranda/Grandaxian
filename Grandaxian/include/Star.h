#ifndef _STAR_H_
#define _STAR_H_

#include "Position.h"

class Star
{
public:
	Star();

	void SetPosition(float a_x, float a_y);
	void GetPosition(float& a_x, float& a_y);
	void SetColour(int a_starColour);
	int GetColour();
	void SetIsShining(bool a_shining);
	bool GetIsShining();
	float GetSpeed();
	void SetSpeed(float a_speed);
	void Move(int a_counter);

private:
	Position pos;
	int colour;
	bool isShining;
	float iSpeed;
};

#endif