#include "stdafx.h"
#include "Character.h"

Character::Character(int id)
{
	index = id;
	type = CT_NONE;
	posX = 0.0f;
	posY = 0.0f;
	biteCount = 0;
}

Character::~Character()
{

}