#ifndef _ALIENANDSPACESHIPBASE_H_
#define _ALIENANDSPACESHIPBASE_H_

#include <stdio.h>
#include <time.h>
#include "Position.h"

class AlienAndSpaceshipBase
{
public:
	AlienAndSpaceshipBase();

	void GetPosition(float& a_x, float& a_y);
	void SetPosition(float a_x, float a_y);

	int GetID();
	void SetID(int a_id);

	float GetWidth();
	void SetWidth(float a_width);

	float GetHeight();
	void SetHeight(float a_height);

	float GetSpeed();
	void SetSpeed(float a_speed);

	int GetDirection();
	void SetDirection(int a_dir);

	int GetPoints();
	void SetPoints(int a_points);

protected:
	clock_t this_time;
	clock_t last_time;
	int iSpriteID;
	float iSpeed;
	float iHeight;
	float iWidth;
	int iDirection;
	int iPoints;
	Position pos;
};

#endif