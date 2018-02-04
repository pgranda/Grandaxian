#include "Alien.h"
#include "Bullet.h"
#include "UGFW.h"
#include <stdio.h>
#include <time.h>

extern float g_fDeltaTime;
extern int g_iScreenWidth;
extern int g_iScreenHeight;

void Alien::GetStartKamikazePosition(float& a_x, float& a_y)
{
	startKamikazePos.Get(a_x, a_y);
}

void Alien::SetStartKamikazePosition(float a_x, float a_y)
{
	startKamikazePos.Set(a_x, a_y);
}

bool Alien::GetIsDead()
{
	return isDead;
}

void Alien::SetIsDead(bool a_dead)
{
	isDead = a_dead;
}

float Alien::GetAngle()
{
	return fAngle;
}

void Alien::SetAngle(float a_angle)
{
	fAngle = a_angle;
}

bool Alien::GetIsGoingBackToFormation()
{
	return isGoingBackToFormation;
}

void Alien::SetIsGoingBackToFormation(bool a_goingBack)
{
	isGoingBackToFormation = a_goingBack;
}

bool Alien::GetIsKamikaze()
{
	return isKamikaze;
}

void Alien::SetIsKamikaze(bool a_kamikaze)
{
	isKamikaze = a_kamikaze;
}

int Alien::GetKamikazeDirection()
{
	return kamikazeDirection;
}

void Alien::SetKamikazeDirection(int a_direction)
{
	kamikazeDirection = a_direction;
}

bool Alien::GetKamikazeAttackPerformed()
{
	return kamikazeAttackPerformed;
}

void Alien::SetKamikazeAttackPerformed(bool a_attackPerformed)
{
	kamikazeAttackPerformed = a_attackPerformed;
}

void Alien::GetSpaceshipStartAttackPos(float& a_x, float& a_y)
{
	spaceshipStartAttackPos.Get(a_x, a_y);
}

void Alien::SetSpaceshipStartAttackPos(float a_x, float a_y)
{
	spaceshipStartAttackPos.Set(a_x, a_y);
}

void Alien::GetKamikazePointPos1(float& a_x, float& a_y)
{
	kamikazePointPos1.Get(a_x, a_y);
}

void Alien::SetKamikazePointPos1(float a_x, float a_y)
{
	kamikazePointPos1.Set(a_x, a_y);
}

void Alien::GetKamikazePointPos2(float& a_x, float& a_y)
{
	kamikazePointPos2.Get(a_x, a_y);
}

void Alien::SetKamikazePointPos2(float a_x, float a_y)
{
	kamikazePointPos2.Set(a_x, a_y);
}

Bullet* Alien::GetBullets()
{
	return bullets;
}

int Alien::GetKamikazePoints()
{
	return iKamikazePoints;
}

void Alien::SetKamikazePoints(int a_points)
{
	iKamikazePoints = a_points;
}

float Alien::GetLerpTime()
{
	return lerpTime;
}

void Alien::SetLerpTime(float a_lerptime)
{
	lerpTime = a_lerptime;
}

float Alien::GetLerpVal()
{
	return lerpVal;
}

void Alien::SetLerpVal(float a_lerpval)
{
	lerpVal = a_lerpval;
}

Position Alien::GetFormationPosition()
{
	return formationPos;
}

void Alien::SetFormationPosition(const Position& a_pos)
{
	formationPos = a_pos;
}

bool Alien::GetShouldGoBackToFormation()
{
	return shouldGoBackToFormation;
}

void Alien::SetShouldGoBackToFormation(bool a_shouldGoBack)
{
	shouldGoBackToFormation = a_shouldGoBack;
}

int Alien::GetExplosionCounter()
{
	return iExplosionCounter;
}

void Alien::SetExplosionCounter(int a_explosionCounter)
{
	iExplosionCounter = a_explosionCounter;
}

