#ifndef _BULLET_H_
#define _BULLET_H_

#include "Position.h"

class Bullet
{
public:
	Bullet();

	void Move();

	void GetPosition(float& a_x, float& a_y);
	void SetPosition(float a_x, float a_y);

	int GetID();
	void SetID(int a_id);

	int GetWidth();

	int GetHeight();

	short GetFireKey();
	void SetFireKey(short a_key);

	int GetSpeed();
	void SetSpeed(int a_speed);

	bool GetIsFired();
	void SetIsFired(bool a_isFired);

private:
	Position pos;
	int iSpriteID;
	int iWidth;
	int iHeight;
	bool isBulletFired;
	short fireKey;
	int iSpeed;
};

#endif
