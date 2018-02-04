#include "Star.h"
#include "UGFW.h"
#include <random>
#include <time.h>

extern int g_iScreenHeight;
extern int g_iScreenWidth;
extern float g_fDeltaTime;

void Star::GetPosition(float& a_x, float& a_y)
{
	pos.Get(a_x, a_y);
}

void Star::SetPosition(float a_x, float a_y)
{
	pos.Set(a_x, a_y);
}

void Star::SetColour(int a_starColour)
{
	colour = a_starColour;
}

int Star::GetColour()
{
	return colour;
}

void Star::SetIsShining(bool a_shining)
{
	isShining = a_shining;
}

bool Star::GetIsShining()
{
	return isShining;
}


static bool seed()
{
	srand((unsigned int)time(NULL));
	return true;
}

float Star::GetSpeed()
{
	return iSpeed;
}

void Star::SetSpeed(float a_speed)
{
	iSpeed = a_speed;
}

void Star::Move(int a_counter)
{
	if (a_counter % 5 == 0)
	{
		isShining = !isShining;
	}

	pos.y -= iSpeed * g_fDeltaTime;

	if (pos.y < 0 || pos.y > g_iScreenHeight)
	{
		pos.y = (float)g_iScreenHeight;
		pos.x = (float)(rand() % g_iScreenWidth);
	}

	SetPosition(pos.x, pos.y);
	if (isShining == false)
	{
		UG::DrawLine((int)pos.x - 2, (int)pos.y - 2, (int)pos.x + 2, (int)pos.y + 2, colour);
		UG::DrawLine((int)pos.x - 2, (int)pos.y + 2, (int)pos.x + 2, (int)pos.y - 2, colour);
		UG::DrawLine((int)pos.x + 2, (int)pos.y - 2, (int)pos.x - 2, (int)pos.y + 2, colour);
		UG::DrawLine((int)pos.x + 2, (int)pos.y + 2, (int)pos.x - 2, (int)pos.y - 2, colour);
	}
}
Star::Star()
{
	iSpeed = 100;
	static bool seeded = seed();
	int colorOption = rand() % 4;

	switch (colorOption)
	{
	case 0:
		colour = 0xFFFFFFFF; //White
		break;
	case 1:
		colour = 0xFF0000FF; //Red
		break;
	case 2:
		colour = 0x00FF00FF; //Green
		break;
	case 3:
		colour = 0x0000FFFF; //Blue
		break;
	default:
		break;
	}

	int shiningOption = rand() % 2;
	switch (shiningOption)
	{
	case 0:
		isShining = true;
		break;
	case 1:
		isShining = false;
		break;
	default:
		break;
	}
}