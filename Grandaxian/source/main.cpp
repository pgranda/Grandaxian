////////////////////////////////////////////////////////////////////////////////////////////////////////
// Project: Grandaxian - Remake of 2D Arcade Game called Galaxian, developed by Namco in 1979			//
//																										//
// Author:  Pawel Granda																				//
//																										// 
// Date Created:  22/11/2016																			//
//																										//
// Brief:    Galaxian is based on ‘Space Invaders’ concept with a few additional gameplay elements.	    //
//           For instance the game was in colour – sprites, explosions, fonts, scrolling starfield      //
//           – the UI was using the whole RGB palette.													//
//           Enemies descending onto the player’s spaceship had different patterns of movement,			//
//           including “kamikaze” style dives, making the game unpredictable and thus more interesting. //
//           The gameplay was based on aliens attacking in swarms, with the player’s ship movement		//
//           restricted to horizontal left/right direction with only one shot on the screen at a time.	//
//																										//
// Note:     I tried to write a code which is self documenting. Please note most of my functions,		//
//           variables are indicating why they are there and what they are doing.						//
//			 I will only comment code which contains logical statements, my approaches to bite			//
//           the problem my own way with my own thinking.                                               //
//           Also please notice that for obvious reasons I am not commenting UG Framework functions.    //
//////////////////////////////////////////////////////////////////////////////////////////////////////////

#pragma region Libraries and Header Files

#include "UGFW.h"
#include "Enumerations.h"

#include <iostream>
#include <fstream>
#include <windows.h>
#include <time.h>
#include <sstream>
#include <cmath>

#include "Position.h"
#include "GameStates.h"
#include "Bullet.h"
#include "Alien.h"
#include "Spaceship.h"
#include "Star.h"
#include "HighScoresHelper.h"

#pragma endregion

#pragma region Defining Global Variables

int g_iScreenWidth = 0;
int g_iScreenHeight = 0;
float g_fAliensGap = 20;										//Gap between Aliens in a row and column
int const g_iStarsAmount = 100;									//Amount of stars appearing on a screen at the same time	
float g_fDeltaTime;
std::string g_highScoresFilePath = "..\\..\\..\\resources\\HighScores.dat";

float g_fPlayOptionY;											//Y coordinate for Play Option in Main Menu
float g_fHighscoresOptionY;										//Y coordinate for HighScores Option in Main Menu
float g_fQuitOptionY;											//Y coordinate for Quit Option in Main Menu		
float g_fPlayOptionCenterY;										//Y coordinate for center of Play Option in Main Menu		
float g_fHighscoresOptionCenterY;								//Y coordinate for center of HighScores Option in Main Menu
float g_fQuitOptionCenterY;										//Y coordinate for center HighScores Option in Main Menu

GameState g_currentState = SPLASH;								//Setting Current Game State to SPLASH window welcoming player

int const g_iRowsAliens = 3, g_iColsAliens = 10;				//Setting how many rows and columns there will be for "Normal" (blue) aliens
int const g_iRowsEliteAliens = 1, g_iColsEliteAliens = 8;		//Setting how many rows and columns there will be for "Elite" (violet) aliens
int const g_iRowsSuperAliens = 1, g_iColsSuperAliens = 6;		//Setting how many rows and columns there will be for "Super" (red) aliens
int const g_iRowsPhoenix = 1;									//Setting how many rows there will be for "Phoenix" (yellow) aliens

int g_iColsPhoenix = rand() % (4 - 2 + 1) + 2;					//Randomly assigning the number of Phoenixes that are going to appear
int g_iAliensAmount = g_iRowsAliens * g_iColsAliens;			//Counting how many "Normal" Aliens there are
int g_iEliteAmount = g_iRowsEliteAliens * g_iColsEliteAliens;	//Counting how many "Elite" Aliens there are
int g_iSuperAmount = g_iRowsSuperAliens * g_iColsSuperAliens;	//Counting how many "Super" Aliens there are
int g_iPhoenixAmount = g_iColsPhoenix * g_iRowsPhoenix;			//Counting how many "Phoenix" Aliens there are

Alien** g_Aliens;
Alien** g_EliteAliens;
Alien** g_SuperAliens;
Alien** g_PhoenixAliens;

float g_fAliensAnimationFrames[7][4] = {						//Creating an array which contains different Sprites, 
	{ 0.003f, 0.8254f, 0.193f, 1.f },							//which later on allows us to create simple animations
	{ 0.003f, 0.632f, 0.193f, 0.8113f },						//each array for different kind of Alien
	{ 0.003f, 0.4669f, 0.193f, 0.630f },						//syntax is self explanatory
	{ 0.035f, 0.f, 0.0886f, 0.085f },
	{ 0.0886f, 0.f, 0.177f, 0.1415f },
	{ 0.177f, 0.f, 0.2785f, 0.151f },
	{ 0.2785f, 0.f, 0.3797f, 0.1132f }
};

float g_fEliteAnimationFrames[7][4] = {
	{ 0.193f, 0.8254f, 0.3829f, 1.f },
	{ 0.193f, 0.632f, 0.3829f, 0.8113f },
	{ 0.193f, 0.4669f, 0.3829f, 0.630f },
	{ 0.035f, 0.f, 0.0886f, 0.085f },
	{ 0.0886f, 0.f, 0.177f, 0.1415f },
	{ 0.177f, 0.f, 0.2785f, 0.151f },
	{ 0.2785f, 0.f, 0.3797f, 0.1132f }
};

float g_fSuperAnimationFrames[7][4] = {
	{ 0.3829f, 0.8254f, 0.5696f, 1.f },
	{ 0.3829f, 0.632f, 0.5696f, 0.8113f },
	{ 0.3829f, 0.4669f, 0.5696f, 0.630f },
	{ 0.035f, 0.f, 0.0886f, 0.085f },
	{ 0.0886f, 0.f, 0.177f, 0.1415f },
	{ 0.177f, 0.f, 0.2785f, 0.151f },
	{ 0.2785f, 0.f, 0.3797f, 0.1132f }
};

float g_fPhoenixAnimationFrames[7][4] = {
	{ 0.003f, 0.151f, 0.193f, 0.4669f },
	{ 0.003f, 0.151f, 0.193f, 0.4669f },
	{ 0.003f, 0.151f, 0.193f, 0.4669f },
	{ 0.035f, 0.f, 0.0886f, 0.085f },
	{ 0.0886f, 0.f, 0.177f, 0.1415f },
	{ 0.177f, 0.f, 0.2785f, 0.151f },
	{ 0.2785f, 0.f, 0.3797f, 0.1132f }
};


Spaceship l_spaceship;									//Creating an object of a Spaceship class
Bullet l_bullet;										//Creating an object of a Bullet class
Star stars[g_iStarsAmount];								//Creating an array object of a Star class containing our 100 stars
Spaceship l_spaceshipLives;								//Creating an object of a Spaceship class - simple life indicator
std::string nick = "";									//Creating a string which will later contain our Players Nickname

#pragma endregion

#pragma region Spaceship Functions

void InitialiseSpaceship(Spaceship& a_spaceship, const char* a_spaceshipImageFilename, float a_xPos, float a_yPos);
void SetSpaceshipSpeed(Spaceship& a_spaceship, float a_iSpeed);
void SetSpaceshipMovementKeys(Spaceship& a_spaceship, short a_leftArrowKey, short a_rightArrowKey);
void MoveSpaceship(Spaceship& a_spaceship);

#pragma endregion

#pragma region Bullet Functions

void InitialiseBullet(Bullet& a_bullet, const char* a_bulletImageFilename, float a_xPos, float a_yPos);
void SetBulletFireKey(Bullet& a_bullet, short a_fireKey);
void MoveSpaceshipBullet(Bullet& a_bullet, Spaceship a_spaceship);

#pragma endregion

#pragma region Alien Functions

void InitialiseAliens(Alien **a_aliens, int a_rows, int a_cols, const char* a_alienImageFilename,
	float a_xPos, float a_yPos, int a_points, int a_kamikazePoints, float a_animationFrames[7][4]);
void SetAliensSpeed(Alien **a_aliens, int a_rows, int a_cols, float a_speed);
void MoveAllAliens(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens, Spaceship& a_spaceship,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens,
	int a_index);
