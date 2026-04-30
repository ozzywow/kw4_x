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

// 캐릭터 풀을 초기화한다. 모든 캐릭터의 type을 CT_NONE으로 바꾼다.
void			CharacterFactory::resetData()
{
	const float cx     = CalcCenterX();
	const float cy     = CalcCenterY();
	const float rangeX = cx * 0.7f;
	const float rangeY = cy * 0.7f;

	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		// 화면의 중앙을 기준으로 70% 범위 내 랜덤한 위치에 캐릭터를 배치한다.
		Character* pCharacter = m_characterPool[i];
		pCharacter->init(i);
		pCharacter->posX = cx + (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * rangeX;
		pCharacter->posY = cy + (((float)rand() / RAND_MAX) * 2.0f - 1.0f) * rangeY;
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
	const float FIND_RANGE = FRAME_WIDTH * 0.10f; // 화면 너비의 약 10% (~100px)

	for (int i = 0; i < MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = m_characterPool[i];
		if (pCharacter->type != CT_APPLE) { continue; }

		float dx = pos.x - pCharacter->posX;
		float dy = pos.y - pCharacter->posY;
		if (std::abs(dx) < FIND_RANGE && std::abs(dy) < FIND_RANGE)
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