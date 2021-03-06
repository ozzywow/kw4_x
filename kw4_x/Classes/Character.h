#pragma once

enum CHARACTER_TYPE
{
	CT_NONE = 0,
	CT_APPLE = 1,
	CT_LAVER = 2,
	CT_FLY = 3,
};

class Character
{
public:
	int index;
	int type; //CHARACTER_TYPE
	int posX;
	int posY;
	int biteCount;

	Character(int id = 0 );
	~Character();

	void init(int id = 0)
	{
		index = id;
		type = CT_NONE;
		posX = 0.0f;
		posY = 0.0f;
		biteCount = 0;
	}

};