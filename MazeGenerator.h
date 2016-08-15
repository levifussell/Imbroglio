#include "GameObject.h"
#include "Globals.h"

#pragma once

enum MAZE_MODE
{
	Fun, Difficult, Annoying
};

class MazeGenerator
{
public:

	static const G g;

	MazeGenerator(MAZE_MODE mode, int PathSize);
	~MazeGenerator(void);
	Cell* cells[g.GRID_HEIGHT / 2 - 1][g.GRID_WIDTH / 2 - 2];
	List<int>* usedIndicesX;
	List<int>* usedIndicesY;
	MAZE_MODE mode;

	//NOTE: look AT .cpp file for definitions
	void Initialize(void);
	void Generate(void);
	void CreateNewWall(GameObject *gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH]);
	bool CheckCellDead(int indexX, int indexY);
	bool isCellDeadEnd(GameObject *gameObjects[g.GRID_HEIGHT][g.GRID_WIDTH], int indexX, int indexY);

	static const int CELL_GRID_WIDTH = g.GRID_WIDTH / 2 - 2;
	static const int CELL_GRID_HEIGHT = g.GRID_HEIGHT / 2 - 2;
	int pathSize;
	bool dead;
};