void MoveAliens(Alien **a_aliens, Spaceship& a_spaceship, int a_rows, int a_cols, int a_index,
	float a_animationFrames[7][4], int &aliensAbleToKamikaze);
int GetAliensMoveDirection(Alien **a_aliens, int a_rows, int a_cols);
void SetAliensDirection(Alien **a_aliens, int a_rows, int a_cols, int a_direction);
void AnimateAliensExplosion(Alien **a_aliens, int a_rows, int a_cols, float a_animationFrames[7][4]);
void Hitmark(Alien **a_aliens, int a_rows, int a_cols, Bullet& a_bullet, Spaceship& a_spaceship,
	int &ableToKamikazeAmount, float a_animationFrames[7][4]);
void InitialiseKamikaze(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens);
void GetRandomAlienAndStartKamikazeIfPossible(Alien **a_aliens, Alien **a_eliteAliens,
	int a_rowsAliens, int a_colsAliens, int a_rowsEliteAliens);
void GetRandomEliteAlienAndStartKamikazeIfPossible(Alien **a_eliteAliens, Alien **a_aliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsAliens, int a_colsAliens);
void GetRandomSuperAlienAndStartKamikazeIfPossible(Alien **a_superAliens, Alien **a_phoenixAliens, Alien **a_eliteAliens, Alien **a_aliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_colsPhoenixAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsAliens, int a_colsAliens);
void GetRandomPhoenixAlienAndStartKamikazeIfPossible(Alien **a_superAliens, Alien **a_phoenixAliens,
	int a_colsSuperAliens, int a_colsPhoenixAliens);
void InitialiseStartKamikazeData(Alien **a_aliens, int a_i, int a_j, int a_direction);
int CalculateKamikazeDirection(Alien **a_aliens, int a_rowsAliens, int a_colsAliens, int a_i, int a_j);
int RandomKamikazeDirection();
bool CheckIfPhoenixesAvailable(Alien **a_phoenixAliens, int a_colsPhoenixAliens);
void DestroyAliensSprites(Alien **a_aliens, int a_rowsAliens, int a_colsAliens);
void ResetAliens();
bool CheckIfAllAliensAreDead(Alien **a_aliens, int a_rows, int a_cols);
bool CheckForWin(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens, Spaceship& a_spaceship,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens);

#pragma endregion

#pragma region HUD Functions

void InitialiseScreenProperties();
void DisplayMenu(Star* a_stars, int a_starsCounter);
void InitialiseStarsPositions(Star *a_stars);
void MoveStars(Star *a_stars, int a_starsCounter);
void InitialiseSpaceshipLives(Spaceship& a_spaceshipLives, const char* a_spaceshipLivesImageFilename, float a_xPos, float a_yPos);
void InitialiseGameplayAssets(Spaceship& a_spaceship, Spaceship& a_spaceshipLives,
	Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens,
	Bullet& a_bullet, int a_phoenixAmount);
void DisplayGameplayHeader(Spaceship& l_spaceship);
void DisplayStarsOnGameover(Star* a_stars);
void GetUserInput();

#pragma endregion


