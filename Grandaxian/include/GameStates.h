#ifndef _GAMESTATES_H_
#define _GAMESTATES_H_

typedef enum GameStates
{
	SPLASH,                   //Splash Screen - first thing we are going to see after starting the game
	MENU,                     //Game Main Menu
	START,                    //Screen appearing after choosing Play Option
	GAMEPLAY,                 //Gameplay itself
	HIGHSCORES,               //Highscores screen
	PROVIDINGNICK,            //Screen when Player provides game with their Nick to be placed in the Hall of Fame (or Shame)
	GAMEOVER,                 //Gameover Screen - Player can see what killed him
	QUIT                      //Closes the program
}GameState;

#endif
