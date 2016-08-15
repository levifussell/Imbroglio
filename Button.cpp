#include <Windows.h>

#include "Button.h"


Button::Button(int posX, int posY, int width, int height, 
			   char* Text, int TextSize)
{
	this->state = EMPTY;
	this->colorWord = WHITE;
	this->rectangle.left = posX;
	this->rectangle.top = posY;
	this->rectangle.right = posX + width;
	this->rectangle.bottom = posY + height;
	this->text = Text;
	this->textSize = TextSize;
	this->Draw();
}
Button::~Button(void)
{
}

void Button::Update()
{
	POINT mousePos = g.GetMousePos(); 

	if(Contains(this->rectangle, mousePos))
	{
		if(GetAsyncKeyState(VK_LBUTTON) != 0 && this->state == HOVERED)
			ChangeState(PRESSED);
		else if(GetAsyncKeyState(VK_LBUTTON) == 0 && this->state != HOVERED)
			ChangeState(HOVERED);
	}
	else if(!Contains(this->rectangle, mousePos) && this->state != EMPTY)
		ChangeState(EMPTY);
}
void Button::ChangeState(int stateNew)
{
	this->state = stateNew;

	switch(this->state)
	{
		case EMPTY:
			this->colorWord = WHITE;
			break;
		case HOVERED:
			this->colorWord = RED;
			break;
		case PRESSED:
			this->colorWord = YELLOW;
			break;
	}

	this->Draw();
}
bool Button::Contains(RECT rec, POINT pos)
{
	if(pos.x >= rec.left && pos.x <= rec.right
		&& pos.y >= rec.top && pos.y <= rec.bottom)
		return true;

	return false;
}
void Button::Draw()
{
	int width = this->rectangle.right- this->rectangle.left;
	int height = this->rectangle.bottom- this->rectangle.top;

	for(int y = 0; y < height; ++y)
	{
		for(int x = 0; x < width; ++x)
		{
			if(x == 0 || x == width - 1
				|| y == 0 || y == height - 1)
			{
				g.PlaceText((char)5, this->rectangle.left + x, 
							this->rectangle.top + y, this->colorWord);
			}
		}
	}

	g.PlaceText(this->text, this->rectangle.left + ((width - this->textSize) / 2), 
							this->rectangle.top + (height / 2), WHITE);
}
