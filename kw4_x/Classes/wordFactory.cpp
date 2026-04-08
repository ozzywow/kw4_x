#include "stdafx.h"
#include "TextLayer.h"
#include "wordFactory.h"

WordFactory::WordFactory()
{	
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename("word_factory.txt");
	char buffer[258];
	FILE* fp = fopen(fullpath.c_str(), "r");
	fgets(buffer, sizeof(buffer), fp);
	fclose(fp);

	m_hanWordFactory = buffer;
	// word_factory.txt is UTF-8; no conversion needed
	std::string initStr("");
	m_emptyLayer = TextLayer::createWithWordText(NULL, Point(0, 0), initStr);
}

WordFactory::~WordFactory()
{	
	m_emptyLayer->autorelease();
}

std::string	WordFactory::RandomWord()
{
	int maxSize = m_hanWordFactory.size() / 3;
	int randID = rand() % maxSize;
	randID *= 3;
	
	std::string resChar = m_hanWordFactory.substr(randID, 3);
	return resChar;
}


TextLayer*		WordFactory::GetEmptyLayer() 
{
	return m_emptyLayer;
}
