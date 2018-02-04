#include "Bullet.h"
#include "UGFW.h"

extern float g_fDeltaTime;

void Bullet::GetPosition(float& a_x, float& a_y)
{
	pos.Get(a_x, a_y);
}

void Bullet::SetPosition(float a_x, float a_y)
{
	pos.Set(a_x, a_y);
}

int Bullet::GetID()
{
	return iSpriteID;
}

void Bullet::SetID(int a_id)
{
	iSpriteID = a_id;
}

int Bullet::GetWidth()
{
	return iWidth;
}

int Bullet::GetHeight()
{
	return iHeight;
}

short Bullet::GetFireKey()
{
	return fireKey;
}

void Bullet::SetFireKey(short a_key)
{
	fireKey = a_key;
}

int Bullet::GetSpeed()
{
	return iSpeed;
}

void Bullet::SetSpeed(int a_speed)
{
	iSpeed = a_speed;
}

bool Bullet::GetIsFired()
{
	return isBulletFired;
}

void Bullet::SetIsFired(bool a_isFired)
{
	isBulletFired = a_isFired;
}

void Bullet::Move()
{
	//If bullet was already shot and destroyed then do nothing
	if (iSpriteID == -1)
	{
		return;
	}

	//Move already shot bullets
	if (pos.x != -1 && pos.y != -1)
	{
		if (pos.y < 0)
		{
			//Bullet was not destroyed yet
			if (iSpriteID != -1)
			{
				UG::StopDrawingSprite(iSpriteID);
				UG::DestroySprite(iSpriteID);
				iSpriteID = -1;
			}
		}
		//Move down
		else if (pos.y > 0)
		{
			pos.y -= iSpeed * g_fDeltaTime;
			UG::MoveSprite(iSpriteID, pos.x, pos.y);
		}
	}
}

Bullet::Bullet()
{
	iSpriteID = -1;
	iWidth = 4;
	iHeight = 11;
	isBulletFired = false;
	fireKey = 0;
	iSpeed = 500;
}