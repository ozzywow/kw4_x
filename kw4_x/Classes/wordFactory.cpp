#include "stdafx.h"
#include "TextLayer.h"
#include "wordFactory.h"

WordFactory::WordFactory()
{
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename("word_factory.txt");
#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	cocos2d::Data fileData = FileUtils::getInstance()->getDataFromFile(fullpath);
	if (fileData.getSize() > 0)
	{
		m_hanWordFactory = std::string(reinterpret_cast<const char*>(fileData.getBytes()), fileData.getSize());
		auto pos = m_hanWordFactory.find_first_of("\r\n");
		if (pos != std::string::npos) m_hanWordFactory = m_hanWordFactory.substr(0, pos);
	}
#else
	char buffer[258];
	FILE* fp = fopen(fullpath.c_str(), "r");
	fgets(buffer, sizeof(buffer), fp);
	fclose(fp);
	m_hanWordFactory = buffer;
#endif
}

WordFactory::~WordFactory()
{	
}

std::string	WordFactory::RandomWord()
{
	int maxSize = m_hanWordFactory.size() / 3;
	int randID = rand() % maxSize;
	randID *= 3;
	
	std::string resChar = m_hanWordFactory.substr(randID, 3);
	return resChar;
}


TextLayer*		WordFactory::CreateEmptyLayer() 
{
	auto emptyLayer = TextLayer::createWithWordText(NULL, Point(0, 0), std::string(""));
	return emptyLayer;
}
