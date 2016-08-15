#include "MazeGenerator.h"
#include "GameObject.h"
#include "List.h"

#include <vector>

//Constructof, takes a game Mode (aka Difficulty) and starts maze generation
MazeGenerator::MazeGenerator(MAZE_MODE Mode, int PathSize)
{
	dead = false;
	mode = Mode;
	pathSize = PathSize + 1;

	Generate();
}

//Deconstructor to delete memory when program closed
MazeGenerator::~MazeGenerator(void)
{
	delete usedIndicesX;
	delete usedIndicesY;

	for(int y = 0; y < CELL_GRID_HEIGHT; ++y)
	{
		for(int x = 0; x < CELL_GRID_WIDTH; ++x)
		{
			delete cells[y][x];
			cells[y][x] = NULL;
		}
	}
}

//Called before Update to preset everything
void MazeGenerator::Initialize(void)
{
	usedIndicesX = new List<int>();
	usedIndicesY = new List<int>();

	//Initialize all the positions of the cells of the maze
	for(int y = 0; y < CELL_GRID_HEIGHT; ++y)
	{
		for(int x = 0; x < CELL_GRID_WIDTH; ++x)
		{
			cells[y][x] = new Cell(pathSize + (y * pathSize), pathSize + (x * pathSize));
		}
	}

	//Place a starting wall in the middle of the grid to build from
	cells[g.Round(CELL_GRID_HEIGHT / 2)][g.Round(CELL_GRID_WIDTH / 2)]->used = true;
	usedIndicesX->AddBack(g.Round(CELL_GRID_WIDTH / 2));
	usedIndicesY->AddBack(g.Round(CELL_GRID_HEIGHT / 2));
}
//Presets anything BEFORE generation
//  NOTE: add extra data her later
void MazeGenerator::Generate(void)
{
	//  NOTE: add extra data her later

	Initialize();
}
//Adds a new "Wall" to the inputed array.
// A "Wall" is defined as 2 Walls placed between 2 cells
void MazeGenerator::CreateNewWall(GameObject *gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH])
{
	////Check if dead
	//int cellUsedCount = 0;
	//for(int y = 0; y < gridHeight; ++y)
	//{
	//	for(int x = 0; x < gridWidth; ++x)
	//	{
	//		if(cells[y][x]->used)
	//			cellUsedCount++;
	//	}
	//} 

	/*if(cellUsedCount == gridWidth * gridHeight)
	{
		dead = true;
		return;
	}*/

	//Check Old Walls to remove from the indeices list
	for(int i = 0; i < usedIndicesX->length; ++i)
	{
		if(cells[usedIndicesY->Get(i)][usedIndicesX->Get(i)] != NULL)
		{
			if(CheckCellDead(usedIndicesX->Get(i), usedIndicesY->Get(i)))
			{
				usedIndicesX->RemoveAt(i);
				usedIndicesY->RemoveAt(i);
			}
		}
	}

	//If there are no used indices, maze is dead
	if(usedIndicesX->length == 0 && usedIndicesY->length == 0)
	{
		dead = true;
		return;
	}

	int indexX = 0;
	int indexY = 0;

	int tryCount = 0;
	//Find a random cell that is Used, but not dead 
	//   (aka has an attaching wall, but is not surrounded)
	do
	{
		int randomIndex = g.Random(0, usedIndicesX->length - 1);
		indexX = usedIndicesX->Get(randomIndex);
		indexY = usedIndicesY->Get(randomIndex);
		tryCount++;
	}
	while(!cells[indexY][indexX]->used && tryCount < 50);

	//Check if looping for too long, then kill maze
	if(tryCount >= 50)
	{
		dead = true;
		return;
	}

	int directionX = 0;
	int directionY = 0;

	//Randomly change X direction
	if(g.Random(0, 2))
	{
		if(g.Random(0, 2))
			directionX = -1;
		else
			directionX = 1;
	}
	//Randomly change Y direction
	else
	{
		if(g.Random(0, 2))
			directionY = -1;
		else
			directionY = 1;
	}

	//Get new index based on old index + direction
	int newIndexX = indexX + directionX;
	int newIndexY = indexY + directionY;

	//Check if new index is not outside of the grid
	if(newIndexX >= 0 && newIndexX < CELL_GRID_WIDTH
					&& newIndexY >= 0 && newIndexY < CELL_GRID_HEIGHT)
	{
		//Check if the cell is not used (aka built a wall from)
		if(!cells[newIndexY][newIndexX]->used)
		{
			//Set the cell to used so that it is not written over twice
			cells[newIndexY][newIndexX]->used = true;
			//Add the cell's indices to the 'used' list so that it can be built from
			usedIndicesX->AddBack(newIndexX);
			usedIndicesY->AddBack(newIndexY);

			//Maze mode built in Annoying mode
			//Wall* w1 = new Wall(cells[newIndexY][newIndexX]->posX, 
			//							cells[newIndexY][newIndexX]->posY);
			//Wall* w2 = new Wall(cells[newIndexY][newIndexX]->posX - directionX, 
			//							cells[newIndexY][newIndexX]->posY - directionY);
			switch(mode)
			{
				case Fun:
					//Add distance if in Fun mode
					/*w1 = new Wall(cells[newIndexX][newIndexY]->posX, 
										cells[newIndexX][newIndexY]->posY);
					w2 = new Wall(cells[newIndexX][newIndexY]->posX + directionX, 
										cells[newIndexX][newIndexY]->posY + directionY);*/
					directionX *= 1;
					directionY *= 1;
					break;
				case Difficult:
					//Add and subtract distance if in difficult mode
					/*w1 = new Wall(cells[newIndexX][newIndexY]->posX, 
										cells[newIndexX][newIndexY]->posY);
					w2 = new Wall(cells[newIndexX][newIndexY]->posX - directionX, 
										cells[newIndexX][newIndexY]->posY + directionY);*/
					directionX *= -1;
					directionY *= 1;
					break;
				case Annoying:
					directionX *= -1;
					directionY *= -1;
					break;

			}
			for(int i = 0; i < pathSize; ++i)
			{
				Wall* w1 = new Wall(cells[newIndexY][newIndexX]->posX + (directionX * i), 
									cells[newIndexY][newIndexX]->posY + (directionY * i));

				//Update walls so that they are drawn
				w1->Update(gameObjects);
			}

			//Change value to delay maze generation each update
			//Sleep(0);
		}
	}
}
//Checks if a cell is surrounded on all four sides, if yes returns true
bool MazeGenerator::CheckCellDead(int indexX, int indexY)
{
	int count = 0;

	//If cell Left
	if(indexY - 1 >= 0)
	{
		if(cells[indexY - 1][indexX]->used)
			count++;
	}
	else
		count++;
	//If cell Right
	if(indexY + 1 < CELL_GRID_HEIGHT)
	{
		if(cells[indexY + 1][indexX]->used)
			count++;
	}
	else
		count++;
	//If cell Up
	if(indexX - 1 >= 0)
	{
		if(cells[indexY][indexX - 1]->used)
			count++;
	}
	else
		count++;
	//If cell Down
	if(indexX + 1 < CELL_GRID_WIDTH)
	{
		if(cells[indexY][indexX + 1]->used)
			count++;
	}
	else
		count++;

	//If count == 4, aka cell is surrounded
	return count == 4;
}
//Determines if a specified position is a dead end
// Dead End means only 3 surrounding walls exactly
bool MazeGenerator::isCellDeadEnd(GameObject *gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH], int indexX, int indexY)
{
	//Return if the checked cell is NOT empty
	if(gameObjects[indexY][indexX] != NULL)
		return false;

	int adjacementWallsCount = 0;

	//X checks
	if(indexX - 1 >= 0)
	{
		if(gameObjects[indexY][indexX - 1] != NULL)
		{
			if(typeid(*gameObjects[indexY][indexX - 1]) == typeid(Wall))
				adjacementWallsCount++;
		}
	}
	if(indexX + 1 < g.GRID_WIDTH)
	{
		if(gameObjects[indexY][indexX + 1] != NULL)
		{
			if(typeid(*gameObjects[indexY][indexX + 1]) == typeid(Wall))
				adjacementWallsCount++;
		}
	}
	//Y checks
	if(indexY - 1 >= 0)
	{
		if(gameObjects[indexY - 1][indexX] != NULL)
		{
			if(typeid(*gameObjects[indexY - 1][indexX]) == typeid(Wall))
				adjacementWallsCount++;
		}
	}
	if(indexY + 1 < g.GRID_HEIGHT)
	{
		if(gameObjects[indexY + 1][indexX] != NULL)
		{
			if(typeid(*gameObjects[indexY + 1][indexX]) == typeid(Wall))
				adjacementWallsCount++;
		}
	}

	if(adjacementWallsCount == 3)
	{
		return true;
	}

	return adjacementWallsCount == 3;
}
