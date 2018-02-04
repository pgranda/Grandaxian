#include "HighScoresHelper.h"
#include "UGFW.h"
#include <iostream>
#include <fstream>

extern std::string g_highScoresFilePath;
extern int g_iScreenWidth;
extern int g_iScreenHeight;

void HighScoresHelper::SaveHighscores(std::string a_nick, int a_points)
{
	std::string currentLines[22];
	std::fstream highscores;
	std::string line;
	highscores.open(g_highScoresFilePath, std::ios_base::in | std::ios_base::binary);

	if (!highscores.is_open())
	{
		std::cout << "ERROR, ALIENS ATE DATA!" << std::endl;
	}

	int lineCounter = 0;

	while (getline(highscores, line))
	{
		Decrypt(line);
		if (lineCounter % 2 == 0) //Players nick
		{
			currentLines[lineCounter] = line;
		}
		else
		{
			int previousOtherPlayerPoints = atoi(currentLines[lineCounter - 2].c_str());
			int otherPlayerPoints = atoi(line.c_str());
			if (otherPlayerPoints < a_points && (previousOtherPlayerPoints >= a_points || lineCounter == 1))
			{
				//Move old highscore below
				currentLines[lineCounter + 2] = line;
				currentLines[lineCounter + 1] = currentLines[lineCounter - 1];
				//Save new highscore
				currentLines[lineCounter] = std::to_string(a_points);
				currentLines[lineCounter - 1] = a_nick;

				lineCounter += 2;
			}
			else
			{
				currentLines[lineCounter] = line;
			}
		}
		lineCounter++;
	}
	if (lineCounter < 19)
	{
		currentLines[lineCounter] = a_nick;
		currentLines[lineCounter + 1] = std::to_string(a_points);
	}
	highscores.sync();
	highscores.close();
	highscores.clear();


	highscores.open(g_highScoresFilePath, std::ios_base::out | std::ios_base::binary);

	if (!highscores.is_open())
	{
		std::cout << "ERROR, ALIENS ATE DATA!" << std::endl;
	}
	for (int i = 0; i < 20; i++)
	{
		Encrypt(currentLines[i]);
		highscores << currentLines[i] << std::endl;
	}
	highscores.sync();
	highscores.close();
	highscores.clear();
}

void HighScoresHelper::DisplayHighscores()
{
	UG::SetFont("./fonts/invaders.fnt");
	std::fstream highscores;
	std::string line;
	highscores.open(g_highScoresFilePath, std::ios_base::in | std::ios_base::binary);

	if (!highscores.is_open())
	{
		std::cout << "ERROR, ALIENS ATE DATA!" << std::endl;
	}
	float lineY = g_iScreenHeight * 0.55f;
	int lineCounter = 1;
	std::string lineToDisplay;
	while (getline(highscores, line))
	{
		Decrypt(line);
		if (lineCounter % 2 == 1)
		{
			lineToDisplay = std::to_string(lineCounter / 2 + 1) + ".";
			UG::DrawString(lineToDisplay.c_str(), (int)(g_iScreenWidth * 0.38), (int)lineY);
			lineToDisplay = line;
			UG::DrawString(lineToDisplay.c_str(), (int)(g_iScreenWidth * 0.44), (int)lineY);
		}
		else
		{
			lineToDisplay = line;
			UG::DrawString(lineToDisplay.c_str(), (int)(g_iScreenWidth * 0.6), (int)lineY);
			lineY -= 50;
		}
		lineCounter++;
	}

	highscores.sync();
	highscores.close();
	highscores.clear();
}

inline int HighScoresHelper::Check(char a_character)
{
	//lowerCase letter
	if (a_character >= 'a' && a_character <= 'z') return 0;
	//upperCase letter
	if (a_character >= 'A' && a_character <= 'Z') return 1;
	//Number
	return 2;
}

void HighScoresHelper::Encrypt(std::string &a_tab)
{
	Enigma(5, a_tab);
}

void HighScoresHelper::Decrypt(std::string &a_tab)
{
	Enigma(-5, a_tab);
}

void HighScoresHelper::Enigma(int a_key, std::string &a_tab)
{
	int aux;												//Auxillary variable
	char start, end;

	for (int i = 0; i < a_tab.size(); i++)
	{
		aux = Check(a_tab[i]);

		if (aux == 0)										//If lower case letters
			start = 'a', end = 'z';
		else if (aux == 1)									//If upper case letters
			start = 'A', end = 'Z';
		else
			start = '0', end = '9';							//If numbers

		if (aux < 2)										//For letters				
		{
			if (a_key >= 0)

				if ((int)a_tab[i] + a_key <= (int)end)
					a_tab[i] += (char)a_key;
				else
					a_tab[i] = a_tab[i] + (char)a_key - 26;
			else
				if ((int)a_tab[i] + a_key >= (int)start)
					a_tab[i] += (char)a_key;
				else
					a_tab[i] = a_tab[i] + (char)a_key + 26;
		}
		else												//For numbers
		{
			if (a_key >= 0)

				if (a_tab[i] + (int)a_key <= end)
					a_tab[i] += (char)a_key;
				else
					a_tab[i] = a_tab[i] + (char)a_key - 10;
			else
				if (a_tab[i] + (int)a_key >= start)
					a_tab[i] += (char)a_key;
				else
					a_tab[i] = a_tab[i] + (char)a_key + 10;
		}
	}
}
