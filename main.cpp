#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <Windows.h>
#include <time.h>
#include <vector>
#include <iostream>
#include <fstream> 


#include "Globals.h"
#include "GameObject.h"
#include "MazeGenerator.h"
#include "List.h"
#include "Explosion.h"
#include "Button.h"
#include "Sign.h"


enum Menu
{
	MAIN, GAME, HELP
};

//Structs
struct Key
{
	int vKey;
	bool oncePressed;
	bool isPressed;
};

//Global
static const G g;

//Variables
int menuCurrent;

//Menu
Button* buttonPlay;
Button* buttonExit;
Button* buttonHelp;
Sign* signTitle;
Sign* signLevel;

//Game
Player *player;
GameObject *gameObjects[g.GRID_WIDTH][g.GRID_HEIGHT];

bool update;

//Keys
void UpdateKeys();
Key KeyCheck(Key k);
Key keyLeft;
Key keyRight;
Key keyDown;
Key keyUp;
Key keyE;

//Explosion
List<Explosion*>* explosions;
void UpdateExplosions();

//Function Initialize
void Initialize();
void Draw();
void Exit();

//Menu Update
void MenuUpdate();
void TransitionMenu(int newMenu);

//Updates for gameObjects
void UpdateObjects();
void UpdateTypeSpecific(GameObject *gameObject, GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
void UpdateControl(GameObject *gameObject);

//Other Initializes
void LoadHighScore();
void NewHighScore(int score);
void LoadMain();
void LoadGame();
void LoadHelp();
void LoadLevel(int levelIndex);
void CreateMap();
void CreateKeys();
void AddItemsAtEnds(int posX, int posY);

//Maze Generator that creates and places walls
MazeGenerator *mazeGenerator;

//Initialize called before Update to preset everything
void Initialize()
{
	//Set screen Dimension
	system("mode 100, 70");
	//Set screen colour
	system("color 00");

	/*keybd_event(VK_MENU,0x38,0,0);
	keybd_event(VK_RETURN,0x1c,0,0);
	keybd_event(VK_RETURN,0x1c,KEYEVENTF_KEYUP,0);
	keybd_event(VK_MENU,0x38,KEYEVENTF_KEYUP,0);*/

	//system("position 0, 0");
//#define size 1024;
//	LPWSTR pszOldWindowTitle[size];
//	HWND h;
//	h = FindWindow(NULL, GetConsoleTitle(pszOldWindowTitle, size));
	/*WINDOWPLACEMENT p;
	HWND foreGroundWindow = GetForegroundWindow();
	GetWindowPlacement(foreGroundWindow, &p);
	p.rcNormalPosition.left = 10;
	SetWindowPlacement(foreGroundWindow, &p);*/

	//******GET display resolution data
	HWND foreGround = GetForegroundWindow();
	RECT clientRect;
	GetClientRect(foreGround, &clientRect);
	int dwWidth = clientRect.right;
	int dwHeight = clientRect.bottom;

	//******SET cmd window to top left and display size
	RECT windowRec;
	windowRec.left = dwWidth / 4;
	windowRec.top = 0;
	windowRec.bottom = dwHeight;
	windowRec.right = dwWidth * 2;
	g.SetWindowData(windowRec);

	//Set random seed
	srand(time(0)); 

	TransitionMenu(MAIN);
}
void LoadMain()
{
	if(g.highScore < g.levelCurrent)
	{
		NewHighScore(g.levelCurrent);
	}

	system("cls");

	WINDOWPLACEMENT window = g.GetWindowData();

	buttonPlay = new Button(25, 30, 40, 5, "PLAY", 4);
	buttonHelp = new Button(28, 38, 15, 5, "HELP", 4);
	buttonExit = new Button(47, 38, 15, 5, "EXIT", 4);

	List<char*>* texts = new List<char*>();
	texts->AddBack(" _____            |                _____            ");
	texts->AddBack("   |     ___ ___  |      ___  __  |     | |   >  __ ");
	texts->AddBack("   |    |   |   | |---  |    |  | |     | |     |  |");
	texts->AddBack("   |    |   |   | |   | |    |  | |    __ |   | |  |");
	texts->AddBack(" __|__  |   |   | |___| |    |__| |_____| |_/ | |__|");
	signTitle = new Sign(0, 15, 0.1f, 0, 18, *texts, false);

	LoadHighScore();
	//Place Highscore text
	g.PlaceText("HIGH SCORE: ", 38, 26, WHITE);

	char value[sizeof(int)*8+1];
	char* highScoreText = itoa(g.highScore, value, 10);
	g.PlaceText(highScoreText, 50, 26, YELLOW);

	g.PlaceText("By Levi Fussell", 10, 50, WHITE);
}
void LoadHelp()
{
	system("cls");

	WINDOWPLACEMENT window = g.GetWindowData();

	buttonExit = new Button(65, 45, 15, 5, "RETURN", 6);

	List<char*>* texts = new List<char*>();
	texts->AddBack("|   |                ");
	texts->AddBack("|   |  ___  |        ");
	texts->AddBack("|___| |   | |    __  ");
	texts->AddBack("|   | |___| |   |  | ");
	texts->AddBack("|   | |___  |_/ |__| ");
	texts->AddBack("                |    ");
	texts->AddBack("                |    ");
	signTitle = new Sign(0, 10, 0.1f, 0, 33, *texts, false);

	g.PlaceText("- Navigate the random mazes carefully!", 15, 20, WHITE);
	g.PlaceText("- Collect all purple orbs ", 15, 21, WHITE);
	g.PlaceText("o", 41, 21, PURPLE);
	g.PlaceText(" to proceed to the next level", 42, 21, WHITE);
	g.PlaceText("- Use arrow keys to move", 15, 22, WHITE);
	g.PlaceText("- ", 15, 24, WHITE);
	g.PlaceText("$", 17, 24, RED);
	g.PlaceText(" move orderly and halt before changing direction", 19, 24, WHITE);
	g.PlaceText("- ", 15, 26, WHITE);
	g.PlaceText("^", 17, 26, GREEN);
	g.PlaceText(" can move diagonally and follow the player within 10 spaces", 19, 26, WHITE);
	g.PlaceText("- Try to beat your HIGHSCORE!!", 15, 28, WHITE);
}
void LoadGame()
{
	LoadLevel(1);
}
void LoadLevel(int levelIndex)
{
	system("cls");

	//Game border
	for(int y = 0; y < g.GRID_HEIGHT + 2; y++)
	{
		for(int x = 0; x < g.GRID_WIDTH + 2; x++)
		{
			if(x == 0 || x == g.GRID_WIDTH + 1
				|| y == 0 || y == g.GRID_HEIGHT + 1)
			{
				g.PlaceText( ' ', x - 1, y - 1, BACKGROUND_RED);
			}
		}
	}
	//g.PlaceText( ' ', g.GRID_WIDTH, 0, BACKGROUND_GREEN);

	//Game Data
	g.gameScore = 0;
	g.previousScore = -1;
	g.levelCurrent = levelIndex;
	g.levelPrevious = -1;
	g.levelPlacedScores = 0;

	//Reset all grid values
	for(int y = 0; y < g.GRID_HEIGHT; ++y)
	{
		for(int x = 0; x < g.GRID_WIDTH; ++x)
		{
			gameObjects[y][x] = NULL;
		}
	}

	//Creates
	delete mazeGenerator;
	mazeGenerator = new MazeGenerator(Fun, 1);
	CreateMap();
	CreateKeys();

	//Explosions
	explosions = new List<Explosion*>();

	//UI
	//g.UpdateGameUI();

	//Set level Data
	//double power = g.levelCurrent;
	//pow(power, 2);
	g.levelData.scoreToWin = 1 + g.levelCurrent;
	g.levelData.wordColor = max((g.levelCurrent) % 15, 1);
}
//File Loading
void LoadHighScore()
{
	try
	{
		//Create new stream
		std::fstream fileStr;
		fileStr.open("Scores.imbr", std::fstream::in | std::fstream::out | std::fstream::app);

		/*char value[sizeof(int)*8+1];
		char* scoreValue = itoa(g.highScore, value, 10);
		int length = 0;
		for(int i = 0; scoreValue[i] !=NULL; ++i)
		{
			length++;
		}*/
		char highScoreText [20];
		int charIndex = 0;

		//Itterate until end of file
		while(!fileStr.eof())
		{
			//Get next char
			char nextText[1];
			fileStr.read(nextText, 1);

			if(fileStr.eof() || charIndex > 50)
				break;

			//write char to highscores to get total number
			highScoreText[charIndex] = nextText[0];
			charIndex++;
		}

		fileStr.close();

		//Convert char* to highscore int
		g.highScore = atoi(highScoreText);
	}
	catch(char* error)
	{
		g.highScore = 0;
	}
}
void NewHighScore(int score)
{
	try
	{
		g.highScore = score;

		//Open new stream
		std::fstream fileStr;
		//delete file to reset data
		remove("Scores.imbr");
		fileStr.open("Scores.imbr", std::fstream::in | std::fstream::out | std::fstream::app);
		//Convert highscore to char*
		char value[sizeof(int)*8+1];
		char* scoreValue = itoa(g.highScore, value, 10);

		//get length of char*
		int length = 0;
		for(int i = 0; scoreValue[i] !=NULL; ++i)
		{
			length++;
		}
		/*for(int i = length - 1; i < 4; ++i)
		{
			scoreValue[4 - i] = scoreValue[i];
		}*/

		//Write char* to file
		fileStr.write(scoreValue, length);
		fileStr.close();
	}
	catch(char* error)
	{
	}
}

//Called every Update
int main()
{
	Initialize();
	
	//Update
	while(true)
	{
		//update = false;

		switch(menuCurrent)
		{
			case GAME:
				if(signLevel != NULL)
					signLevel->Update();

				g.UpdateGameUI();

				UpdateKeys();
				UpdateObjects();
				UpdateExplosions();
		
				//Check if level score reached, wait for explosions and
				// then go to next level
				if(g.gameScore >= g.levelData.scoreToWin && explosions->length == 0
					&& g.latestExplosionComplete)
				{
					//Activate WIN sign that moves right across the screen
					//if(signLevel == NULL)
					//{
					//	List<char*>* texts = new List<char*>();
					//	texts->AddBack("|       |        ");
					//	texts->AddBack("|       | >  ___ ");
					//	texts->AddBack("|   |   |   |   |");
					//	texts->AddBack("|   |   | | |   |");
					//	texts->AddBack("|___|___| | |   |");
					//	signLevel = new Sign(0, 15, 0.6f, 0, 80, *texts, true);
					//}
					////If the WIN sign is dead, proceed to next level
					//else if(signLevel->isDead && signLevel != NULL)
					//{
					//	delete signLevel;
						LoadLevel(g.levelCurrent + 1);
					//}
				}

				//Create new walls while mazeGenerator has room
				if(!mazeGenerator->dead)
					mazeGenerator->CreateNewWall(gameObjects);
				else
				{
					if(player->lightDistance >= player->lightDistanceMinimum && explosions->length == 0)
					{
						player->lightDistance -= 0.2f;
					}

					//Set old level to new level so UI is drawable
					g.levelPrevious = g.levelCurrent;

					while(g.levelPlacedScores < g.levelData.scoreToWin)
					{
						int randX = g.Random(0, g.GRID_WIDTH - 1);
						int randY = g.Random(0, g.GRID_HEIGHT - 1);
						if(mazeGenerator->isCellDeadEnd(gameObjects, randX, randY))
						{
							gameObjects[randY][randX] = new Item(randX, randY);
							gameObjects[randY][randX]->Update(gameObjects);
							explosions->AddBack(new Explosion(randX, randY,
												gameObjects[randY][randX]->colorWord, '*', 10));
							g.levelPlacedScores++;
						}
					}
				}

				//Exit game with Backspace press
				if(keyE.isPressed)
					explosions->AddBack(new Explosion(g.GRID_WIDTH / 2, g.GRID_HEIGHT / 2, 
														g.Random(0, 15), '*', 12));
				break;
			case MAIN:
				MenuUpdate();
				//g.UpdateGameUI();
				signTitle->Update();
				break;
			case HELP:
				//Exit Button Pressed
				buttonExit->Update();
				if(buttonExit->state == PRESSED)
				{
					Sleep(100);
					TransitionMenu(MAIN);
				}
				signTitle->Update();
				break;
		}
	}
}
//Call this to exit the game and clear up memory
void Exit()
{
	for(int y = 0; y < g.GRID_HEIGHT; ++y)
	{
		for(int x = 0; x < g.GRID_WIDTH; ++x)
		{
			delete gameObjects[y][x];
		}
	}

	delete mazeGenerator;
	exit(EXIT_SUCCESS);
}

//--------------Functions:

//Create Functions:
//Places starting object positions...etc.
void CreateMap()
{
	//Create a new player
	//player = new Player(mazeGenerator->pathSize - 1, mazeGenerator->pathSize - 1, 25);
	player = new Player(g.GRID_WIDTH / 2, g.GRID_HEIGHT / 2, 14);
	player->Update(gameObjects);

	//Create a new Enemy
	//Enemy1 *enemy = new Enemy1(g.GRID_WIDTH / 2, g.GRID_HEIGHT / 2);
	//enemy->Update(gameObjects);
}
//Initialize the keys with what key they are recieving input from
void CreateKeys()
{
	keyLeft.vKey = VK_LEFT;
	keyRight.vKey = VK_RIGHT;
	keyUp.vKey = VK_UP;
	keyDown.vKey = VK_DOWN;
	keyE.vKey = VK_BACK;
}
//Places Items at end positions in mazes based on random values
void AddItemsAtEnds(int posX, int posY)
{
	//Add an Enemy Navigator
	if(g.Random(0, 4000) == 1)
	{
		gameObjects[posY][posX] = new EnemyNavigator(posX, posY);
		gameObjects[posY][posX]->Update(gameObjects);
	}
	//Add Item
	/*else if(g.Random(0, 8000) == 1 && g.levelPlacedScores < g.levelData.scoreToWin)
	{
		gameObjects[posY][posX] = new Item(posX, posY);
		gameObjects[posY][posX]->Update(gameObjects);
		g.levelPlacedScores++;
	}*/
	//Add Enemy Follower
	else if(g.Random(0, 14000) == 1)
	{
		gameObjects[posY][posX] = new EnemyFollower(posX, posY);
		gameObjects[posY][posX]->Update(gameObjects);
	}
}

//Updates Functions:
//Update each object in the array if NOT null
void UpdateObjects()
{
	//int v = sizeof(gameObjects);
	//for(int i = 0; i < gameObjects->length; ++i)
	//{
	//	if(gameObjects->Get(i) != NULL)
	//	{
	//		//gameObjects->Get(i)->speedX = -0.001;
	//		//gameObjects->Get(i)->speedY = 0;
	//		gameObjects->Get(i)->Update(gameObjects);
	//	}
	//}


	int playerX = -1;
	int playerY = -1;
	//Set all gameobjects to state: Not Updated
	for(int y = 0; y < g.GRID_HEIGHT; y++)
	{
		for(int x = 0; x < g.GRID_WIDTH; x++)
		{
			if(gameObjects[y][x] != NULL)
			{
				if(typeid(*gameObjects[y][x]) == typeid(Player))
				{
					playerX = x;
					playerY = y;
				}

				if(gameObjects[y][x]->isDeadFlag)
				{
					if(typeid(*gameObjects[y][x]) == typeid(Player))
					{
						Sleep(100);
						TransitionMenu(MAIN);
						return;
					}

					explosions->AddBack(new Explosion(x, y,
													gameObjects[y][x]->colorWord, '*', 12));
					g.latestExplosionComplete = true;

					gameObjects[y][x] = NULL;
				}
				else //if(typeid(*gameObjects[y][x]) != typeid(Player))
					gameObjects[y][x]->updated = false;
			}
		}
	}

	if(player->speedX == 0 && player->speedY == 0 && mazeGenerator->dead
		&& player->lightDistance <= player->lightDistanceMinimum)
	{
		player->action = false;
	}
	else
		player->action = true;

	//Check if player has not been player when enemy should have
	// NOTE: occurs when overwritten by moving enemy
	/*if(playerX == -1 && playerY == -1)
	{
		Sleep(100);
		TransitionMenu(MAIN);
		return;
	}*/

	/*if(player->colorWord == RED)
		player->ChangeColor(YELLOW);
	else
		player->ChangeColor(RED);*/

	//UpdateTypeSpecific(gameObjects[g.Round(player->posY)][g.Round(player->posX)], gameObjects);
	//gameObjects[g.Round(player->posY)][g.Round(player->posX)]->Update(gameObjects);

	//Update Game Objects
	for(int y = 0; y < g.GRID_HEIGHT; y++)
	{
		for(int x = 0; x <  g.GRID_WIDTH; x++)
		{
			if(gameObjects[y][x] != NULL)
			{
				if(!player->action && gameObjects[y][x] != player)
					gameObjects[y][x]->updated = true;

				if(!gameObjects[y][x]->updated)
				{
					//Determine distance from player:

					//Squash Value: value to multiply Y distance by to "squash" the oval light into a circle
					float squashValue = 1.6f;
					float distX = abs(gameObjects[y][x]->posX - player->posX);
					float distY = abs(gameObjects[y][x]->posY - player->posY) * squashValue;
					//If hypotenuse of distance is less than light range, 
					// draw the object dark, else draw it light
					if(sqrt((distX*distX) + (distY*distY)) < player->lightDistance)
						gameObjects[y][x]->drawDark = false;
					else
						gameObjects[y][x]->drawDark = true;

					/*if(typeid(*gameObjects[y][x]) != typeid(Player))
					{*/
					if(player->lightDistance <= player->lightDistanceMinimum)
						UpdateTypeSpecific(gameObjects[y][x], gameObjects);

					gameObjects[y][x]->Update(gameObjects);
					//}
				}
			}

			if(mazeGenerator->isCellDeadEnd(gameObjects, x, y) && !mazeGenerator->dead)
			{
				AddItemsAtEnds(x, y);
			}
		}
	}
}
//Update the game object based on its type (used for unique updates like AI and keyboard controls)
void UpdateTypeSpecific(GameObject* gameObject, GameObject* gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	//If player
	if(typeid(*gameObject) == typeid(Player))
	{
		UpdateControl(gameObject);

		//Draws to Output window
		/*char value[sizeof(int)*8+1];
		itoa(g.Round(gameObject->speedX), value, 10);
		OutputDebugStringA("\nPosX = ");
		OutputDebugStringA(value);

		char value2[sizeof(int)*8+1];
		itoa(g.Round(gameObject->speedY), value2, 10);
		OutputDebugStringA("\nPosY = ");
		OutputDebugStringA(value2);*/
	}
	//If enemy Navigator
	else if(typeid(*gameObject) == typeid(EnemyNavigator) && mazeGenerator->dead)
	{
		EnemyNavigator* enemyTemp = (EnemyNavigator*)gameObject;
		enemyTemp->UpdateAI(gameObjects);
	}
	//If enemy Follower
	else if(typeid(*gameObject) == typeid(EnemyFollower) && mazeGenerator->dead)
	{
		EnemyFollower* enemyTemp = (EnemyFollower*)gameObject;
		enemyTemp->UpdateAI(gameObjects, player);
	}
}
//Update Keyboard controls, input gameObject to be controlled by the keyboard
void UpdateControl(GameObject *gameObject)
{
	//Controls
				
	//X movement
	if(keyLeft.isPressed)
		gameObject->speedX = -1;
	else if(keyRight.isPressed)
		gameObject->speedX = 1;
	else
		gameObject->speedX = 0;

	//Y movement
	if(keyUp.isPressed)
		gameObject->speedY = -1;
	else if(keyDown.isPressed)
		gameObject->speedY = 1;
	else
		gameObject->speedY = 0;
}
//
void UpdateExplosions()
{
	for(int i = 0; i < explosions->length; ++i)
	{
		explosions->Get(i)->Update(gameObjects);

		if(explosions->Get(i)->isDead())
		{
			explosions->RemoveAt(i);
		}
	}
}
//Checks button presses and menu transition
void MenuUpdate()
{
	//Play Button Pressed
	buttonPlay->Update();
	if(buttonPlay->state == PRESSED)
	{
		Sleep(100);
		TransitionMenu(GAME);
		return;
	}
	//Help Button Pressed
	buttonHelp->Update();
	if(buttonHelp->state == PRESSED)
	{
		Sleep(100);
		TransitionMenu(HELP);
		return;
	}
	//Exit Button Pressed
	buttonExit->Update();
	if(buttonExit->state == PRESSED)
	{
		Sleep(100);
		Exit();
	}
}
void TransitionMenu(int newMenu)
{
	menuCurrent = newMenu;

	switch(menuCurrent)
	{
		case MAIN:
			LoadMain();
			break;
		case GAME:
			LoadGame();
			break;
		case HELP:
			LoadHelp();
			break;
	}
}

//Key Functions:
//
void UpdateKeys()
{
	keyLeft = KeyCheck(keyLeft);
	keyRight = KeyCheck(keyRight);
	keyUp = KeyCheck(keyUp);
	keyDown = KeyCheck(keyDown);
	keyE = KeyCheck(keyE);
}
//Returns a state of a key (Pressed once, held, released)
Key KeyCheck(Key k)
{
	//Presse automatically equals false
	k.isPressed = false;

	//If key is pressed and the last key state was NOT pressed
	if(GetAsyncKeyState(k.vKey) != 0 && !k.oncePressed)
	{
		//Set pressed to true and last key state to pressed
		k.oncePressed = true;
		k.isPressed = true;
	}
	//If key is NOT pressed and last key state equals pressed, set pressed to false
	else if(GetAsyncKeyState(k.vKey) == 0 && k.oncePressed)
		k.oncePressed = false;

	return k;
}

////Draw Functions:
////A single first call that draws the starting layout of the screen
//// NOTE: Remove later on (not necessary)
//void Draw()
//{
//	for(int y = 0; y < g.GRID_HEIGHT; ++y)
//	{
//		for(int x = 0; x < g.GRID_WIDTH; ++x)
//		{
//			//Draw Floor Text
//			g.DrawChar(g.FLOOR_TEXT, g.COLOR_WORLD_NORMAL);
//		}
//
//		printf("\n");
//	}
//}
