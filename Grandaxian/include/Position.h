#ifndef _POSITION_H_
#define _POSITION_H_

class Position
{
public:
	Position();
	Position(float a_x, float a_y);
	void Set(float a_x, float a_y);
	void Set(float a_x);
	void Get(float& a_x);
	void Get(float& a_x, float& a_y);
	float GetPointsDistance(Position a_point);
	float CalculateAngle(Position a_point);

	float x;
	float y;
};

#endif
