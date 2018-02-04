#ifndef _ALIEN_H_
#define _ALIEN_H_

#include <stdio.h>
#include <time.h>
#include <memory>
#include "Position.h"
#include "Bullet.h"
#include "AlienAndSpaceshipBase.h"

class Alien : public AlienAndSpaceshipBase
{
public:
	Alien();

	void Alien::Move(Position& a_playerPos, float* animFrames, int &aliensAbleToKamikaze);

	void GetStartKamikazePosition(float& a_x, float& a_y);
	void SetStartKamikazePosition(float a_x, float a_y);

	bool GetIsDead();
	void SetIsDead(bool a_dead);

	float GetAngle();
	void SetAngle(float a_angle);

	bool GetIsGoingBackToFormation();
	void SetIsGoingBackToFormation(bool a_goingBack);

	bool GetIsKamikaze();
	void SetIsKamikaze(bool a_kamikaze);

	int GetKamikazeDirection();
	void SetKamikazeDirection(int a_direction);

	bool GetKamikazeAttackPerformed();
	void SetKamikazeAttackPerformed(bool a_attackPerformed);

	void GetSpaceshipStartAttackPos(float& a_x, float& a_y);
	void SetSpaceshipStartAttackPos(float a_x, float a_y);

	void GetKamikazePointPos1(float& a_x, float& a_y);
	void SetKamikazePointPos1(float a_x, float a_y);

	void GetKamikazePointPos2(float& a_x, float& a_y);
	void SetKamikazePointPos2(float a_x, float a_y);

	Bullet* GetBullets();

	int GetKamikazePoints();
	void SetKamikazePoints(int a_points);

	float GetLerpTime();
	void SetLerpTime(float a_lerptime);

	float GetLerpVal();
	void SetLerpVal(float a_lerpval);

	void InitialiseShooting();
	void MoveBullets();

	Position GetFormationPosition();
	void SetFormationPosition(const Position& a_pos);

	bool GetShouldGoBackToFormation();
	void SetShouldGoBackToFormation(bool a_shouldGoBack);

	int GetExplosionCounter();
	void SetExplosionCounter(int a_explosionCounter);

protected:
	clock_t this_time;
	clock_t last_time;
	bool isDead;
	bool isGoingBackToFormation;
	bool shouldGoBackToFormation;
	float fAngle;
	int iKamikazePoints;
	Position startKamikazePos;
	Position spaceshipStartAttackPos;
	Position kamikazePointPos1;
	Position kamikazePointPos2;
	bool isKamikaze;
	int kamikazeDirection;
	bool kamikazeAttackPerformed;
	Bullet bullets[3];
	float lerpVal;
	float lerpTime;
	Position formationPos;
	int iExplosionCounter;
};

#endif