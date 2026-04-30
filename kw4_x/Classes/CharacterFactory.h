#pragma once

#include "cocos2d.h"
#include "Character.h"
#include <vector>
#include "Singleton.h"


class CharacterFactory : public Singleton<CharacterFactory>
{
public :
	static const int s_maxLaverCount = 3;
private:

	std::vector<Character*>	        m_characterPool;

public:
	CharacterFactory();
	~CharacterFactory();	
	

	void			init();
	void			resetData();
	Character*		NewCharacter();
	Character*		GetCharacterWithID(int id);
	int				FindAppleIDByPos(Point pos);
	int				GetCountWithType(int ty);
	std::vector<Character*>& GetCharacterPool() { return m_characterPool; }

	int				GetCountOfApple() { return GetCountWithType(CT_APPLE); }			
	int				GetCountOfLaver() { return GetCountWithType(CT_LAVER); }
};

