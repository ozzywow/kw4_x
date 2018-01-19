#pragma execution_character_set("utf-8")

#include<iostream>
#include "stdafx.h"
#include "PointManager.h"
#include "SoundFactory.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "pugixml.hpp"
#include "AppleTreeScene.h"
#include "StudyScene.h"
#include "MainMenuScene.h"
#include "smartAssert.h"
#include "InfoScene.h"

//using namespace pugi;

PointManager::PointManager()
{
	init();

	LoadData();
}

PointManager::~PointManager()
{
	SaveData();
}

void PointManager::init()
{
	m_currStage = 0;
	_point = 0;
	_level = 1;
	_hint = false;

	m_mCardsByWord.clear();
	m_mCardsByLevel.clear();
	m_mMastWords.clear();
	
#ifdef LITE_VER
	m_vCart.assign(PRODUCT_ID::PID_MAX, false);
#else  //LITE_VER
	m_vCart.assign(PRODUCT_ID::PID_MAX, true);
#endif //LITE_VER
}

void	PointManager::SetPoint(int point)
{
	_point = point;
}

int		PointManager::AddPoint(int point)
{
	_point += point;

	if (_point >= 6)
	{
		SoundFactory* sound = SoundFactory::Instance();
		sound->play(SOUND_FILE_excellent_voice);

		for (int i = 0; i<1; ++i)
		{
			Character* pNewCharacter = CharacterFactory::Instance()->NewCharacter();
			if (pNewCharacter)
			{
				pNewCharacter->type = CT_APPLE;
			}
		}


		_point = 0;
	}

	return _point;
}

int		PointManager::DelPoint(int point)
{
	if (_point > 0)
	{
		_point -= 1;
	}

	return _point;
}

int		PointManager::GetPoint()
{
	return _point;
}



void	PointManager::SaveData() 
{
	UserDefault::getInstance()->setIntegerForKey("curr_stage", m_currStage);
	UserDefault::getInstance()->setIntegerForKey("point", _point);
	UserDefault::getInstance()->setIntegerForKey("level", _level);
	UserDefault::getInstance()->setBoolForKey("hint", _hint);

	{
		pugi::xml_document _xmlDocForMastWords;
		pugi::xml_node _rootnode = _xmlDocForMastWords.append_child("root");
		for (auto itr = m_mMastWords.begin(); itr != m_mMastWords.end(); ++itr)
		{
			pugi::xml_node _node = _rootnode.append_child("mastwrod");
			pugi::xml_attribute attrWord = _node.append_attribute("word");
			attrWord.set_value(itr->first.c_str());

			pugi::xml_attribute attrIsMast = _node.append_attribute("ismast");
			attrIsMast.set_value(itr->second);
		}

		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
		path.append("mast_words.xml");
		_xmlDocForMastWords.save_file(path.c_str());
	}

	// character
	{
		pugi::xml_document _xmlDocForMastWords;
		pugi::xml_node _rootnode = _xmlDocForMastWords.append_child("root");

		std::vector<Character*>& characterPool = CharacterFactory::Instance()->GetCharacterPool();
		for (auto itr = characterPool.begin(); itr != characterPool.end(); ++itr)
		{
			Character* character = (*itr);
			pugi::xml_node _node = _rootnode.append_child("character");
			pugi::xml_attribute attrIndex = _node.append_attribute("index");
			attrIndex.set_value(character->index);
			pugi::xml_attribute attrType = _node.append_attribute("type");
			attrType.set_value(character->type);
			pugi::xml_attribute attrX = _node.append_attribute("x");
			attrX.set_value(character->posX);
			pugi::xml_attribute attrY = _node.append_attribute("y");
			attrY.set_value(character->posY);
			pugi::xml_attribute attrBiteCount = _node.append_attribute("bitecount");
			attrBiteCount.set_value(character->biteCount);
		}

		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
		path.append("characters.xml");
		_xmlDocForMastWords.save_file(path.c_str());
	}
	
}