void Alien::InitialiseShooting()
{
	float bulletX = 0.f, bulletY = 0.f;
	float alienX = 0.f, alienY = 0.f;
	pos.Get(alienX, alienY);

	if (GetIsDead() == false && GetIsKamikaze() == true && alienY > 0)
	{
		for (int k = 0; k < 3; k++)
		{
			bullets[k].GetPosition(bulletX, bulletY);
			if (bullets[k].GetID() == -1)
			{
				this_time = clock();
				double time_counter = (double)(this_time - last_time);
				if (time_counter < (double)(1.0 * CLOCKS_PER_SEC))
				{
					return;
				}
				last_time = this_time;


				bulletX = alienX;
				bulletY = alienY - GetHeight() * 0.5f;

				//Initialise bullet
				bullets[k].SetPosition(bulletX, bulletY);
				bullets[k].SetID(UG::CreateSprite("./images/bullet2.png", (float)(bullets[k].GetWidth()), (float)(bullets[k].GetHeight()), true));
				UG::MoveSprite(bullets[k].GetID(), bulletX, bulletY);

				UG::DrawSprite(bullets[k].GetID());
				return;
			}
		}
	}
}

void Alien::MoveBullets()
{
	for (int k = 0; k < 3; k++)
	{
		bullets[k].Move();
	}
}

