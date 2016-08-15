#include "GameObject.h"

#include <math.h>
#include <Windows.h>
#include <typeinfo>

//Game Object
GameObject::GameObject()
{
}
GameObject::GameObject(float PosX, float PosY, char Text, int ColorWord)
{
	InitializeBasicValues(PosX, PosY, Text, ColorWord, 0, 0, 0);
}
GameObject::~GameObject(void)
{
}
//Set values of GameObject to basics
void GameObject::InitializeBasicValues(float PosX, float PosY, char Text, int ColorWord,
									   float SpeedX, float SpeedY, float SpeedRate)
{
	firstDraw = true;
	isDeadFlag = false;
	posX = PosX;
	posY = PosY;
	speedX = SpeedX;
	speedY = SpeedY;
	speedRate = SpeedRate;
	text = Text;
	textOriginal = text;
	colorWord = ColorWord;
	colorChange = false;
	GenerateName();
}
//Main update call that adds speed to position, moving object
void GameObject::Update(GameObject *gameObjects[g.GRID_WIDTH][g.GRID_HEIGHT])
{
	updated = true;
	//Check collision with gameobjects
	/*for(int i = 0; i < gameObjects->length; ++i)
	{
		if(g.Collision(posX + speedX, posY + speedY, 
			gameObjects->Get(i)->posX, gameObjects->Get(i)->posY))
		{
			CollisionResult();
		}
	}*/

	//Record last speed if not zero
	//   NOT: for future use, not currently used
	if(speedX != 0 || speedY != 0)
	{
		lastSpeedX = speedX;
		lastSpeedY = speedY;
	}

	//Get future position, divide by speed rate to make object slower/faster
	if(speedRate == 0)
		speedRate = 1;
	else
		speedRate = speedRate;

	float futureX = posX + speedX;
	float futureY = posY + speedY;

	//Wrap Position
	futureX = g.Wrap(0, g.GRID_WIDTH, futureX);
	futureY = g.Wrap(0, g.GRID_HEIGHT, futureY);

	//overrides the draw so that objects can be drawn in darkness
	// or in light (adds effect of torch)
	bool overrideDraw = false;
	//textOriginal = base ASCII character of object
	// text = current ASCII character of object
	if(!drawDark && text != textOriginal)
	{
		text = textOriginal;
		overrideDraw = true;
	}
	else if(drawDark && text != g.FLOOR_TEXT)
	{
		text = g.FLOOR_TEXT;
		overrideDraw = true;
	}

	//Check if the future position is NOT the same as the current position
	//so that drawing is not wasted (causes flicker)
	//   NOTE: check is overriden if it is objects first draw
	if((posX != g.Round(futureX) || posY != g.Round(futureY)) 
		|| firstDraw || overrideDraw || colorChange)
	{
		//If it is NOT the first draw, replace the current player's position with the floor
		if(!firstDraw)
		{
			//Round player's position to closest integer, because it can be a decimal
			int posXRound = g.Wrap(0, g.GRID_WIDTH, g.Round(posX));
			int posYRound = g.Wrap(0, g.GRID_HEIGHT, g.Round(posY));
			//Set grid position to NULL because it is floor (empty)
			gameObjects[posYRound][posXRound] = NULL;
			//Place the Floor text
			g.PlaceText(g.FLOOR_TEXT, posXRound, posYRound, g.COLOR_WORLD_NORMAL);
		}

		//Set position to future position
		posX = futureX;
		posY = futureY;

		//Get the collision result based of where the player is going to be
		//   NOTE: checks if object player is going to walk into is enemy,
		//         wall, collectable, etc.
		CollisionResult(gameObjects[g.Round(posY)][g.Round(posX)], this);

		//Set first draw to false because next draw is not the first
		firstDraw = false;
		colorChange = false;

		//}

		//Round object's position to closest integer, because it can be a decimal
		int posXRound = g.Wrap(0, g.GRID_WIDTH, g.Round(posX));
		int posYRound = g.Wrap(0, g.GRID_HEIGHT, g.Round(posY));

		//Only set new position to this if the new position is not the player position
		if(gameObjects[posYRound][posXRound] == NULL || typeid(*gameObjects[posYRound][posXRound]) != typeid(Player))
		{
			gameObjects[posYRound][posXRound] = this;
		}

		//Place symbol
		g.PlaceText(text, posXRound, posYRound, colorWord);
	}
	else
	{
		//If no results after rounding, set position anyway because object's
		// speed could be a decimal, therefore taking multiple updates to move
		// one space
		posX = futureX;
		posY = futureY;
	}
}
//Checks the type of object this is colliding with and performs an action based on the outcome
void GameObject::CollisionResult(GameObject* collisionObj,
								 GameObject* collidingObj)
{
	if(collisionObj != NULL)
	{
		//If type Wall
		if(typeid(*collisionObj).name() == typeid(Wall).name())
		{
			collidingObj->posX -= collidingObj->speedX;
			collidingObj->posY -= collidingObj->speedY;
		}
	}
}
void GameObject::ChangeColor(int ColorWord)
{
	colorWord = ColorWord;
	colorChange = true;
}