void	PointManager::LoadData() 
{
	m_currStage = UserDefault::getInstance()->getIntegerForKey("curr_stage");
	//if (m_currStage == 0) m_currStage = 1;
	_point = UserDefault::getInstance()->getIntegerForKey("point");
	_level = UserDefault::getInstance()->getIntegerForKey("level");
	if (_level == 0) _level = 1;
	_hint = UserDefault::getInstance()->getBoolForKey("hint");	

	
	// mast_words
	{
		pugi::xml_document _xmlDocForMastWords;
		pugi::xml_node _xmlNode;
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
		path.append("mast_words.xml");
		if (!FileUtils::sharedFileUtils()->isFileExist(path))
		{
			FILE* dest = fopen(path.c_str(), "wb");
			fclose(dest);
		}

		unsigned char* pBuffer = NULL;
		ssize_t bufferSize = 0;
		pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &bufferSize);
		if (_xmlDocForMastWords.load_buffer(pBuffer, bufferSize))
		{
			_xmlNode = _xmlDocForMastWords.child("root");
			for (pugi::xml_node_iterator itr = _xmlNode.begin(); itr != _xmlNode.end(); ++itr)
			{
				std::string word = itr->attribute("word").as_string();
				bool isMast = itr->attribute("ismast").as_bool();
				m_mMastWords[word.c_str()] = isMast;
			}
		}
	}


	// character
	{
		pugi::xml_document _xmlDocForMastWords;
		pugi::xml_node _xmlNode;
		std::string path = CCFileUtils::sharedFileUtils()->getWritablePath();
		path.append("characters.xml");
		if (!FileUtils::sharedFileUtils()->isFileExist(path))
		{
			FILE* dest = fopen(path.c_str(), "wb");
			fclose(dest);
		}

		unsigned char* pBuffer = NULL;
		ssize_t bufferSize = 0;
		pBuffer = CCFileUtils::sharedFileUtils()->getFileData(path.c_str(), "r", &bufferSize);
		if (_xmlDocForMastWords.load_buffer(pBuffer, bufferSize))
		{
			_xmlNode = _xmlDocForMastWords.child("root");
			for (pugi::xml_node_iterator itr = _xmlNode.begin(); itr != _xmlNode.end(); ++itr)
			{
				int index = itr->attribute("index").as_int();
				int type = itr->attribute("type").as_int();
				int x = itr->attribute("x").as_int();
				int y = itr->attribute("y").as_int();
				int biteCount = itr->attribute("bitecount").as_int();
				
				Character* pCharacter = CharacterFactory::Instance()->GetCharacterWithID(index);
				if (pCharacter)
				{
					pCharacter->type = type;
					pCharacter->posX = x;
					pCharacter->posY = y;
					pCharacter->biteCount = biteCount;
				}				
			}
		}
	}
	

	
}

void	PointManager::CheckMast(std::string& word) 
{
	auto itr = m_mMastWords.find(word);
	if (itr != m_mMastWords.end())
	{
		itr->second = true;
	}
	else
	{
		m_mMastWords[word] = true;
	}
}

bool	PointManager::IsMasted(std::string& word) 
{
	auto itr = m_mMastWords.find(word);
	if (itr != m_mMastWords.end())
	{
		return itr->second;
	}
	return false;
}

void	PointManager::ResetMast() 
{
	m_currStage = 0;
	_point = 0;
	_level = 1;
	_hint = false;
	m_mMastWords.clear();
}

int		PointManager::GetMastedCount()
{
	return m_mMastWords.size();
}

int		PointManager::GetTotalCount()
{
	return m_mCardsByWord.size();
}

void	PointManager::SetLevel(int level)
{
	_level = level;
}

int		PointManager::GetLevel()
{
	return _level;
}

bool	PointManager::GetHintOption()
{
	return _hint;
}

void	PointManager::SetHintOption(bool opt)
{
	_hint = opt;
}

bool PointManager::LoadXML()
{	
	pugi::xml_document _xmlDoc;
	pugi::xml_node _xmlNode;
		
	
	std::string fullpath = FileUtils::getInstance()->fullPathForFilename("word_card_data_x.xml");
	pugi::xml_parse_result result = _xmlDoc.load_file(fullpath.c_str());
	if (!result)
	{
		assertSmartlyWithMsg(false, fullpath.c_str());
		return false;
	}	

	_xmlNode = _xmlDoc.child("root");

	

	//std::string str = _xmlNode.print()

	//OutputDebugStringA(str.c_str());

	m_mCardsByWord.clear();
	m_mCardsByLevel.clear();
	for (pugi::xml_node_iterator itr = _xmlNode.begin(); itr != _xmlNode.end(); ++itr)
	{
		pugi::xpath_node card = *itr;				

		WordCard wordCard;		
		wordCard.word = card.node().child("word").text().get();
		//wordCard.word = UTF8(wordCard.word);
		wordCard.level = atoi(card.node().child("level").text().get());		
		wordCard.text = card.node().child("text").text().get();
		//wordCard.text = UTF8(wordCard.word);

		m_mCardsByWord[wordCard.word] = wordCard;
		if (m_mCardsByLevel.end() != m_mCardsByLevel.find(wordCard.level))
		{
			m_mCardsByLevel[wordCard.level].push_back(wordCard);
		}
		else
		{
			vtWordCards wordCards;
			wordCards.push_back(wordCard);
			m_mCardsByLevel[wordCard.level] = wordCards;
		}
	}

	return true;
}