int main()
{
	//Seeding rand with a random number
	srand((unsigned int)time(nullptr));

#pragma region Game Creation

	if (UG::Create(1024, 1000, false, "Grandaxian", 100, 100))
	{
		InitialiseScreenProperties();
		InitialiseStarsPositions(stars);

		float fFrameTime = 0.2f;
		float fExplosionFrameTime = 0.075f;

#pragma endregion

		//Galaxian logo sprite
		int iLogoID = UG::CreateSprite("./images/galaxianlogo.png", 800, 240, true);
		UG::MoveSprite(iLogoID, g_iScreenWidth * 0.5f, g_iScreenHeight * 0.75f);
		UG::DrawSprite(iLogoID);

		//Spaceship sprite which indicates current choice in the main menu
		float fIndicatorX = g_iScreenWidth * 0.32f, fIndicatorY = g_fPlayOptionY - 11;
		int iIndicatorID = UG::CreateSprite("./images/spaceship2.png", 50, 64, true);

		int iMenuKeyCounter = 0;               //Auxillary Variable used to slow down the process of choosing an option in Main Menu
		int iStarsCounter = 0;
		int iIndex = 0;                        //Variable used to iterate through Sprites Animation
		int iExplosionCounter = 0;
		int iPreviousExplosionCounter = 0;
		int iStartMenuOptionCounter = 0;
		int tmp1ID = -1, tmp2ID = -1, tmp3ID = -1, tmp4ID = -1;   //Variables containing Sprites IDs used for START screen
		do
		{
			g_fDeltaTime = UG::GetDeltaTime();
			fFrameTime -= g_fDeltaTime;
			fExplosionFrameTime -= g_fDeltaTime;

			if (fFrameTime < 0.f)
			{
				iIndex++;
				iStarsCounter++;
				iIndex = iIndex % 3;
				fFrameTime = 0.2f;
			}

			iPreviousExplosionCounter = iExplosionCounter;
			if (fExplosionFrameTime < 0.f)
			{
				iExplosionCounter++;
				fExplosionFrameTime = 0.075f;
			}


			UG::ClearScreen();

			switch (g_currentState)
			{
			case SPLASH:
			{
				UG::DrawString("PROJECT GRANDAXIAN", (int)(g_iScreenWidth * 0.35), (int)(g_iScreenHeight * 0.5), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("REMAKE BY PAWEL GRANDA", (int)(g_iScreenWidth * 0.30), (int)(g_iScreenHeight * 0.35), 1.0f, UG::SColour(174, 0, 255, 255));
				UG::DrawString("PRESS G TO CONTINUE", (int)(g_iScreenWidth * 0.337), (int)(g_iScreenHeight * 0.20), 1.0f);
				if (UG::IsKeyDown(UG::KEY_G))
				{
					g_currentState = MENU;
					UG::MoveSprite(iIndicatorID, fIndicatorX, fIndicatorY);
					UG::DrawSprite(iIndicatorID);
				}

				break;
			}
			case MENU:
			{
				DisplayMenu(stars, iStarsCounter);

				if (UG::IsKeyDown(UG::KEY_DOWN))
				{
					if (iMenuKeyCounter > 8)
					{
						if (fIndicatorY == g_fQuitOptionCenterY)
						{
							break;
						}
						fIndicatorY -= g_iScreenHeight * 0.08f;
						iMenuKeyCounter = 0;
					}
					iMenuKeyCounter++;
				}
				else if (UG::IsKeyDown(UG::KEY_UP))
				{
					if (iMenuKeyCounter > 8)
					{
						if (fIndicatorY == g_fPlayOptionCenterY)
						{
							break;
						}
						fIndicatorY += g_iScreenHeight * 0.08f;
						iMenuKeyCounter = 0;
					}
					iMenuKeyCounter++;
				}

				UG::MoveSprite(iIndicatorID, fIndicatorX, fIndicatorY);

				if (UG::IsKeyDown(UG::KEY_ENTER))
				{
					UG::StopDrawingSprite(iIndicatorID);

					if (fIndicatorY == g_fPlayOptionCenterY)
					{
						g_currentState = START;
						UG::StopDrawingSprite(iLogoID);
					}
					else if (fIndicatorY == g_fHighscoresOptionCenterY)
					{
						g_currentState = HIGHSCORES;
					}
					else if (fIndicatorY == g_fQuitOptionCenterY)
					{
						g_currentState = QUIT;
					}
				}
				break;
			}
			case START:
			{
				DisplayGameplayHeader(l_spaceship);
				UG::DrawString("WE ARE THE GALAXIANS", (int)(g_iScreenWidth * 0.35), (int)(g_iScreenHeight * 0.85), 1.25f, UG::SColour(255, 0, 0, 255));
				UG::DrawString("MISSION: DESTROY ALIENS", (int)(g_iScreenWidth * 0.32), (int)(g_iScreenHeight * 0.75), 1.25f, UG::SColour(255, 0, 0, 255));
				UG::DrawString("- SCORE ADVANCE TABLE -", (int)(g_iScreenWidth * 0.32), (int)(g_iScreenHeight * 0.65), 1.25f);
				UG::DrawString("CONVOY           CHARGER", (int)(g_iScreenWidth * 0.398), (int)(g_iScreenHeight * 0.55), 1.0f, UG::SColour(0, 138, 255, 255));

				UG::DrawString("60", (int)(g_iScreenWidth * 0.425), (int)(g_iScreenHeight * 0.485), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("50", (int)(g_iScreenWidth * 0.425), (int)(g_iScreenHeight * 0.41), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("40", (int)(g_iScreenWidth * 0.425), (int)(g_iScreenHeight * 0.335), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("30", (int)(g_iScreenWidth * 0.425), (int)(g_iScreenHeight * 0.26), 1.0f, UG::SColour(0, 138, 255, 255));

				UG::DrawString("150   PTS", (int)(g_iScreenWidth * 0.585), (int)(g_iScreenHeight * 0.485), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("100   PTS", (int)(g_iScreenWidth * 0.585), (int)(g_iScreenHeight * 0.41), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("80    PTS", (int)(g_iScreenWidth * 0.585), (int)(g_iScreenHeight * 0.335), 1.0f, UG::SColour(0, 138, 255, 255));
				UG::DrawString("60    PTS", (int)(g_iScreenWidth * 0.585), (int)(g_iScreenHeight * 0.26), 1.0f, UG::SColour(0, 138, 255, 255));
				if (iStartMenuOptionCounter == 0)
				{
					tmp1ID = UG::CreateSprite("./images/phoenix.png", 61, 55, true);
					tmp2ID = UG::CreateSprite("./images/alien3a.png", 60, 39, true);
					tmp3ID = UG::CreateSprite("./images/alien2a.png", 60, 39, true);
					tmp4ID = UG::CreateSprite("./images/alien1a.png", 60, 39, true);
					UG::MoveSprite(tmp1ID, g_iScreenWidth * 0.35f, g_iScreenHeight * 0.475f);
					UG::MoveSprite(tmp2ID, g_iScreenWidth * 0.35f, g_iScreenHeight * 0.4f);
					UG::MoveSprite(tmp3ID, g_iScreenWidth * 0.35f, g_iScreenHeight * 0.325f);
					UG::MoveSprite(tmp4ID, g_iScreenWidth * 0.35f, g_iScreenHeight * 0.25f);
					UG::DrawSprite(tmp1ID);
					UG::DrawSprite(tmp2ID);
					UG::DrawSprite(tmp3ID);
					UG::DrawSprite(tmp4ID);
				}
				iStartMenuOptionCounter++;

				UG::DrawString("PRESS P TO PLAY", (int)(g_iScreenWidth * 0.41), (int)(g_iScreenHeight * 0.15), 1.0f);
				if (UG::IsKeyDown(UG::KEY_P))
				{
					g_currentState = GAMEPLAY;
					g_iColsPhoenix = rand() % (4 - 2 + 1) + 2;
					ResetAliens();
					InitialiseGameplayAssets(l_spaceship, l_spaceshipLives, g_Aliens, g_EliteAliens, g_SuperAliens, g_PhoenixAliens, l_bullet, g_iColsPhoenix);
					if (tmp1ID != -1 && tmp2ID != -1 && tmp3ID != -1 && tmp4ID != -1)
					{
						UG::StopDrawingSprite(tmp1ID);
						UG::StopDrawingSprite(tmp2ID);
						UG::StopDrawingSprite(tmp3ID);
						UG::StopDrawingSprite(tmp4ID);
						UG::DestroySprite(tmp1ID);
						UG::DestroySprite(tmp2ID);
						UG::DestroySprite(tmp3ID);
						UG::DestroySprite(tmp4ID);
					}
					iStartMenuOptionCounter = 0;
				}
				break;
			}
			case GAMEPLAY:
			{

#pragma region Gameplay functions

				MoveSpaceship(l_spaceship);
				MoveSpaceshipBullet(l_bullet, l_spaceship);

				MoveAllAliens(g_Aliens, g_EliteAliens, g_SuperAliens, g_PhoenixAliens, l_spaceship,
					g_iRowsAliens, g_iColsAliens,
					g_iRowsEliteAliens, g_iColsEliteAliens,
					g_iRowsSuperAliens, g_iColsSuperAliens,
					g_iRowsPhoenix, g_iColsPhoenix,
					iIndex);

				Hitmark(g_Aliens, g_iRowsAliens, g_iColsAliens, l_bullet, l_spaceship, g_iAliensAmount, g_fAliensAnimationFrames);
				Hitmark(g_EliteAliens, g_iRowsEliteAliens, g_iColsEliteAliens, l_bullet, l_spaceship, g_iEliteAmount, g_fEliteAnimationFrames);
				Hitmark(g_SuperAliens, g_iRowsSuperAliens, g_iColsSuperAliens, l_bullet, l_spaceship, g_iSuperAmount, g_fSuperAnimationFrames);
				Hitmark(g_PhoenixAliens, g_iRowsPhoenix, g_iColsPhoenix, l_bullet, l_spaceship, g_iPhoenixAmount, g_fPhoenixAnimationFrames);
				bool levelEnd = CheckForWin(g_Aliens, g_EliteAliens, g_SuperAliens, g_PhoenixAliens, l_spaceship,
					g_iRowsAliens, g_iColsAliens,
					g_iRowsEliteAliens, g_iColsEliteAliens,
					g_iRowsSuperAliens, g_iColsSuperAliens,
					g_iRowsPhoenix, g_iColsPhoenix);

				if (iExplosionCounter != iPreviousExplosionCounter)
				{
					AnimateAliensExplosion(g_Aliens, g_iRowsAliens, g_iColsAliens, g_fAliensAnimationFrames);
					AnimateAliensExplosion(g_EliteAliens, g_iRowsEliteAliens, g_iColsEliteAliens, g_fEliteAnimationFrames);
					AnimateAliensExplosion(g_SuperAliens, g_iRowsSuperAliens, g_iColsSuperAliens, g_fSuperAnimationFrames);
					AnimateAliensExplosion(g_PhoenixAliens, g_iRowsPhoenix, g_iColsPhoenix, g_fPhoenixAnimationFrames);
				}

				MoveStars(stars, iStarsCounter);

				DisplayGameplayHeader(l_spaceship);

				if (levelEnd)
				{
					g_currentState = GAMEOVER;
				}
#pragma endregion

				break;
			}
			case HIGHSCORES:
			{
				HighScoresHelper::DisplayHighscores();

				if (UG::IsKeyDown(UG::KEY_ESCAPE))
				{
					g_currentState = MENU;
					UG::DrawSprite(iIndicatorID);
				}
				break;
			}
			case GAMEOVER:
			{
				DisplayGameplayHeader(l_spaceship);
				DisplayStarsOnGameover(stars);

				UG::DrawString("GAME OVER", (int)(g_iScreenWidth * 0.4185), (int)(g_iScreenHeight * 0.5), 1.0f, UG::SColour(255, 0, 0, 255));
				UG::DrawString("PRESS SPACE TO CONTINUE...", (int)(g_iScreenWidth * 0.31), (int)(g_iScreenHeight * 0.45), 1.0f);

				if (UG::IsKeyDown(UG::KEY_SPACE))
				{
					UG::DrawSprite(iLogoID);

					UG::StopDrawingSprite(l_spaceship.GetID());
					UG::StopDrawingSprite(l_bullet.GetID());
					UG::StopDrawingSprite(l_spaceshipLives.GetID());
					UG::DestroySprite(l_spaceship.GetID());
					UG::DestroySprite(l_bullet.GetID());
					UG::DestroySprite(l_spaceshipLives.GetID());
					DestroyAliensSprites(g_Aliens, g_iRowsAliens, g_iColsAliens);
					DestroyAliensSprites(g_EliteAliens, g_iRowsEliteAliens, g_iColsEliteAliens);
					DestroyAliensSprites(g_SuperAliens, g_iRowsSuperAliens, g_iColsSuperAliens);
					DestroyAliensSprites(g_PhoenixAliens, g_iRowsPhoenix, g_iColsPhoenix);

					g_currentState = PROVIDINGNICK;
				}
				break;
			}
			case PROVIDINGNICK:
			{
				GetUserInput();
				if (UG::IsKeyDown(UG::KEY_ENTER))
				{
					HighScoresHelper::SaveHighscores(nick, l_spaceship.GetPoints());
					nick = "";
					g_currentState = HIGHSCORES;
				}
				break;
			}
			case QUIT:
			{
				UG::Close();
				break;
			}
			default:
				break;
			}
			UG::SetFont(nullptr);

		} while (UG::Process());

		UG::Dispose();
	}
	return 0;
}

#pragma region Functions 

void InitialiseGameplayAssets(Spaceship& a_spaceship, Spaceship& a_spaceshipLives,
	Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens,
	Bullet& a_bullet,
	int a_phoenixAmount)
{

#pragma region Game Assets Variables

	float fPXSpaceship, fPYSpaceship;
	float aliensHeight = a_aliens[0][0].GetHeight();
	float eliteAliensHeight = a_eliteAliens[0][0].GetHeight();
	float aliensWidth = a_aliens[0][0].GetWidth();
	float superAliensHeight = a_superAliens[0][0].GetHeight();
	float eliteAliensWidth = a_eliteAliens[0][0].GetWidth();
	float phoenixAliensHeight = a_phoenixAliens[0][0].GetHeight();
	float superAliensWidth = a_superAliens[0][0].GetWidth();

#pragma endregion

#pragma region Game Assets Initialisation

	InitialiseSpaceship(a_spaceship, "./images/spaceship2.png", g_iScreenWidth * 0.5f, g_iScreenHeight * 0.1f);
	SetSpaceshipMovementKeys(a_spaceship, UG::KEY_LEFT, UG::KEY_RIGHT);
	SetSpaceshipSpeed(a_spaceship, 500);
	a_spaceship.SetPoints(0);
	UG::DrawSprite(a_spaceship.GetID());

	InitialiseSpaceshipLives(a_spaceshipLives, "./images/spaceship.png", g_iScreenWidth * 0.10f, g_iScreenHeight * 0.05f);
	UG::DrawSprite(a_spaceshipLives.GetID());

	a_spaceship.GetPosition(fPXSpaceship, fPYSpaceship);
	InitialiseBullet(a_bullet, "./images/bullet.png", fPXSpaceship, fPYSpaceship + a_spaceship.GetHeight() * 0.5f + a_bullet.GetHeight() * 0.5f);
	SetBulletFireKey(a_bullet, UG::KEY_SPACE);
	a_bullet.SetSpeed(750);
	UG::DrawSprite(a_bullet.GetID());

	InitialiseAliens(a_aliens, g_iRowsAliens, g_iColsAliens, "./images/alien1a.png", g_iScreenWidth * 0.25f, g_iScreenHeight * 0.75f,
		30, 60, g_fAliensAnimationFrames);
	SetAliensSpeed(a_aliens, g_iRowsAliens, g_iColsAliens, 100.f);

	InitialiseAliens(a_eliteAliens, g_iRowsEliteAliens, g_iColsEliteAliens, "./images/alien2a.png", g_iScreenWidth * 0.25f + aliensWidth + g_fAliensGap, g_iScreenHeight * 0.75f + g_fAliensGap + aliensHeight * 0.5f + eliteAliensHeight * 0.5f,
		40, 80, g_fEliteAnimationFrames);
	SetAliensSpeed(a_eliteAliens, g_iRowsEliteAliens, g_iColsEliteAliens, 100.f);

	InitialiseAliens(a_superAliens, g_iRowsSuperAliens, g_iColsSuperAliens, "./images/alien3a.png", g_iScreenWidth * 0.25f + aliensWidth + eliteAliensWidth + g_fAliensGap * 2, g_iScreenHeight * 0.75f + g_fAliensGap * 2 + aliensHeight + eliteAliensHeight * 0.5f + superAliensHeight * 0.5f,
		50, 100, g_fSuperAnimationFrames);
	SetAliensSpeed(a_superAliens, g_iRowsSuperAliens, g_iColsSuperAliens, 100.f);

	InitialiseAliens(a_phoenixAliens, g_iRowsPhoenix, a_phoenixAmount, "./images/phoenix.png", g_iScreenWidth * 0.25f + aliensWidth + eliteAliensWidth + superAliensWidth + g_fAliensGap * 3, g_iScreenHeight * 0.75f + g_fAliensGap * 3 + aliensHeight + eliteAliensHeight + superAliensHeight * 0.5f + phoenixAliensHeight * 0.5f,
		60, 150, g_fPhoenixAnimationFrames);
	SetAliensSpeed(a_phoenixAliens, g_iRowsPhoenix, a_phoenixAmount, 100.f);

#pragma endregion

}

void InitialiseSpaceship(Spaceship& a_spaceship, const char* a_spaceshipImageFilename, float a_xPos, float a_yPos)
{
	a_spaceship.SetPosition(a_xPos, a_yPos);
	a_spaceship.SetLives(1);
	a_spaceship.SetID(UG::CreateSprite(a_spaceshipImageFilename, (float)(a_spaceship.GetWidth()), (float)(a_spaceship.GetHeight()), true));
	UG::MoveSprite(a_spaceship.GetID(), a_xPos, a_yPos);
}

void SetSpaceshipSpeed(Spaceship& a_spaceship, float a_iSpeed)
{
	a_spaceship.SetSpeed(a_iSpeed);
}

void SetSpaceshipMovementKeys(Spaceship& a_spaceship, short a_leftArrowKey, short a_rightArrowKey)
{
	a_spaceship.SetMoveLeftKey(a_leftArrowKey);
	a_spaceship.SetMoveRightKey(a_rightArrowKey);
}

void MoveSpaceship(Spaceship& a_spaceship)
{
	int iDirection = 0;

	if (UG::IsKeyDown(a_spaceship.GetMoveLeftKey()))
	{
		iDirection = -1;
	}
	if (UG::IsKeyDown(a_spaceship.GetMoveRightKey()))
	{
		iDirection = 1;
	}
	float pX = 0.f, pY = 0.f;
	a_spaceship.GetPosition(pX, pY);

	float newPositionX = pX + iDirection * a_spaceship.GetSpeed() * g_fDeltaTime;		//Calculating new position left/right for spaceship
	pX = newPositionX;

	if (pX >= (a_spaceship.GetWidth() * 0.5f) && pX <= (g_iScreenWidth - a_spaceship.GetWidth() * 0.5f))
	{
		UG::MoveSprite(a_spaceship.GetID(), pX, pY);
		a_spaceship.SetPosition(pX, pY);
	}

}

void InitialiseBullet(Bullet& a_bullet, const char* a_bulletImageFilename, float a_xPos, float a_yPos)
{
	a_bullet.SetPosition(a_xPos, a_yPos);
	a_bullet.SetID(UG::CreateSprite(a_bulletImageFilename, (float)(a_bullet.GetWidth()), (float)(a_bullet.GetHeight()), true));
	UG::MoveSprite(a_bullet.GetID(), a_xPos, a_yPos);
}

void SetBulletFireKey(Bullet& a_bullet, short a_fireKey)
{
	a_bullet.SetFireKey(a_fireKey);
}

void MoveSpaceshipBullet(Bullet& a_bullet, Spaceship a_spaceship)
{
	if (a_bullet.GetIsFired() == false)					//Move bullet with spaceship
	{
		float x, y;
		a_spaceship.GetPosition(x, y);
		a_bullet.SetPosition(x, y + a_spaceship.GetHeight() * 0.5f + a_bullet.GetHeight() * 0.5f);
		UG::MoveSprite(a_bullet.GetID(), x, y + a_spaceship.GetHeight() * 0.5f + a_bullet.GetHeight() * 0.5f);
	}

	if (UG::IsKeyDown(a_bullet.GetFireKey()))			//Set that bullet was shot
	{
		if (a_bullet.GetIsFired() == false)
		{
			a_bullet.SetIsFired(true);
		}
	}

	if (a_bullet.GetIsFired() == true)					//Move bullet that was shot
	{
		float x, y;
		a_bullet.GetPosition(x, y);
		if (y > g_iScreenHeight)
		{
			a_bullet.SetIsFired(false);
			return;
		}

		y = y + a_bullet.GetSpeed() * g_fDeltaTime;		//Calculating new position for bullet
		UG::MoveSprite(a_bullet.GetID(), x, y);
		a_bullet.SetPosition(x, y);
	}



}

void InitialiseAliens(Alien **a_aliens, int a_rows, int a_cols, const char* a_alienImageFilename, float a_xPos, float a_yPos, int a_points, int a_kamikazePoints, float a_animationFrames[7][4])
{
	float centre[2] = { 0.5f, 0.5f };

	float x = a_xPos;
	float y = a_yPos;
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			float size[2] = { (float)(a_aliens[i][j].GetWidth()), (float)(a_aliens[i][j].GetHeight()) };
			a_aliens[i][j].SetPosition(x, y);
			a_aliens[i][j].SetPoints(a_points);
			a_aliens[i][j].SetKamikazePoints(a_kamikazePoints);
			a_aliens[i][j].SetID(UG::CreateSprite("./images/Galaxian.png", size, centre, a_animationFrames[0]));
			UG::MoveSprite(a_aliens[i][j].GetID(), x, y);
			UG::DrawSprite(a_aliens[i][j].GetID());
			x += a_aliens[i][j].GetWidth() + g_fAliensGap;
		}
		x = a_xPos;
		y -= a_aliens[i][0].GetHeight() + g_fAliensGap;
	}
}

void SetAliensSpeed(Alien **a_aliens, int a_rows, int a_cols, float a_speed)
{
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			a_aliens[i][j].SetSpeed(a_speed);
		}
	}
}

bool CheckIfAllAliensAreDead(Alien **a_aliens, int a_rows, int a_cols)
{
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			if (a_aliens[i][j].GetIsDead() == false)
			{
				return false;
			}
		}
	}
	return true;
}

