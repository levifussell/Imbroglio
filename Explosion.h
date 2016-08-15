#include "List.h"
#include "Globals.h"
#include "GameObject.h"

#pragma once
class Explosion
{
public:
	//Globals
	static const G g;

	Explosion(int, int, int, char, int);
	~Explosion(void);
	void Initialize(int);
	void Update(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
	bool isDead();

	int posX;
	int posY;
	List<Particle*>* particleList;
	int colorWord;
	char text;
};