void	PointManager::GetNextScene(bool isEnter, bool isNextStage)
{
	this->SaveData();
	

	if (isNextStage)
	{
		++m_currStage;
	}

	if (isEnter)
	{
		m_currStage = 0;
	}

	
	std::string wordName;
	std::string text;
	int level = 0;
	bool isGotNextScene = false;

	for (int l = 1; l <= 5; ++l)
	{
		vtWordCards& cardListByLevel = m_mCardsByLevel[l];
		for (int i = m_currStage ; i< cardListByLevel.size(); ++i)
		{
			WordCard& wordCard = cardListByLevel[i];
			wordName = wordCard.word;
			if (IsMasted(wordName) == true)
			{
				continue;
			}

			text = wordCard.text;
			level = wordCard.level;
			if (level != l)
			{
				continue;
			}
			
			isGotNextScene = true;			
			m_currStage = i;
			break;
		}

		if (isGotNextScene) { break; }
	}

	

	if (false == isGotNextScene)
	{
		AppleTreeScene* appleScene = (AppleTreeScene*)AppleTreeScene::createScene(false);
		appleScene->initWithVal(false);
		TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2f, appleScene);
		auto director = Director::getInstance();
		director->replaceScene(sceneSlide);

		return;
	}


	//#define LITE_VER
#ifdef LITE_VER
	if (level > 1)
	{

		PRODUCT_ID pid = PID_NONE;
		if (level == 2)
		{
			pid = PID_STEP2;
		}
		else if (level == 3)
		{
			pid = PID_STEP3;
		}
		else if (level == 4)
		{
			pid = PID_STEP4;
		}
		else if (level == 5)
		{
			pid = PID_STEP5;
		}
		else
		{
			pid = PID_NONE;
		}

		if (pid > PID_NONE)
		{
			if (false == GetCartWithPID(pid))
			{
				InfoScene* infoScene = (InfoScene*)InfoScene::createScene();
				TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2f, infoScene);
				auto director = Director::getInstance();
				director->replaceScene(sceneSlide);
				return;
			}
		}
	}
#endif //LITE_VER

	if (level > _level)
	{
		SetLevel(level);
	}


	auto studyScene = StudyScene::createScene(wordName, level, text);	
	auto director = Director::getInstance();	
	if (isEnter == true)
	{
		TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2f, studyScene);
		director->replaceScene(sceneSlide);
	}
	else
	{
		TransitionPageTurn* scenePageTurn = TransitionPageTurn::create(0.2f, studyScene, false);
		director->replaceScene(scenePageTurn);
	}
	
}

void	PointManager::GetPrevSecene()
{
	// 첫 페이지에서 이전버튼이 눌려지면 홈으로 간다.
	if (m_currStage == 0)
	{
		auto mainScene = MainMenuScene::createScene();		
		TransitionSlideInR* sceneSlide = TransitionSlideInR::create(0.2f, mainScene);
		auto director = Director::getInstance();
		director->replaceScene(mainScene);
		return;
	}

	

	vtWordCards& cardListByLevel = m_mCardsByLevel[_level];
	int max_stage = cardListByLevel.size();
	if (m_currStage < max_stage)
	{
		--m_currStage;
	}
	else{ 
		m_currStage = 0; 
	}
			
	WordCard& wordCard = cardListByLevel[m_currStage];
	std::string wordName = wordCard.word;
	std::string text = wordCard.text;
	int level = wordCard.level;

	auto studyScene = StudyScene::createScene(wordName, level, text);
	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2f, studyScene);
	auto director = Director::getInstance();
	director->replaceScene(studyScene);

}


void	PointManager::SetCartWithPID(int pid, bool bBought)
{
	if (pid > PID_TOTAL || pid < 0)
	{
		return;
	}

	if (pid == PID_TOTAL && bBought == true)
	{
		m_vCart[PID_STEP2] = bBought;
		m_vCart[PID_STEP3] = bBought;
		m_vCart[PID_STEP4] = bBought;
		m_vCart[PID_STEP5] = bBought;
		m_vCart[PID_TOTAL] = bBought;
	}
	else
	{
		m_vCart[pid] = bBought;
	}
}

bool	PointManager::GetCartWithPID(int pid)
{
	if (pid > PID_TOTAL || pid < 0)
	{
		return false;
	}

	return m_vCart[pid];
}