bool CheckForWin(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens, Spaceship& a_spaceship,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens)
{
	bool allAliensDead = CheckIfAllAliensAreDead(a_aliens, a_rowsAliens, a_colsAliens);
	allAliensDead = allAliensDead && CheckIfAllAliensAreDead(a_eliteAliens, a_rowsEliteAliens, a_colsEliteAliens);
	allAliensDead = allAliensDead && CheckIfAllAliensAreDead(a_superAliens, a_rowsSuperAliens, a_colsSuperAliens);
	allAliensDead = allAliensDead && CheckIfAllAliensAreDead(a_phoenixAliens, a_rowsPhoenixAliens, a_colsPhoenixAliens);
	return allAliensDead;
}

void MoveAllAliens(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens, Spaceship& a_spaceship,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens,
	int a_index)
{
	int direction1 = GetAliensMoveDirection(a_aliens, a_rowsAliens, a_colsAliens);
	int direction2 = GetAliensMoveDirection(a_eliteAliens, a_rowsEliteAliens, a_colsEliteAliens);
	int direction3 = GetAliensMoveDirection(a_superAliens, a_rowsSuperAliens, a_colsSuperAliens);
	int direction4 = GetAliensMoveDirection(a_phoenixAliens, a_rowsPhoenixAliens, a_colsPhoenixAliens);
	int direction = 0;

	if (direction1 != 0)
	{
		direction = direction1;
	}
	else if (direction2 != 0)
	{
		direction = direction2;
	}
	else if (direction3 != 0)
	{
		direction = direction3;
	}
	else if (direction4 != 0)
	{
		direction = direction4;
	}

	if (direction != 0) //If move direction has changed then set new direction for all aliens
	{
		SetAliensDirection(a_aliens, a_rowsAliens, a_colsAliens, direction);
		SetAliensDirection(a_eliteAliens, a_rowsEliteAliens, a_colsEliteAliens, direction);
		SetAliensDirection(a_superAliens, a_rowsSuperAliens, a_colsSuperAliens, direction);
		SetAliensDirection(a_phoenixAliens, a_rowsPhoenixAliens, a_colsPhoenixAliens, direction);
	}
	else
	{
		//Do nothing
	}

	MoveAliens(a_aliens, a_spaceship, a_rowsAliens, a_colsAliens, a_index, g_fAliensAnimationFrames, g_iAliensAmount);
	MoveAliens(a_eliteAliens, a_spaceship, a_rowsEliteAliens, a_colsEliteAliens, a_index, g_fEliteAnimationFrames, g_iEliteAmount);
	MoveAliens(a_superAliens, a_spaceship, a_rowsSuperAliens, a_colsSuperAliens, a_index, g_fSuperAnimationFrames, g_iSuperAmount);
	MoveAliens(a_phoenixAliens, a_spaceship, a_rowsPhoenixAliens, a_colsPhoenixAliens, a_index, g_fPhoenixAnimationFrames, g_iPhoenixAmount);

	if (a_index == 0)
	{
		InitialiseKamikaze(a_aliens, a_eliteAliens, a_superAliens, a_phoenixAliens, a_rowsAliens, a_colsAliens,
			a_rowsEliteAliens, a_colsEliteAliens, a_rowsSuperAliens, a_colsSuperAliens, a_rowsPhoenixAliens, a_colsPhoenixAliens);
	}
}