//Cell
Cell::Cell()
{
	GameObject::GameObject();
	used = false;
}
Cell::Cell(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	GameObject::GameObject(PosX, PosY, '#', FOREGROUND_BLUE|FOREGROUND_INTENSITY);
	text = '#';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedRate = 0;
	colorWord = FOREGROUND_BLUE|FOREGROUND_INTENSITY;
	used = false;
	GenerateName();
}

//Wall
Wall::Wall()
{
	GameObject::GameObject();
}
Wall::Wall(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, '&', 0);
	InitializeBasicValues(PosX, PosY, '&', g.levelData.wordColor, 0, 0, 0);
	/*text = '&';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedRate = 0;
	colorWord = 11;
	speedX = 0;
	speedY = 0;
	firstDraw = true;
	GenerateName();*/
}

//Player
Player::Player()
{
	GameObject::GameObject();
}
Player::Player(float PosX, float PosY, int LightDistance)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, '@', FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY);
	InitializeBasicValues(PosX, PosY, '@', YELLOW, 0, 0, 1);
	lightDistanceMinimum = LightDistance;
	lightDistance = 60;
    /*text = '@';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 1;
	colorWord = FOREGROUND_RED|FOREGROUND_GREEN|FOREGROUND_INTENSITY;
	GenerateName();*/
}
//Overrides the Collision Result of GameObject
void Player::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	GameObject::CollisionResult(collisionObj, collidingObj);

	if(collisionObj != NULL)
	{
		//If type Enemy Navigator
		if(typeid(*collisionObj) == typeid(EnemyNavigator))
		{
			isDeadFlag = true;
		}
		//If type Enemy Follower
		else if(typeid(*collisionObj) == typeid(EnemyFollower))
		{
			isDeadFlag = true;
		}
		//If type Item
		else if(typeid(*collisionObj) == typeid(Item))
		{
			collisionObj->isDeadFlag = true;
			collidingObj->posX -= collidingObj->speedX;
			collidingObj->posY -= collidingObj->speedY;
			g.latestExplosionComplete = false;
			g.AddToScore(1);
		}
	}
}
//Enemy
Enemy::Enemy()
{
	GameObject::GameObject();
}
Enemy::Enemy(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, '$', RED);
	InitializeBasicValues(PosX, PosY, '$', RED, 0, 0, 1);
	//direction = 3;
	//collision = false;
	//checkForDestination = false;
	//leftDestX = posX;
	//leftDestY = posY;
    /*text = '$';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 1;
	colorWord = RED;
	direction = 3;
	collision = false;
	checkForDestination = false;
	leftDestX = posX;
	leftDestY = posY;
	GenerateName();*/
}
//AI: navigate maze by hugging the left wall
void Enemy::UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	//float speedXToLeft = 0;
	//float speedYToLeft = 0;
	//SpeedBasedOnDirection((direction + 3) % 4, &speedXToLeft, &speedYToLeft);
	//speedXToLeft = g.Wrap(0, g.GRID_WIDTH, g.Round(posX) + speedXToLeft);
	//speedYToLeft = g.Wrap(0, g.GRID_HEIGHT, g.Round(posY) + speedYToLeft);


	////Check if destination met
	//bool atDestination = false;
	//if(!checkForDestination)
	//	atDestination = true;
	//else if(g.Round(leftDestX) == g.Round(posX) && g.Round(leftDestY) == g.Round(posY) && checkForDestination)
	//{
	//	atDestination = true;
	//	checkForDestination = false;
	//}

	//if(gameObjects[(int)speedXToLeft][(int)speedYToLeft] == NULL && atDestination)
	//{
	//	//Set new direction
	//	direction = (direction + 3) % 4;
	//	SpeedBasedOnDirection(direction, &speedX, &speedY);
	//	//Set destination
	//	leftDestX = posX + speedX;
	//	leftDestY = posY + speedY;
	//	checkForDestination = true;

	//	//Reset Speed
	//	speedX = 0;
	//	speedY = 0;
	//}
	//else if(collision)
	//{
	//	//Reset Speed
	//	speedX = 0;
	//	speedY = 0;
	//	//Set new direction
	//	direction = (direction + 1) % 4;
	//}
	//else
	//{
	//	//move in current Direction
	//	SpeedBasedOnDirection(direction, &speedX, &speedY);
	//	speedX /= speedRate;
	//	speedY /= speedRate;
	//}

	//collision = false;
}
//Overrides the Collision Result of GameObject
void Enemy::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	if(collisionObj != NULL)
	{
		if(typeid(*collisionObj).name() == typeid(Player).name())
			collisionObj->isDeadFlag = true;
	}
}
//Enemy Navigator
EnemyNavigator::EnemyNavigator()
{
	GameObject::GameObject();
}
EnemyNavigator::EnemyNavigator(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, '$', RED);
	InitializeBasicValues(PosX, PosY, '$', RED, 0, 0, 1);
	direction = 3;
	collision = false;
	checkForDestination = false;
	leftDestX = posX;
	leftDestY = posY;
    /*text = '$';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 1;
	colorWord = RED;
	direction = 3;
	collision = false;
	checkForDestination = false;
	leftDestX = posX;
	leftDestY = posY;
	GenerateName();*/
}
//AI: navigate maze by hugging the left wall
void EnemyNavigator::UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	Enemy::UpdateAI(gameObjects);

	float speedXToLeft = 0;
	float speedYToLeft = 0;
	SpeedBasedOnDirection((direction + 3) % 4, &speedXToLeft, &speedYToLeft);
	speedXToLeft = g.Wrap(0, g.GRID_WIDTH, g.Round(posX) + speedXToLeft);
	speedYToLeft = g.Wrap(0, g.GRID_HEIGHT, g.Round(posY) + speedYToLeft);


	//Check if destination met
	bool atDestination = false;
	if(!checkForDestination)
		atDestination = true;
	else if(g.Round(leftDestX) == g.Round(posX) && g.Round(leftDestY) == g.Round(posY) && checkForDestination)
	{
		atDestination = true;
		checkForDestination = false;
	}

	if(gameObjects[(int)speedXToLeft][(int)speedYToLeft] == NULL && atDestination)
	{
		//Set new direction
		direction = (direction + 3) % 4;
		SpeedBasedOnDirection(direction, &speedX, &speedY);
		//Set destination
		leftDestX = posX + speedX;
		leftDestY = posY + speedY;
		checkForDestination = true;

		//Reset Speed
		speedX = 0;
		speedY = 0;
	}
	else if(collision)
	{
		//Reset Speed
		speedX = 0;
		speedY = 0;
		//Set new direction
		direction = (direction + 1) % 4;
	}
	else
	{
		//move in current Direction
		SpeedBasedOnDirection(direction, &speedX, &speedY);
		speedX /= speedRate;
		speedY /= speedRate;
	}

	collision = false;
}
//Returns the enemy's new speed based on the direction it is "facing"
void EnemyNavigator::SpeedBasedOnDirection(int direction, float* speedX, float* speedY)
{
	switch(direction)
	{
		case 0:
			*speedY = -1;
			break;
		case 1:
			*speedX = 1;
			break;
		case 2:
			*speedY = 1;
			break;
		case 3:
			*speedX = -1;
			break;
	}
}
//Overrides the Collision Result of GameObject
void EnemyNavigator::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	Enemy::CollisionResult(collisionObj, collidingObj);

	if(collisionObj != NULL)
	{
		if(typeid(*collisionObj).name() == typeid(Wall).name()
			|| typeid(*collisionObj).name() == typeid(Item).name()
			|| typeid(*collisionObj).name() == typeid(EnemyNavigator).name()
			|| typeid(*collisionObj).name() == typeid(EnemyFollower).name())
		{
			collision = true;
			collidingObj->posX -= collidingObj->speedX;
			collidingObj->posY -= collidingObj->speedY;
		}
	}
}

