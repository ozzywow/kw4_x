#pragma once

#include "cocos2d.h"
#include "Character.h"
#include <vector>
#include "Singleton.h"


class CharacterFactory : public Singleton<CharacterFactory>
{
private:

	std::vector<Character*>	        m_characterPool;

public:
	CharacterFactory();
	~CharacterFactory();	
	

	void			reset();
	Character*		NewCharacter();
	Character*		GetCharacterWithID(int id);
	int				FindAppleIDByPos(Point pos);
	int				GetCountWithType(int ty);
};