void InitialiseKamikaze(Alien **a_aliens, Alien **a_eliteAliens, Alien **a_superAliens, Alien **a_phoenixAliens,
	int a_rowsAliens, int a_colsAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_rowsPhoenixAliens, int a_colsPhoenixAliens)
{
	int random = rand() % 5;
	int movementOptionAliens = g_iAliensAmount != 0 ? random % (g_iAliensAmount) : 1;
	int movementOptionElite = g_iEliteAmount != 0 ? random % (g_iEliteAmount * 2) : 1;			//Magic Numbers for Kamikaze
	int movementOptionSuper = g_iSuperAmount != 0 ? random % (g_iSuperAmount * 960) : 1;		//to occur less frequent
	int movementOptionPhoenix = g_iPhoenixAmount != 0 ? random % (g_iPhoenixAmount * 960) : 1;

	switch (movementOptionAliens)
	{
	case 0:
		GetRandomAlienAndStartKamikazeIfPossible(a_aliens, a_eliteAliens, a_rowsAliens, a_colsAliens, a_rowsEliteAliens);
		break;
	default:
		break;
	}

	switch (movementOptionElite)
	{
	case 0:
		GetRandomEliteAlienAndStartKamikazeIfPossible(a_eliteAliens, a_aliens, a_rowsEliteAliens, a_colsEliteAliens, a_rowsAliens, a_colsAliens);
		break;
	default:
		break;
	}

	switch (movementOptionSuper)
	{
	case 0:
		GetRandomSuperAlienAndStartKamikazeIfPossible(a_superAliens, a_phoenixAliens, a_eliteAliens, a_aliens,
			a_rowsSuperAliens, a_colsSuperAliens, a_colsPhoenixAliens, a_rowsEliteAliens, a_colsEliteAliens, a_rowsAliens, a_colsAliens);
		break;
	default:
		break;
	}
	switch (movementOptionPhoenix)
	{
	case 0:
		GetRandomPhoenixAlienAndStartKamikazeIfPossible(a_superAliens, a_phoenixAliens, a_colsSuperAliens, a_colsPhoenixAliens);
		break;
	default:
		break;
	}
}

void GetRandomAlienAndStartKamikazeIfPossible(Alien **a_aliens, Alien **a_eliteAliens, int a_rowsAliens, int a_colsAliens, int a_rowsEliteAliens)
{
	int i = rand() % a_rowsAliens;
	int j = rand() % a_colsAliens;
	if (a_aliens[i][j].GetIsKamikaze() == false && a_aliens[i][j].GetIsDead() == false)
	{
		int direction = CalculateKamikazeDirection(a_aliens, a_rowsAliens, a_colsAliens, i, j);
		if (direction == 0)
		{
			return;
		}

		//Check if there is free space above
		if (i > 0 && !(a_aliens[i - 1][j].GetIsKamikaze() == true || a_aliens[i - 1][j].GetIsDead() == true))
		{
			return;
		}
		else if (i == 0 && (j == 0 || j == a_colsAliens - 1))
		{
			//Do nothing, there is no alien above
		}
		else if (i == 0 && !(a_eliteAliens[a_rowsEliteAliens - 1][j - 1].GetIsKamikaze() == true || a_eliteAliens[a_rowsEliteAliens - 1][j - 1].GetIsDead() == true))
		{
			return;
		}

		InitialiseStartKamikazeData(a_aliens, i, j, direction);
		g_iAliensAmount--;
	}
}