//Enemy Follower
EnemyFollower::EnemyFollower()
{
	GameObject::GameObject();
}
EnemyFollower::EnemyFollower(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, '$', RED);
	InitializeBasicValues(PosX, PosY, '^', GREEN, 0, 0, 1);
    /*text = '$';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 1;
	colorWord = RED;
	direction = 3;
	collision = false;
	checkForDestination = false;
	leftDestX = posX;
	leftDestY = posY;
	GenerateName();*/
}
//AI: navigate maze by hugging the left wall
void EnemyFollower::UpdateAI(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH], Player* player)
{
	Enemy::UpdateAI(gameObjects);

	float diffX = player->posX - this->posX;
	float diffY = player->posY - this->posY;
	float diff = sqrt((diffX*diffY) + (diffY*diffY));

	if(diff < 10)
	{
		//Normalize X direction
		if(diffX < 0)
			diffX = -1;
		else if(diffX > 0)
			diffX = 1;

		//Normalize Y direction
		if(diffY < 0)
			diffY = -1;
		else if(diffY > 0)
			diffY = 1;

		speedX = diffX / speedRate;
		speedY = diffY / speedRate;
	}
}
//Overrides the Collision Result of GameObject
void EnemyFollower::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	Enemy::CollisionResult(collisionObj, collidingObj);

	if(collisionObj != NULL)
	{
		if(typeid(*collisionObj).name() == typeid(Wall).name()
			|| typeid(*collisionObj).name() == typeid(Item).name()
			|| typeid(*collisionObj).name() == typeid(EnemyNavigator).name()
			|| typeid(*collisionObj).name() == typeid(EnemyFollower).name())
		{
			collidingObj->posX -= collidingObj->speedX;
			collidingObj->posY -= collidingObj->speedY;
		}
	}
}

