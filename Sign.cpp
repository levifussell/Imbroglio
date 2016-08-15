#include "Sign.h"


Sign::Sign(int PosX, int PosY, float SpeedX, float SpeedY, int DistanceX, 
					List<char*> TextLines, bool ClearAtEnd)
{
	this->posX = PosX;
	this->posY = PosY;
	this->speedX = SpeedX;
	this->speedY = SpeedY;

	this->posXstart = PosX;
	this->distanceX = DistanceX;

 	this->textLines = new List<char*>();

	for(int i = 0; i < TextLines.length; ++i)
	{
	
		this->textLines->AddBack(TextLines.Get(i));
	}

	this->isDead = false;
	this->clearAtEnd = ClearAtEnd;
}
Sign::~Sign(void)
{
	try
	{
		delete this->textLines;
	}
	catch(char* error)
	{
	}
}
void Sign::Update()
{
	//Kill sign if it has proceeded beyond given distance
	if(abs((int)(this->posXstart - this->posX)) > this->distanceX)
	{
		if(this->clearAtEnd)
			this->Clear();

		this->isDead = true;
	}

	if(!this->isDead)
	{
		int pastPosX = g.Round(this->posX);
		int pastPosY = g.Round(this->posY);
		this->posX = this->posX + this->speedX;
		this->posY = this->posY + this->speedY;

		//Clear sign's old position and redraw at new position
		if(pastPosX != g.Round(this->posX) || pastPosY != g.Round(this->posY))
		{
			this->Clear();
			this->Draw();
		}
	}
}
void Sign::Draw()
{
	//New draw
	for(int i = 0; i < this->textLines->length; ++i)
	{
		g.PlaceText(this->textLines->Get(i), g.Round(this->posX), g.Round(this->posY + i), WHITE);
	}
}
void Sign::Clear()
{
	//Clear last draw
	for(int i = 0; i < this->textLines->length; ++i)
	{
		g.PlaceText(this->textLines->Get(i), g.Round(this->posX - this->speedX), g.Round(this->posY - this->speedY + i), BLACK);
	}
}