void GetRandomEliteAlienAndStartKamikazeIfPossible(Alien **a_eliteAliens, Alien **a_aliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsAliens, int a_colsAliens)
{
	int i = rand() % a_rowsEliteAliens;
	int j = rand() % a_colsEliteAliens;
	if (a_eliteAliens[i][j].GetIsKamikaze() == false && a_eliteAliens[i][j].GetIsDead() == false)
	{
		int direction = CalculateKamikazeDirection(a_eliteAliens, a_rowsEliteAliens, a_colsEliteAliens, i, j);
		if (direction == 0)
		{
			return;
		}

		int colToCheck;
		if (direction == -1)
		{
			colToCheck = j;
		}
		else //Direction == 1
		{
			colToCheck = j + 2;
		}

		for (int k = 0; k < a_rowsAliens; k++)
		{
			if (a_aliens[k][colToCheck].GetIsKamikaze() == false && a_aliens[k][colToCheck].GetIsDead() == false)
			{
				return;
			}
		}

		InitialiseStartKamikazeData(a_eliteAliens, i, j, direction);
		g_iEliteAmount--;
	}
}

void GetRandomSuperAlienAndStartKamikazeIfPossible(Alien **a_superAliens, Alien **a_phoenixAliens, Alien **a_eliteAliens, Alien **a_aliens,
	int a_rowsSuperAliens, int a_colsSuperAliens,
	int a_colsPhoenixAliens,
	int a_rowsEliteAliens, int a_colsEliteAliens,
	int a_rowsAliens, int a_colsAliens)
{
	bool anyPhoenixAvailable = CheckIfPhoenixesAvailable(a_phoenixAliens, a_colsPhoenixAliens);
	if (anyPhoenixAvailable == true)
	{
		return;
	}

	int i = rand() % a_rowsSuperAliens;
	int j = rand() % a_colsSuperAliens;
	if (a_superAliens[i][j].GetIsKamikaze() == false && a_superAliens[i][j].GetIsDead() == false)
	{
		int direction = CalculateKamikazeDirection(a_superAliens, a_rowsSuperAliens, a_colsSuperAliens, i, j);
		if (direction == 0)
		{
			return;
		}

		int eliteAliensColToCheck, aliensColToCheck;
		if (direction == -1)
		{
			eliteAliensColToCheck = j;
			aliensColToCheck = j + 1;
		}
		else //Direction == 1
		{
			eliteAliensColToCheck = j + 2;
			aliensColToCheck = j + 3;
		}

		for (int k = 0; k < a_rowsEliteAliens; k++)
		{
			if (a_eliteAliens[k][eliteAliensColToCheck].GetIsKamikaze() == false && a_eliteAliens[k][eliteAliensColToCheck].GetIsDead() == false)
			{
				return;
			}
		}
		for (int k = 0; k < a_rowsAliens; k++)
		{
			if (a_aliens[k][aliensColToCheck].GetIsKamikaze() == false && a_aliens[k][aliensColToCheck].GetIsDead() == false)
			{
				return;
			}
		}

		InitialiseStartKamikazeData(a_superAliens, i, j, direction);
		g_iSuperAmount--;
	}
}

void GetRandomPhoenixAlienAndStartKamikazeIfPossible(Alien **a_superAliens, Alien **a_phoenixAliens, int a_colsSuperAliens, int a_colsPhoenixAliens)
{
	int i = 0;
	int j = rand() % a_colsPhoenixAliens;
	if (a_phoenixAliens[i][j].GetIsKamikaze() == false && a_phoenixAliens[i][j].GetIsDead() == false)
	{
		int direction = RandomKamikazeDirection();
		float phoenixAlienX = 0.f, phoenixAlienY = 0.f;
		a_phoenixAliens[i][j].GetPosition(phoenixAlienX, phoenixAlienY);

		int superAliensAvailability[6];

		for (int k = 0; k < a_colsSuperAliens; k++)
		{
			if (a_superAliens[0][k].GetIsDead() == true || a_superAliens[0][k].GetIsKamikaze() == true)
			{
				superAliensAvailability[k] = -1;
				continue; //Start next "for" iteration
			}
			float superAlienX = 0.f, superAlienY = 0.f;
			a_superAliens[0][k].GetPosition(superAlienX, superAlienY);


			if (superAlienX == phoenixAlienX
				|| phoenixAlienX == superAlienX - a_superAliens[0][k].GetWidth() - g_fAliensGap
				|| phoenixAlienX == superAlienX + a_superAliens[0][k].GetWidth() + g_fAliensGap)
			{
				superAliensAvailability[k] = k;
			}
			else
			{
				superAliensAvailability[k] = -1;
			}
		}

		int counter = 0;
		for (int k = 0; k < a_colsSuperAliens; k++)
		{
			if (superAliensAvailability[k] != -1)
			{
				counter++;
			}
		}

		InitialiseStartKamikazeData(a_phoenixAliens, i, j, direction);
		g_iPhoenixAmount--;
		if (counter == 3)
		{
			if (direction == -1)
			{
				for (int k = 0; k < a_colsSuperAliens; k++)
				{
					if (counter == 1)
					{
						return;
					}
					if (superAliensAvailability[k] != -1)
					{
						InitialiseStartKamikazeData(a_superAliens, 0, k, direction);
						g_iSuperAmount--;
						counter--;
					}
				}
			}
			else
			{
				for (int k = a_colsSuperAliens; k > 0; k--)
				{
					if (counter == 1)
					{
						return;
					}
					if (superAliensAvailability[k] != -1)
					{
						InitialiseStartKamikazeData(a_superAliens, 0, k, direction);
						g_iSuperAmount--;
						counter--;
					}
				}
			}
		}
		else //Counter < 3
		{
			for (int k = 0; k < a_colsSuperAliens; k++)
			{
				if (superAliensAvailability[k] != -1)
				{
					InitialiseStartKamikazeData(a_superAliens, 0, k, direction);
					g_iSuperAmount--;
				}
			}
		}
	}
}

bool CheckIfPhoenixesAvailable(Alien **a_phoenixAliens, int a_colsPhoenixAliens)
{
	for (int k = 0; k < a_colsPhoenixAliens; k++)
	{
		if (a_phoenixAliens[0][k].GetIsDead() == false && a_phoenixAliens[0][k].GetIsKamikaze() == false)
		{
			return true;
		}
	}
	return false;
}

void DestroyAliensSprites(Alien **a_aliens, int a_rowsAliens, int a_colsAliens)
{
	Bullet *bullets;
	for (int i = 0; i < a_rowsAliens; i++)
	{
		for (int j = 0; j < a_colsAliens; j++)
		{
			UG::StopDrawingSprite(a_aliens[i][j].GetID());
			UG::DestroySprite(a_aliens[i][j].GetID());
			bullets = a_aliens[i][j].GetBullets();
			for (int k = 0; k < 3; k++)
			{
				int bulletID = bullets[k].GetID();
				if (bulletID != -1)
				{
					UG::StopDrawingSprite(bulletID);
					UG::DestroySprite(bulletID);
				}
				bullets[k].SetPosition(-1, -1);
			}
		}
	}
}

void ResetAliens()
{
	g_Aliens = new Alien*[g_iRowsAliens];
	for (size_t i = 0; i < g_iRowsAliens; i++)
		g_Aliens[i] = new Alien[g_iColsAliens];

	g_EliteAliens = new Alien*[g_iRowsEliteAliens];
	for (size_t i = 0; i < g_iRowsEliteAliens; i++)
		g_EliteAliens[i] = new Alien[g_iColsEliteAliens];

	g_SuperAliens = new Alien*[g_iRowsSuperAliens];
	for (size_t i = 0; i < g_iRowsSuperAliens; i++)
		g_SuperAliens[i] = new Alien[g_iColsSuperAliens];

	g_PhoenixAliens = new Alien*[g_iRowsPhoenix];
	for (size_t i = 0; i < g_iRowsPhoenix; i++)
		g_PhoenixAliens[i] = new Alien[g_iColsPhoenix];
}

