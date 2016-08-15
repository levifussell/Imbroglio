#include "Explosion.h"

#include <math.h>


Explosion::Explosion(int PosX, int PosY, int ColorWord, 
					 char Text, int ParticleCount)
{
	this->posX = PosX;
	this->posY = PosY;
	this->colorWord = ColorWord;
	this->text = Text;

	this->particleList = new List<Particle*>();

	this->Initialize(ParticleCount);
}
Explosion::~Explosion(void)
{
	delete this->particleList;
}
void Explosion::Initialize(int ParticleCount)
{
	for(int i = 0; i < ParticleCount; ++i)
	{
		//float pSpeedX = ((float)g.Random(1, 200) - (float)100) / (float)2000;
		//float pSpeedY = ((float)g.Random(1, 200) - (float)100) / (float)2000;
		int randDegree = g.Random(1, 360);
		float randRadian = (float)randDegree / (float)180 * 3.14159f;
		float pSpeedX = cos(randRadian)/ (float)g.Random(2, 4);
		float pSpeedY = sin(randRadian)/ (float)g.Random(2, 4);

		Particle* p = new Particle(this->posX, this->posY,
							pSpeedX, pSpeedY, this->colorWord, this->text);
		this->particleList->AddBack(p);
	}
}
void Explosion::Update(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	for(int i = 0; i < this->particleList->length; ++i)
	{
		this->particleList->Get(i)->Update(gameObjects);

		if(this->particleList->Get(i)->isDead())
		{
			g.PlaceText(g.FLOOR_TEXT, this->particleList->Get(i)->posX, 
										this->particleList->Get(i)->posY, g.COLOR_WORLD_NORMAL);
			this->particleList->RemoveAt(i);
		}
	}
}
bool Explosion::isDead()
{
	return this->particleList->length == 0;
}

