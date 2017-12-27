#pragma execution_character_set("utf-8")

#include "stdafx.h"
#include "PointManager.h"
#include "SoundFactory.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "pugixml.hpp"
#include "AppleTreeScene.h"
#include "StudyScene.h"
#include "MainMenuScene.h"

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

	pugi::xml_document _xmlDocForMastWords;
	for (auto itr = m_mMastWords.begin(); itr != m_mMastWords.end(); ++itr)
	{		
		pugi::xml_attribute attr = _xmlDocForMastWords.append_attribute(itr->first.c_str());
		attr.set_value(itr->second);		
	}
	
	cocos2d::Data data;
	//data.copy(_xmlDocForMastWords.value()
	UserDefault::getInstance()->setDataForKey("mast_words", data);

}

void	PointManager::LoadData() 
{
	m_currStage = UserDefault::getInstance()->getIntegerForKey("curr_stage");
	_point = UserDefault::getInstance()->getIntegerForKey("point");
	_level = UserDefault::getInstance()->getIntegerForKey("level");
	if (_level == 0) _level = 1;
	_hint = UserDefault::getInstance()->getBoolForKey("hint");

	cocos2d::Data data = UserDefault::getInstance()->getDataForKey("mast_words");	
	pugi::xml_document _xmlDocForMastWords;
	_xmlDocForMastWords.load_buffer(data.getBytes(), data.getSize());
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

void	PointManager::GetNextScene(bool isEnter)
{
	// 처음 진입이면 첫 패이지 부터 보여준다.
	if (isEnter == true)
	{
		m_currStage = 0;
	}
	else
	{
		++m_currStage;
	}

//#define LITE_VER
#ifdef LITE_VER
	if (_level > 1)
	{

		PRODUCT_ID pid = PID_NONE;
		if (_level == 2)
		{
			pid = PID_STEP2;
		}
		else if (_level == 3)
		{
			pid = PID_STEP3;
		}
		else if (_level == 4)
		{
			pid = PID_STEP4;
		}
		else if (_level == 5)
		{
			pid = PID_STEP5;
		}
		else
		{
			pid = PID_NONE;
		}

		if (pid > PID_NONE)
		{
			if( false == GetCartWithPID(pid) )
			{
				//BuyScene* buyScene = [[BuyScene node] init];
				//[[CCDirector sharedDirector] replaceScene:[CCTransitionSlideInR transitionWithDuration : 0.2 scene : buyScene]];
				//return;
			}
		}
	}
#endif //LITE_VER

	int max_stage = m_mCardsByWord.size();


	std::string wordName;
	std::string text;
	int level = 0;
	bool isGotNextScene = false;
	vtWordCards& cardListByLevel = m_mCardsByLevel[_level];
	if (cardListByLevel.size() > m_currStage)
	{
		for (int i = m_currStage; i< cardListByLevel.size(); ++i)
		{
			WordCard& wordCard = cardListByLevel[i];
			wordName = wordCard.word;
			if (IsMasted(wordName) == true)
			{
				continue;
			}

			text = wordCard.text;
			level = wordCard.level;
			if (level != _level)
			{
				continue;
			}

			isGotNextScene = true;
			break;
		}
	}
	

	if (false == isGotNextScene)
	{
		// 해당 레벨을 모두 완료했으니 레벨을 올려준다.
		if (_level <= 5)
		{
			++_level;
			m_currStage = 0;

			if (_level > 1)
			{
				//[self GetNextScene:NO] ;
				//NSString* message = [NSString stringWithFormat : @"%d 단계를 완료했어요.", _level-1] ;
				//ConfirmScene* confirmScene = [[ConfirmScene node] initWithString:message];
				//[[CCDirector sharedDirector] replaceScene:[CCTransitionSlideInR transitionWithDuration : 0.2 scene : confirmScene]];	//슬라이드

			}


			return;

		}
		else
		{
			// 여기서는 완료페이지를 보여주자..
			auto appleScene = AppleTreeScene::createScene(false);
			TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2f, appleScene);
			auto director = Director::getInstance();
			director->replaceScene(sceneSlide);			

			return;

		}
	}


	auto studyScene = StudyScene::createScene(wordName, level, text);
	if (NULL == studyScene)
	{
		GetNextScene(isEnter);		
		return;
	}

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
	}

	vtWordCards& cardListByLevel = m_mCardsByLevel[_level];
	int max_stage = cardListByLevel.size();


	std::string wordName;
	std::string text;
	int level = 0;
	for (; m_currStage<max_stage; --m_currStage)
	{
		// 첫 페이지에서 이전버튼이 눌려지면 홈으로 간다.
		if (m_currStage < 0)
		{
			m_currStage = 0;
			auto mainScene = MainMenuScene::createScene();
			TransitionSlideInR* sceneSlide = TransitionSlideInR::create(0.2f, mainScene);
			auto director = Director::getInstance();
			director->replaceScene(mainScene);
			return;
		}

		WordCard& wordCard = cardListByLevel[m_currStage];


		wordName = wordCard.word;		
		text = wordCard.text;
		level = wordCard.level;
		break;
	}


	auto studyScene = StudyScene::createScene(wordName, level, text);
	if (studyScene == studyScene)
	{
		return;
	}

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

void	PointManager::ToggleIndicator(bool lock)
{
	/*
	if (nil == m_activity)
	{
		m_activity
			= [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(320 / 2, 480 / 2, 40.0, 40.0)];

		[m_rootView addSubview : m_activity];
	}

	if (false == Lock)
		[m_activity stopAnimating];
	else
		[m_activity startAnimating];

	return;
	*/
}
