#include "list.h"
#include "Globals.h"

#pragma once
class Sign
{
public:

	const static G g;

	Sign(int PosX, int PosY, float SpeedX, float SpeedY, int DistanceX, 
					List<char*> textLines, bool ClearAtEnd);
	~Sign(void);
	void Update();
	void Clear();
	void Draw();

	float posX;
	float posY;
	float speedX;
	float speedY;
	List<char*>* textLines;
	bool isDead;

	bool clearAtEnd;
	int posXstart;
	int distanceX;
};

