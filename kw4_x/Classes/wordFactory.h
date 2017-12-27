#pragma once

#include <string>
#include "Singleton.h"

class TextLayer;
class WordFactory : public Singleton<WordFactory>
{
public:
	
	std::string			m_hanWordFactory;
	TextLayer *			m_emptyLayer;


	WordFactory();
	~WordFactory();
	
	std::string			RandomWord();
	TextLayer*		GetEmptyLayer();
	
};