void Alien::Move(Position& a_playerPos, float* animFrames, int &aliensAbleToKamikaze)
{
	float backSpeed = 4;
	if (GetShouldGoBackToFormation() == true) //Prepare start position to move down to the formation
	{
		formationPos.x += GetDirection() * GetSpeed() * g_fDeltaTime;

		pos.Set(formationPos.x, (float)g_iScreenHeight);

		SetIsGoingBackToFormation(true);
		SetShouldGoBackToFormation(false);

		for (int i = 0; i < 3; i++)
		{
			if (bullets[i].GetID() != -1)
			{
				UG::StopDrawingSprite(bullets[i].GetID());
				UG::DestroySprite(bullets[i].GetID());
				bullets[i].SetID(-1);
			}
		}
		UG::MoveSprite(iSpriteID, pos.x, pos.y);
	}
	else if (GetIsGoingBackToFormation() == true) //Move down to the formation
	{
		Position newPos = Position(pos.x + GetDirection() * GetSpeed() * g_fDeltaTime, pos.y - GetSpeed() * backSpeed * g_fDeltaTime);
		formationPos.x = formationPos.x + GetDirection() * GetSpeed();
		if (newPos.y <= formationPos.y)
		{
			newPos.y = formationPos.y;
			SetIsGoingBackToFormation(false);
			aliensAbleToKamikaze++;
		}
		pos.Set(newPos.x, newPos.y);

		int rotateDirection = formationPos.x > pos.x ? 1 : -1;
		float angle = (pos.CalculateAngle(formationPos) - 90)* rotateDirection;
		UG::RotateSprite(iSpriteID, GetAngle() - angle);
		SetAngle(angle);

		UG::MoveSprite(iSpriteID, pos.x, pos.y);
	}
	else if (GetIsKamikaze() == false) //Standard movement calculation
	{
		pos.x = pos.x + GetDirection() * GetSpeed() * g_fDeltaTime;
		UG::MoveSprite(iSpriteID, pos.x, pos.y);
		formationPos = pos;

		if (GetExplosionCounter() == 0)
		{
			UG::SetSpriteUVCoordinates(iSpriteID, animFrames);
		}
	}
	else //Kamikaze movement calculation
	{
		formationPos.x += GetDirection() * GetSpeed() * g_fDeltaTime;

		if (GetKamikazeAttackPerformed() == false)
		{
			//we haven't begun the attack so lets gather the information we need to perform it
			//Get players position and store it
			spaceshipStartAttackPos.Set(a_playerPos.x, a_playerPos.y);
			//Get enemy currentPosition store this too
			SetStartKamikazePosition(pos.x, pos.y);

			SetKamikazePointPos1(pos.x + (GetKamikazeDirection() * 8 * 32), pos.y + (5 * 32));
			SetKamikazePointPos2(pos.x + (GetKamikazeDirection() * 5 * 32), pos.y - 32);

			//three spline points have been gathered as outlined in the steps at the top of this file
			SetLerpTime(0.f);
			SetLerpVal(0.f);
			SetKamikazeAttackPerformed(true);

			//roate us to attack facing forward
			float angle = 180;
			UG::RotateSprite(iSpriteID, angle);
			SetAngle(angle);
		}
		else
		{
			SetLerpTime(GetLerpTime() + g_fDeltaTime);
			SetLerpVal(GetLerpTime() * 0.9f);

			//a lerp value of less than 1 means we have not reached the end of our attack arc
			if (GetLerpVal() < 1.f)
			{
				//now we have to get the vectors to lerp along
				float sX = 0.f, sY = 0.f;
				GetStartKamikazePosition(sX, sY);

				float point1X = 0.f, point1Y = 0.f;
				GetKamikazePointPos1(point1X, point1Y);

				float point2X = 0.f, point2Y = 0.f;
				GetKamikazePointPos2(point2X, point2Y);

				float v1X = point1X - sX;
				float v1Y = point1Y - sY;

				float v2X = point2X - point1X;
				float v2Y = point2Y - point1Y;

				//now that we have the vectors use these with our lerp value to determine the end points for our
				//motion vector
				v1X = sX + (v1X * GetLerpVal());
				v1Y = sY + (v1Y * GetLerpVal());

				v2X = point1X + (v2X * GetLerpVal());
				v2Y = point1Y + (v2Y * GetLerpVal());

				//V1 & V2 now rep the two points of our vector
				float v3X = v2X - v1X;
				float v3Y = v2Y - v1Y;
				//update eX, eY accordingly
				pos.x = v1X + (v3X * GetLerpVal());
				pos.y = v1Y + (v3Y * GetLerpVal());

			}
			else
			{
				//we have finished the arc part of our attack 
				//now aim this enemy towards where the player was
				float spX = 0.f, spY = 0.f;
				GetSpaceshipStartAttackPos(spX, spY);
				float point2X = 0.f, point2Y = 0.f;
				GetKamikazePointPos2(point2X, point2Y);

				float vX = spX - point2X;
				float vY = spY - point2Y;

				if (vY > 0.75f)
				{

				}
				//normalise this by dividing it by its length
				float vLength = sqrtf(vX * vX + vY * vY);
				vX = vX / vLength;
				vY = vY / vLength;
				//move along the normalised vector by some delta amount
				//the numbers in the figure equate to 400 pix per second
				pos.x += (vX * 400.f * g_fDeltaTime);
				pos.y += (vY * 400.f * g_fDeltaTime);

				InitialiseShooting();
				MoveBullets();


				int rotateDirection = a_playerPos.x > pos.x ? 1 : -1;
				float angle = a_playerPos.CalculateAngle(pos) * rotateDirection;
				UG::RotateSprite(iSpriteID, GetAngle() - angle);
				SetAngle(angle);

				if (pos.y < 0)
				{
					SetShouldGoBackToFormation(true);
					SetIsKamikaze(false);
					SetKamikazeAttackPerformed(false);
					pos.x = -1;
					pos.y = -1;
				}
			}
		}
	}
	SetPosition(pos.x, pos.y);
	UG::MoveSprite(iSpriteID, pos.x, pos.y);
}

Alien::Alien()
{
	this_time = clock();
	last_time = this_time;
	iSpeed = 4;
	iHeight = 31;
	iWidth = 48;
	isDead = false;
	isGoingBackToFormation = false;
	isKamikaze = false;
	kamikazeDirection = 0;
	kamikazeAttackPerformed = false;

	for (int i = 0; i < 3; i++)
	{
		bullets[i].SetPosition(-1, -1);
	}

	fAngle = 0;
	lerpVal = 0.f;
	lerpTime = 0.f;
	shouldGoBackToFormation = false;
	iExplosionCounter = 0;
}