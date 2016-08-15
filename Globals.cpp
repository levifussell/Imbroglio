#include "Globals.h"

#include <stdio.h>
#include <conio.h>
#include <Windows.h>

//NOTE: remove this function later on if still not useful
int G::Modulo(int nb, int mod)
{
	if(nb < 0)
		nb += mod;

	//Allows player to wrap from one screen side to the other

	return nb % mod;
}
//Places text based on a coordinate position, a text character and a colour (w)
void G::PlaceText(char text, int posX, int posY, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Set new position
	COORD cursorPlacePos;
	cursorPlacePos.X = posX + 1;
	cursorPlacePos.Y = posY + 1;
	//Place cursor position
	SetConsoleCursorPosition(hConsole, cursorPlacePos);
	//Draw character
	DrawChar(text, w);
}
void G::PlaceText(char* text, int posX, int posY, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Set new position
	COORD cursorPlacePos;
	//+1 = offset whole screen by one to add border
	cursorPlacePos.X = posX + 1;
	cursorPlacePos.Y = posY + 1;
	//Place cursor position
	SetConsoleCursorPosition(hConsole, cursorPlacePos);
	//Draw character
	DrawChar(text, w);
}
//Draws a character at the cursor position based on a text character and a colour (w)
void G::DrawChar(char charToDraw, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Set Color
	//   NOTE: Background colour of world is constant unless otherwise
	SetConsoleTextAttribute(hConsole, w|COLOR_WORLD_BACKGROUND);
	//Print character with colour
	cprintf("%c", charToDraw);
}
void G::DrawChar(char* charToDraw, int w)
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	//Set Color
	//   NOTE: Background colour of world is constant unless otherwise
	SetConsoleTextAttribute(hConsole, w|COLOR_WORLD_BACKGROUND);
	//Print character with colour
	cprintf("%s", charToDraw);
}
//Returns a random value greater than or equal to base and less than max
int G::Random(int base, int max)
{
	//Get random integer (something like 0 to 80000000)
	int randomValue = rand();

	//If max value is zero, return zero because you cannot divide by zero
	if(max == 0)
		return 0;
	//Else return modulo of randomnumber plus the base
	else
		return randomValue % max + base;
}
//Rounds a float to the NEAREST integer
int G::Round(float number)
{
	//Get integer value of float (floor)
	int numberRoundDown = (int)number;
	//Get the remaining decimal
	float remainder = number - (float)numberRoundDown;

	//If decimals are negative, make positive
	//  NOTE: abs() does not work because it takes an integer
	if(remainder < 0)
		remainder *= -1;

	//If remainder is less than 0.5f, round down
	if(remainder < 0.5f)
		return numberRoundDown;
	//If remainder is greater than 0.5f, round up
	else 
		return numberRoundDown + 1;
}
//Check whether to positions are equal
bool G::Collision(int obj1PosX, int obj1PosY, int obj2PosX, int obj2PosY)
{
	return obj1PosX == obj2PosX && obj1PosY == obj2PosY;
}
//Similar to modulo, but used for floats
//  --Wraps a position around screen if greater than or less than specified values
float G::Wrap(int minX, int maxX, float value)
{
	if(value < minX)
		value = (float)maxX - 1;
	else if(value > maxX - 1)
		value = (float)minX;

	return value;
} 
//UI Functions:
void G::UpdateGameUI()
{
	try
	{
		//Place score
		if(previousScore != gameScore
			|| levelPrevious != levelCurrent)
		{
			PlaceText("SCORE: ", 60, 2, WHITE);

			char value[sizeof(int)*8+1];
			char* scoreValue = itoa(gameScore, value, 10);
			PlaceText(scoreValue, 67, 2, PURPLE);

			PlaceText(" of ", 68, 2, WHITE);

			char* scoreGoal = itoa(levelData.scoreToWin, value, 10);
			PlaceText(scoreGoal, 72, 2, WHITE);
		}
		previousScore = gameScore;

		//Write Level Number
		if(levelPrevious != levelCurrent)
		{
			PlaceText("LEVEL: ", 60, 4, WHITE);

			char value[sizeof(int)*8+1];
			char* levelNumber = itoa(levelCurrent, value, 10);
			PlaceText(levelNumber, 67, 4, levelData.wordColor);
		}

		//Write Mouse Position
		PlaceText("MOUSE POS: X=     Y=", 60, 12, WHITE);
		char value[sizeof(int)*8+1];
		char* posValueX = itoa(GetMousePos().x, value, 10);
		PlaceText(posValueX, 73, 12, WHITE);
		char* posValueY = itoa(GetMousePos().y, value, 10);
		PlaceText(posValueY, 81, 12, WHITE);
	}
	catch(char* error)
	{
		printf(error);
	}

}
void G::AddToScore(int additionValue)
{
	gameScore += additionValue;
}
POINT G::GetMousePos()
{
	POINT posOrigin;
	WINDOWPLACEMENT window = GetWindowData();
	posOrigin.x = window.rcNormalPosition.left;
	posOrigin.y = window.rcNormalPosition.top;

	POINT pos;

	GetCursorPos(&pos);
	pos.x -= posOrigin.x;
	pos.y -= posOrigin.y;
	pos.x /= 8;
	pos.y /= 13;
	return pos;
}
WINDOWPLACEMENT G::GetWindowData()
{
	WINDOWPLACEMENT p;
	HWND foreGroundWindow = GetForegroundWindow();
	GetWindowPlacement(foreGroundWindow, &p);

	return p;
}
void G::SetWindowData(RECT windowRectangle)
{
	WINDOWPLACEMENT p = GetWindowData();
	p.rcNormalPosition = windowRectangle;

	HWND foreGroundWindow = GetForegroundWindow();
	SetWindowPlacement(foreGroundWindow, &p);
}

//Game Variables definition
int G::gameScore;
int G::previousScore;
int G::levelCurrent;
int G::levelPrevious;
int G::levelPlacedScores;
LevelData G::levelData;
int G::highScore;

bool G::latestExplosionComplete;