int RandomKamikazeDirection()
{
	int direction = rand() % 1;

	if (direction == 1)
	{
		return 1;
	}
	else
	{
		return -1;
	}

}

int CalculateKamikazeDirection(Alien **a_aliens, int a_rowsAliens, int a_colsAliens, int a_i, int a_j)
{
	int direction = 0;
	if (a_j == 0)
		direction = -1;
	else if (a_j == a_colsAliens - 1)
		direction = 1;
	else
	{
		for (int k = 0; k < a_j; k++)
		{
			if (a_aliens[a_i][k].GetIsKamikaze() == true || a_aliens[a_i][k].GetIsDead() == true)
			{
				direction = -1; //Free space on the left
			}
			else
			{
				direction = 0;
				break;
			}
		}
		if (direction == 0)
		{
			direction = -1;
			for (int k = a_colsAliens - 1; k > a_j; k--)
			{
				if (a_aliens[a_i][k].GetIsKamikaze() == true || a_aliens[a_i][k].GetIsDead() == true)
				{
					direction = 1; //Free space on the right
				}
				else
				{
					direction = 0;
					return direction;
				}
			}
		}
	}
	return direction;
}

void InitialiseStartKamikazeData(Alien **a_aliens, int a_i, int a_j, int a_direction)
{
	a_aliens[a_i][a_j].SetKamikazeDirection(a_direction);
	a_aliens[a_i][a_j].SetIsKamikaze(true);
	float x = 0.f, y = 0.f;
	a_aliens[a_i][a_j].GetPosition(x, y);
	a_aliens[a_i][a_j].SetStartKamikazePosition(x, y);
}

void MoveAliens(Alien **a_aliens, Spaceship& a_spaceship, int a_rows, int a_cols, int a_index, float a_animationFrames[7][4], int &aliensAbleToKamikaze)
{
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			float spaceshipX = 0.f, spaceshipY = 0.f;
			a_spaceship.GetPosition(spaceshipX, spaceshipY);
			Position playerPos = Position(spaceshipX, spaceshipY);
			float* animFrames = a_animationFrames[(a_index + j % 3) % 3];
			a_aliens[i][j].Move(playerPos, animFrames, aliensAbleToKamikaze);
		}
	}
}

int GetAliensMoveDirection(Alien **a_aliens, int a_rows, int a_cols)
{
	float pX = 0.f, pY = 0.f;
	for (int j = 0; j < a_cols; j++)
	{
		for (int i = a_rows - 1; i >= 0; i--) //Change move direction to the right
		{
			if (a_aliens[i][j].GetID() != -1 && a_aliens[i][j].GetIsDead() == false && a_aliens[i][j].GetIsKamikaze() == false
				&& a_aliens[i][j].GetShouldGoBackToFormation() == false && a_aliens[i][j].GetIsGoingBackToFormation() == false)
			{
				a_aliens[i][j].GetPosition(pX, pY);
				if (pX <= (a_aliens[i][j].GetWidth() * 0.5f))
					return 1;
			}
		}
	}
	for (int j = a_cols - 1; j >= 0; j--)
	{
		for (int i = a_rows - 1; i >= 0; i--) //Change move direction to the left
		{
			if (a_aliens[i][j].GetID() != -1 && a_aliens[i][j].GetIsDead() == false && a_aliens[i][j].GetIsKamikaze() == false
				&& a_aliens[i][j].GetShouldGoBackToFormation() == false && a_aliens[i][j].GetIsGoingBackToFormation() == false)
			{
				a_aliens[i][j].GetPosition(pX, pY);
				if (pX >= (g_iScreenWidth - a_aliens[i][j].GetWidth() * 0.5f))
					return -1;
			}
		}
	}
	return 0; //Move direction the same as before
}

void SetAliensDirection(Alien **a_aliens, int a_rows, int a_cols, int a_direction)
{
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			a_aliens[i][j].SetDirection(a_direction);
		}
	}
}

void AnimateAliensExplosion(Alien **a_aliens, int a_rows, int a_cols, float a_animationFrames[7][4])
{
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			if (a_aliens[i][j].GetExplosionCounter() == 1)
			{
				a_aliens[i][j].SetExplosionCounter(a_aliens[i][j].GetExplosionCounter() + 1);
				UG::SetSpriteUVCoordinates(a_aliens[i][j].GetID(), a_animationFrames[3]);
				continue;
			}
			else if (a_aliens[i][j].GetExplosionCounter() == 2)
			{
				a_aliens[i][j].SetExplosionCounter(a_aliens[i][j].GetExplosionCounter() + 1);
				UG::SetSpriteUVCoordinates(a_aliens[i][j].GetID(), a_animationFrames[4]);
				continue;
			}
			else if (a_aliens[i][j].GetExplosionCounter() == 3)
			{
				a_aliens[i][j].SetExplosionCounter(a_aliens[i][j].GetExplosionCounter() + 1);
				UG::SetSpriteUVCoordinates(a_aliens[i][j].GetID(), a_animationFrames[5]);
				continue;
			}
			else if (a_aliens[i][j].GetExplosionCounter() == 4 || a_aliens[i][j].GetExplosionCounter() == 5)
			{
				a_aliens[i][j].SetExplosionCounter(a_aliens[i][j].GetExplosionCounter() + 1);
				UG::SetSpriteUVCoordinates(a_aliens[i][j].GetID(), a_animationFrames[6]);
				continue;
			}
			else if (a_aliens[i][j].GetExplosionCounter() == 6)
			{
				UG::StopDrawingSprite(a_aliens[i][j].GetID());
				UG::DestroySprite(a_aliens[i][j].GetID());
				a_aliens[i][j].SetExplosionCounter(0);
				continue;
			}
		}
	}
}

