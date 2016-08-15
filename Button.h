#include <Windows.h>

#include "Globals.h"

#pragma once
enum ButttonState
{
	EMPTY, HOVERED, PRESSED, RELEASED
};

class Button
{
public:
	Button(int posX, int posY, int width, int height, 
			char* Text, int TextSize);
	~Button(void);

	void Update();
	bool Contains(RECT rec, POINT pos);
	void ChangeState(int stateNew);
	void Draw();

	//Variables
	RECT rectangle;
	static const G g;
	int colorWord;
	char* text;
	int textSize;

	int state;
};