//Item
Item::Item()
{
	GameObject::GameObject();
}
Item::Item(float PosX, float PosY)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, 'o', PURPLE);
	InitializeBasicValues(PosX, PosY, 'o', PURPLE, 0, 0, 0);
    /*text = 'o';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 0;
	colorWord = PURPLE;
	GenerateName();*/
}
//Overrides the Collision Result of GameObject
void Item::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	if(collisionObj != NULL)
	{
		////If type Player
		//if(typeid(*collisionObj).name() == typeid(Player).name())
		//{
		//	isDeadFlag = true;
		//	g.AddToScore(1);
		//}
	}
}

//Particle
Particle::Particle()
{
	GameObject::GameObject();
}
Particle::Particle(float PosX, float PosY, float SpeedX, float SpeedY, int ColorWord, char text)
{
	//NOTE: find out how to call base class
	//GameObject::GameObject(PosX, PosY, 'o', PURPLE);
	InitializeBasicValues(PosX, PosY, text, ColorWord, SpeedX, SpeedY, 0);
    /*text = 'o';
	textOriginal = text;
	posX = PosX;
	posY = PosY;
	speedX = 0;
	speedY = 0;
	speedRate = 0;
	colorWord = PURPLE;
	GenerateName();*/
}
//Overrides the Collision Result of GameObject
void Particle::CollisionResult(GameObject* collisionObj,
							 GameObject* collidingObj)
{
	if(collisionObj != NULL)
	{
		this->isDeadFlag = true;
	}
}
void Particle::Update(GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	float futureX = posX + speedX;
	float futureY = posY + speedY;

	if(gameObjects[(int)posY][(int)posX] == NULL)
		g.PlaceText(g.FLOOR_TEXT, (int)posX, (int)posY, g.COLOR_WORLD_NORMAL);

	this->posX += this->speedX;
	this->posY += this->speedY;

	if(gameObjects[(int)futureY][(int)futureX] == NULL)
		g.PlaceText(text, (int)posX, (int)posY, colorWord);
}
bool Particle::isDead()
{
	if(this->posX <= 0 || this->posX >= g.GRID_WIDTH - 1
		|| this->posY <= 0 || this->posY >= g.GRID_HEIGHT - 1)
	{
		return true;
	}

	return false;
}


