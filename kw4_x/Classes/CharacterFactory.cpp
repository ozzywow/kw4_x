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
	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = new Character(i);
		m_characterPool.push_back(pCharacter);		
	}
	resetData();
}

void			CharacterFactory::resetData()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();

	const float halfW = frameSize.width  * 0.8f * 0.5f;
	const float halfH = frameSize.height * 0.7f * 0.5f;
	const float centerX = frameSize.width  * 0.5f;
	const float centerY = frameSize.height * 0.5f;

	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		pCharacter->init(i);

		pCharacter->posX = centerX + (CCRANDOM_0_1() * 2.0f - 1.0f) * halfW;
		pCharacter->posY = centerY + (CCRANDOM_0_1() * 2.0f - 1.0f) * halfH;
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