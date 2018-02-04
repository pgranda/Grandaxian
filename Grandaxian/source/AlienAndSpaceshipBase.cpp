#include "AlienAndSpaceshipBase.h"
#include "Bullet.h"
#include "UGFW.h"
#include <stdio.h>
#include <time.h>

void AlienAndSpaceshipBase::GetPosition(float& a_x, float& a_y)
{
	pos.Get(a_x, a_y);
}

void AlienAndSpaceshipBase::SetPosition(float a_x, float a_y)
{
	pos.Set(a_x, a_y);
}

int AlienAndSpaceshipBase::GetID()
{
	return iSpriteID;
}

void AlienAndSpaceshipBase::SetID(int a_id)
{
	iSpriteID = a_id;
}

float AlienAndSpaceshipBase::GetWidth()
{
	return iWidth;
}

void AlienAndSpaceshipBase::SetWidth(float a_width)
{
	iWidth = a_width;

}

float AlienAndSpaceshipBase::GetHeight()
{
	return iHeight;
}

void AlienAndSpaceshipBase::SetHeight(float a_height)
{
	iHeight = a_height;

}

float AlienAndSpaceshipBase::GetSpeed()
{
	return iSpeed;
}

void AlienAndSpaceshipBase::SetSpeed(float a_speed)
{
	iSpeed = a_speed;
}

int AlienAndSpaceshipBase::GetDirection()
{
	return iDirection;
}

void AlienAndSpaceshipBase::SetDirection(int a_dir)
{
	iDirection = a_dir;
}

int AlienAndSpaceshipBase::GetPoints()
{
	return iPoints;
}

void AlienAndSpaceshipBase::SetPoints(int a_points)
{
	iPoints = a_points;
}

AlienAndSpaceshipBase::AlienAndSpaceshipBase()
{
	iSpriteID = -1;
	iDirection = 1;
	iPoints = 0;
}