void Hitmark(Alien **a_aliens, int a_rows, int a_cols, Bullet& a_bullet, Spaceship& a_spaceship, int &ableToKamikazeAmount, float a_animationFrames[7][4])
{
	float bulletX, bulletY;
	float alienX = 0.f, alienY = 0.f;
	float spaceshipX = 0.f, spaceshipY = 0.f;
	a_spaceship.GetPosition(spaceshipX, spaceshipY);
	a_bullet.GetPosition(bulletX, bulletY);
	for (int i = 0; i < a_rows; i++)
	{
		for (int j = 0; j < a_cols; j++)
		{
			a_aliens[i][j].GetPosition(alienX, alienY);
			//If alien is exploding then don't check hitmark
			if (a_aliens[i][j].GetExplosionCounter() > 0)
			{
				continue;
			}
			//Check if spaceship's bullet hit alien
			else if (a_aliens[i][j].GetIsDead() == false
				&& a_bullet.GetIsFired() == true
				&& (bulletX - a_bullet.GetWidth() * 0.5f <= alienX + a_aliens[i][j].GetWidth() * 0.5f)
				&& (bulletY + a_bullet.GetHeight() * 0.5f >= alienY - a_aliens[i][j].GetHeight() * 0.5f)
				&& (bulletX + a_bullet.GetWidth() * 0.5f >= alienX - a_aliens[i][j].GetWidth() * 0.5f)
				&& (bulletY - a_bullet.GetHeight() * 0.5f <= alienY + a_aliens[i][j].GetHeight() * 0.5f))
			{
				a_aliens[i][j].SetIsDead(true);
				ableToKamikazeAmount--;
				UG::SetSpriteUVCoordinates(a_aliens[i][j].GetID(), a_animationFrames[3]);
				a_aliens[i][j].SetExplosionCounter(a_aliens[i][j].GetExplosionCounter() + 1);
				a_bullet.SetIsFired(false);
				if (a_aliens[i][j].GetIsKamikaze() == false)
				{
					a_spaceship.SetPoints(a_spaceship.GetPoints() + a_aliens[i][j].GetPoints());
				}
				else
				{
					a_spaceship.SetPoints(a_spaceship.GetPoints() + a_aliens[i][j].GetKamikazePoints());
				}

				return;
			}

			//Check if alien hits the spaceship
			if (a_aliens[i][j].GetIsDead() == false
				&& a_aliens[i][j].GetIsKamikaze() == true
				&& (spaceshipX - a_spaceship.GetWidth() * 0.5f <= alienX + a_aliens[i][j].GetWidth() * 0.5f)
				&& (spaceshipY + a_spaceship.GetHeight() * 0.5f >= alienY - a_aliens[i][j].GetHeight() * 0.5f)
				&& (spaceshipX + a_spaceship.GetWidth() * 0.5f >= alienX - a_aliens[i][j].GetWidth() * 0.5f)
				&& (spaceshipY - a_spaceship.GetHeight() * 0.5f <= alienY + a_aliens[i][j].GetHeight() * 0.5f))
			{
				a_aliens[i][j].SetIsDead(true);
				ableToKamikazeAmount--;
				UG::StopDrawingSprite(a_aliens[i][j].GetID());
				UG::DestroySprite(a_aliens[i][j].GetID());
				a_bullet.SetIsFired(false);
				int livesLeft = a_spaceship.GetLives() - 1;
				a_spaceship.SetLives(livesLeft);

				if (livesLeft < 0)
				{
					g_currentState = GAMEOVER;
					return;
				}
				else
				{
					UG::DestroySprite(l_spaceshipLives.GetID());
					l_spaceship.SetPosition(g_iScreenWidth * 0.5f, g_iScreenHeight * 0.1f);
				}
			}

			Bullet *bullets = a_aliens[i][j].GetBullets();
			float alienBulletX = 0.f, alienBulletY = 0.f;
			//Check if alien's bullets hit the spaceship
			for (int k = 0; k < 3; k++)
			{
				if (bullets[k].GetID() == -1)
				{
					continue;
				}
				bullets[k].GetPosition(alienBulletX, alienBulletY);
				if ((alienBulletX - a_bullet.GetWidth() * 0.5f <= spaceshipX + a_spaceship.GetWidth() * 0.5f)
					&& (alienBulletY + a_bullet.GetHeight() * 0.5f >= spaceshipY - a_spaceship.GetHeight() * 0.5f)
					&& (alienBulletX + a_bullet.GetWidth() * 0.5f >= spaceshipX - a_spaceship.GetWidth() * 0.5f)
					&& (alienBulletY - a_bullet.GetHeight() * 0.5f <= spaceshipY + a_spaceship.GetHeight() * 0.5f))
				{
					int livesLeft = a_spaceship.GetLives() - 1;
					a_spaceship.SetLives(livesLeft);

					UG::StopDrawingSprite(bullets[k].GetID());
					UG::DestroySprite(bullets[k].GetID());
					bullets[k].SetID(-1);

					if (livesLeft < 0)
					{
						g_currentState = GAMEOVER;
						return;
					}
					else
					{
						UG::DestroySprite(l_spaceshipLives.GetID());
						l_spaceship.SetPosition(g_iScreenWidth * 0.5f, g_iScreenHeight * 0.1f);
					}
				}
			}
		}
	}
}

void DisplayMenu(Star* a_stars, int a_starsCounter)
{
	UG::SetFont("./fonts/invaders.fnt");
	UG::DrawString("PLAY", (int)(g_iScreenWidth * 0.47), (int)g_fPlayOptionY);
	UG::DrawString("HIGH SCORES", (int)(g_iScreenWidth * 0.4185), (int)g_fHighscoresOptionY);
	UG::DrawString("QUIT", (int)(g_iScreenWidth * 0.47), (int)g_fQuitOptionY);
	MoveStars(a_stars, a_starsCounter);
}

void DisplayGameplayHeader(Spaceship& a_spaceship)
{
	std::ostringstream ssScore;
	std::ostringstream ssHighScore;
	std::ostringstream ss1UP;
	std::ostringstream ss1UPScore;

	ss1UP << "1UP";
	ss1UPScore << a_spaceship.GetPoints();
	ssHighScore << "HIGH    SCORE";
	ssScore << a_spaceship.GetPoints();

	UG::SetFont("./fonts/invaders.fnt");

	UG::DrawString(ss1UP.str().c_str(), (int)(g_iScreenWidth * 0.25f), g_iScreenHeight - 2, 1.5f);
	UG::DrawString(ss1UPScore.str().c_str(), (int)(g_iScreenWidth * 0.291f), g_iScreenHeight - 37, 1.5f, UG::SColour(255, 0, 0, 255));
	UG::DrawString(ssHighScore.str().c_str(), (int)(g_iScreenWidth * 0.4f), g_iScreenHeight - 2, 1.5f);
	UG::DrawString(ssScore.str().c_str(), (int)(g_iScreenWidth * 0.54f), g_iScreenHeight - 37, 1.5f, UG::SColour(255, 0, 0, 255));
}

void DisplayStarsOnGameover(Star* a_stars)
{
	float xPos, yPos;
	for (int i = 0; i < g_iStarsAmount; i++)
	{
		a_stars[i].GetPosition(xPos, yPos);

		if (a_stars[i].GetIsShining() == false)
		{
			UG::DrawLine((int)xPos - 2, (int)yPos - 2, (int)xPos + 2, (int)yPos + 2, a_stars[i].GetColour());
			UG::DrawLine((int)xPos - 2, (int)yPos + 2, (int)xPos + 2, (int)yPos - 2, a_stars[i].GetColour());
			UG::DrawLine((int)xPos + 2, (int)yPos - 2, (int)xPos - 2, (int)yPos + 2, a_stars[i].GetColour());
			UG::DrawLine((int)xPos + 2, (int)yPos + 2, (int)xPos - 2, (int)yPos - 2, a_stars[i].GetColour());
		}
	}

}

void InitialiseScreenProperties()
{
	UG::GetScreenSize(g_iScreenWidth, g_iScreenHeight);

	g_fPlayOptionY = g_iScreenHeight * 0.5f;
	g_fHighscoresOptionY = g_iScreenHeight * 0.42f;
	g_fQuitOptionY = g_iScreenHeight * 0.34f;
	g_fPlayOptionCenterY = g_fPlayOptionY - 11;
	g_fHighscoresOptionCenterY = g_fHighscoresOptionY - 11;
	g_fQuitOptionCenterY = g_fQuitOptionY - 11;

	UG::SetBackgroundColor(UG::SColour(0x00, 0x00, 0x00, 0xFF));
	UG::AddFont("./fonts/invaders.fnt");
}

void InitialiseStarsPositions(Star *a_stars)
{
	for (int i = 0; i < g_iStarsAmount; i++)
	{
		a_stars[i].SetPosition((float)(rand() % g_iScreenWidth), (float)(rand() % g_iScreenHeight));
	}
}

void MoveStars(Star *a_stars, int a_counter)
{
	for (int i = 0; i < g_iStarsAmount; i++)
	{
		stars[i].Move(a_counter);
	}
}

void InitialiseSpaceshipLives(Spaceship& a_spaceshipLives, const char* a_spaceshipLivesImageFilename, float a_xPos, float a_yPos)
{
	a_spaceshipLives.SetPosition(a_xPos, a_yPos);
	a_spaceshipLives.SetID(UG::CreateSprite(a_spaceshipLivesImageFilename, (float)(a_spaceshipLives.GetLivesWidth()), (float)(a_spaceshipLives.GetLivesHeight()), true));
	UG::MoveSprite(a_spaceshipLives.GetID(), a_xPos, a_yPos);
}

void GetUserInput()
{
	UG::DrawString("ENTER YOUR NAME:", (int)(g_iScreenWidth * 0.37), (int)(g_iScreenHeight * 0.5), 1.0f, UG::SColour(255, 0, 0, 255));
	if (UG::IsKeyDown(UG::KEY_BACKSPACE))
	{
		nick = nick.substr(0, nick.size() - 1);
		Sleep(100);
	}

	if (nick.length() < 8)
	{
		for (char letter = UG::KEY_A; letter <= UG::KEY_Z; ++letter)
		{
			if (UG::IsKeyDown(letter))
			{
				nick += letter;
			}
		}
	}
	Sleep(100);

	UG::DrawString(nick.c_str(), (int)(g_iScreenWidth * 0.455), (int)(g_iScreenHeight * 0.45), 1.0f);
}

#pragma endregion