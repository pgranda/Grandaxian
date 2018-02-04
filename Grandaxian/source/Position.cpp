#include "Position.h"
#include <math.h>

void Position::Get(float& a_x, float& a_y)
{
	a_x = x;
	a_y = y;
}

void Position::Set(float a_x, float a_y)
{
	x = a_x;
	y = a_y;
}

void Position::Set(float a_x)
{
	x = a_x;
}

void Position::Get(float& a_x)
{
	a_x = x;
}

Position::Position()
{
	x = 0.f;
	y = 0.f;
}

Position::Position(float a_x, float a_y)
{
	x = a_x;
	y = a_y;
}

float Position::GetPointsDistance(Position a_point)
{
	return sqrtf(powf((x - a_point.x), 2) + powf((y - a_point.y), 2));
}

float Position::CalculateAngle(Position a_point)
{
	float distance = GetPointsDistance(a_point);
	float angle = (float)(acos((y - a_point.y) / distance) * 180 / 3.14f);
	return angle;
}