#include "Globals.h"
#include "List.h"

#pragma once

//Game Object
class GameObject
{
public:
	static const G g;

	GameObject();
	GameObject(float PosX, float PosY, char Char, int colorWord);
	~GameObject(void);
	void InitializeBasicValues(float PosX, float PosY, char Text, int ColorWord,
								float SpeedX, float SpeedY, float SpeedRate);
	virtual void Update(GameObject *gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);
	void ChangeColor(int ColorWord);
	/*virtual void CollisionResultXOnly(GameObject* collisionObj,
						GameObject* collidingObj);
	virtual void CollisionResultYOnly(GameObject* collisionObj,
						GameObject* collidingObj);*/
	
	void GenerateName(void)
	{
		ID = ((float)g.Random(0, 1000) * (float)g.Random(0, g.Random(1, 1000)) / 1000) + 
				((float)g.Random(0, 45) * (float)g.Random(0, 50) / 1000);
	}

	float posX;
	float posY;
	float speedX;
	float speedY;
	float speedRate;
	char text;
	char textOriginal;

	int colorWord;
	float ID;
	bool firstDraw;
	bool drawDark;
	bool isDeadFlag;

	float lastSpeedX;
	float lastSpeedY;

	bool updated;

private:
	bool colorChange;
};

//Cell
class Cell : public GameObject
{
public:
	Cell();
	Cell(float PosX, float PosY);
	int main(void);

	bool used;
};

//Wall
class Wall : public GameObject
{
public:
	Wall();
	Wall(float PosX, float PosY);
	int main(void);
};

//Player
class Player : public GameObject
{
public:
	Player();
	Player(float PosX, float PosY, int LightDistance);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);

	float lightDistance;
	float lightDistanceMinimum;
	int action;
};


//Enemy
class Enemy : public GameObject
{
public:
	Enemy();
	Enemy(float PosX, float PosY);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);

	//Directions: 0 = NORTH, 1 = EAST, 2 = SOUTH, 3 = WEST
	/*int direction;
	bool collision;
	float leftDestX;
	float leftDestY;
	bool checkForDestination;*/

	void UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
};
//Enemy Navigator
class EnemyNavigator : public Enemy
{
public:
	EnemyNavigator();
	EnemyNavigator(float PosX, float PosY);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);
	/*virtual void CollisionResultXOnly(GameObject* collisionObj,
						GameObject* collidingObj);
	virtual void CollisionResultYOnly(GameObject* collisionObj,
						GameObject* collidingObj);*/

	//Directions: 0 = NORTH, 1 = EAST, 2 = SOUTH, 3 = WEST
	int direction;
	bool collision;
	float leftDestX;
	float leftDestY;
	bool checkForDestination;

	void UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
	void SpeedBasedOnDirection(int direction, float* speedX, float* speedY);
};
//Enemy Follower
class EnemyFollower : public Enemy
{
public:
	EnemyFollower();
	EnemyFollower(float PosX, float PosY);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);

	void UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH], Player* player);
};

//Item
class Item : public GameObject
{
public:
	Item();
	Item(float PosX, float PosY);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);
};

//Particle
class Particle : public GameObject
{
public:
	Particle();
	Particle(float PosX, float PosY, float SpeedX, float SpeedY, int ColorWord, char text);
	int main(void);
	virtual void CollisionResult(GameObject* collisionObj,
						GameObject* collidingObj);
	virtual void Update(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
	bool isDead();
};
