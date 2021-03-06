#include "stdafx.h"
#include "CharacterFactory.h"

CharacterFactory::CharacterFactory()
{
	init();
}
CharacterFactory::~CharacterFactory()
{

}


void			CharacterFactory::init()
{	
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	float H_OFFSET = frameSize.height - FRAME_HEIGHT;

	const float begginXOffset = 50;
	const float Xoffset = 7;
	const float yOffset = 50;

	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		const float  offsetVal = begginXOffset + (i*Xoffset);

		Character* pCharacter = new Character(i);
		pCharacter->posX = offsetVal;
		pCharacter->posY = H_OFFSET + yOffset;

		m_characterPool.push_back(pCharacter);
	}
}

void			CharacterFactory::resetData()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	float H_OFFSET = frameSize.height - FRAME_HEIGHT;

	const float begginXOffset = 50;
	const float Xoffset = 7;
	const float yOffset = 50;

	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		pCharacter->init(i);

		const float  offsetVal = begginXOffset + (i*Xoffset);
		pCharacter->posX = offsetVal;
		pCharacter->posY = H_OFFSET + yOffset;
	}
}

Character*		CharacterFactory::NewCharacter()
{
	for (int i = 0; i< MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		if (pCharacter && pCharacter->type == CT_NONE)
		{
			return pCharacter;
		}
	}
	return NULL;
}
Character*		CharacterFactory::GetCharacterWithID(int id)
{
	if (id < MAX_SIZE_OF_CHARACTER_POOL)
	{
		return m_characterPool[id];
	}
	return NULL;
}

int				CharacterFactory::FindAppleIDByPos(Point pos)
{
	float xx = pos.x;
	float yy = pos.y;
	
	for (int i = 0; i< MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		if (pCharacter->type != CT_APPLE) { continue; }

		int x = pCharacter->posX;
		int y = pCharacter->posY;

		if (xx  < (x + 50) && xx  >(x - 50) &&
			yy  < (y + 50) && yy  >(y - 50))
		{
			return i;
		}
	}

	return -1;
}


int				CharacterFactory::GetCountWithType(int ty)
{
	int res = 0;
	for (int i = 0; i< MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		if (pCharacter && pCharacter->type == ty)
		{
			++res;
		}
	}

	return  res;
}