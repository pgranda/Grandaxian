#ifndef _HIGHSCORESHELPER_H_
#define _HIGHSCORESHELPER_H_

#include <string>


class HighScoresHelper
{
public:
	static void SaveHighscores(std::string a_nick, int a_points);
	static void DisplayHighscores();

private:
	static inline int Check(char a_character);
	static void Encrypt(std::string &a_tab);
	static void Decrypt(std::string &a_tab);
	static void Enigma(int a_key, std::string &a_tab);
};

#endif 

