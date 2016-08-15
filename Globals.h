#include <Windows.h>

#include "List.h"

#pragma once

//Enums
enum SystemColors
{ 
	BLACK = 0, DARK_BLUE = 1, DARK_GREEN = 2, TEAL = 3, DARK_RED = 4, DARK_PURPLE = 5,  
	GOLD = 6, GREY = 7, DARK_WHITE = 8, BLUE = 9, GREEN = 10, CYAN = 11,  
	RED = 12, PURPLE = 13, YELLOW = 14, WHITE = 15  
}; 
//Structs
struct LevelData
{
	int wordColor;
	int scoreToWin;
};

class G
{
public:

	//Functions
	static int Modulo(int nb, int mod);
	static void PlaceText(char Text, int PosX, int PosY, int w);
	static void PlaceText(char*, int PosX, int PosY, int w);
	static void DrawChar(char charToDraw, int w);
	static void DrawChar(char*, int w);
	static int Random(int base, int max);
	static int Round(float number);
	static bool Collision(int obj1PosX, int obj1PosY, int obj2PosX, int obj2PosY);
	static float Wrap(int minX, int maxX, float value);
	//UI Functions:
	static void UpdateGameUI();
	static void AddToScore(int);
	static POINT GetMousePos();
	static WINDOWPLACEMENT GetWindowData();
	static void SetWindowData(RECT);
	//Variables
	static const int GRID_WIDTH = 50;
	static const int GRID_HEIGHT = 50;
	static const char FLOOR_TEXT = ' ';
	static const int COLOR_WORLD_NORMAL = 0;//FOREGROUND_GREEN;
	static const int COLOR_WORLD_BACKGROUND = 0;//BACKGROUND_RED|BACKGROUND_GREEN|BACKGROUND_BLUE|BACKGROUND_INTENSITY;
	//Game Variables
	static int gameScore;
	static int previousScore;
	static int levelCurrent;
	static int levelPrevious;
	static int levelPlacedScores;
	static LevelData levelData;
	static int highScore;

	static bool latestExplosionComplete;
private:
};

