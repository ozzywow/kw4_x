#include "stdafx.h"
#include "StudyScene.h"
#include "SoundFactory.h"
#include "PointManager.h"
#include "CharacterFactory.h"
#include "UI_GameResultWindow.h"
#include "AppleTreeScene.h"
#include "MainMenuScene.h"
#include "TextLayer.h"
#include "wordFactory.h"
#include "TouchedHandleLayer.h"







StudyScene::StudyScene()
{
	m_timeFuncAction = NULL;
	m_lavar = NULL;
	m_isSuccessed = false;
}
StudyScene::~StudyScene()
{
	//if (m_timeFuncAction)
	//{
	//	m_timeFuncAction->release();
	//	m_timeFuncAction = NULL;
	//}

	if (m_lavar)
	{
		m_lavar->release();
		m_lavar = NULL;
	}
}

bool StudyScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	frameSize = glview->getDesignResolutionSize();
	H_OFFSET = (frameSize.height - FRAME_HEIGHT)*0.5;
	

	return true;
}

void StudyScene::initVal(std::string& worldName, int level, std::string& text)
{
	
	/////////////////////////////////////////////	
	InitRandNum();
	//arrayTextLayer = [[NSMutableArray alloc] init];
	// arrayAnswerLayer = [[NSMutableArray alloc] init];

	TextLayer* pEmptyLayer = WordFactory::Instance()->GetEmptyLayer();
	m_wordQueue.assign(4, pEmptyLayer);
	
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	Sprite* backGround = NULL;
	if (level == 5)
	{
		Sprite* background = NULL;
		if (ResolutionPolicy::FIXED_WIDTH == glview->getResolutionPolicy())
		{
			backGround = Sprite::create("UI4HD/playScene_5-hdx.png");
		}
		else
		{
			backGround = Sprite::create("UI4HD/playScene_5-hd.png");
		}
		

		// TODO : 의성어,의태어 버젼일 경우 예문 출력
		Point posOfTextLabel(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.6f)); //position of create
		int sizeOfTextFont = FRAME_WIDTH*0.08f;

		// lable of shadow
		Size sizeOfTextBox;
		sizeOfTextBox.width = FRAME_WIDTH;
		sizeOfTextBox.height = H_OFFSET + (FRAME_HEIGHT*0.3f);

		//text = [text stringByReplacingOccurrencesOfString : @"/n" withString:@"\n"];
		//[[NSString stringWithUTF8String:(char *)sqlite3_column_text(statement, 0)] stringByReplacingCharactersInRange:@"/n" withString:@"\n"];
		text = replace_all(text, "/n", "\n");				
		auto answerShadow = Label::createWithSystemFont(text, "Arial", sizeOfTextFont, sizeOfTextBox, TextHAlignment::CENTER);
		answerShadow->setPosition(posOfTextLabel);
		answerShadow->setColor(Color3B(0, 0, 0));
		this->addChild(answerShadow, kGameSceneTagAnswerShadow);
	}
	else
	{
		if (ResolutionPolicy::FIXED_WIDTH == glview->getResolutionPolicy())
		{
			backGround = Sprite::create("UI4HD/playScene-hdx.png");
		}
		else
		{
			backGround = Sprite::create("UI4HD/playScene-hd.png");
		}
		
	}


	backGround->setAnchorPoint(Point::ANCHOR_MIDDLE);
	backGround->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	this->addChild(backGround, kGameSceneTagBackground, kGameSceneTagBackground);

	m_fileName = StringUtils::format("word_card_pic_h/%s.jpg", worldName.c_str());
	m_wordName = worldName;


	int lenth = worldName.size()/3;

	m_text = text;
	m_level = level;


	MenuItemImage* prevBtnItem = MenuItemImage::create("UI4HD/preBtn_n-hd.png", "UI4HD/preBtn_s-hd.png", CC_CALLBACK_1(StudyScene::callbackOnPushedPrevBtnItem, this));
	MenuItemImage* nextBtnItem = MenuItemImage::create("UI4HD/nextBtn_n-hd.png", "UI4HD/nextBtn_s-hd.png", CC_CALLBACK_1(StudyScene::callbackOnPushedNextBtnItem, this));
	MenuItemImage* homeBtnItem = MenuItemImage::create("UI4HD/homeBtn_n-hd.png", "UI4HD/homeBtn_s-hd.png", CC_CALLBACK_1(StudyScene::callbackOnPushedHomeBtnItem, this));
	MenuItemImage* hintBtnItem = MenuItemImage::create("UI4HD/hintBtn_n-hd.png", "UI4HD/hintBtn_s-hd.png", CC_CALLBACK_1(StudyScene::callbackOnPushedHintBtnItem, this));
		
		
	const int sizeOfPadding = 5;
	Menu* mainMenu = Menu::create(prevBtnItem, nextBtnItem, homeBtnItem, hintBtnItem, NULL);
	mainMenu->alignItemsHorizontallyWithPadding(sizeOfPadding);
	const Point posOfTopMenu(FRAME_WIDTH*0.5f, H_OFFSET + (FRAME_HEIGHT*0.95f));
	mainMenu->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	mainMenu->setPosition(posOfTopMenu);

	this->addChild(mainMenu, kGameSceneTagFuncBtn, kGameSceneTagFuncBtn);

	
	// XML Data 에서 읽은 파일이름으로 그림을 띄운다.	
	Sprite* image = Sprite::create(m_fileName);
	if (image == NULL)
	{
		PointManager::Instance()->CheckMast(m_wordName);
		return ;
	}
	image->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	Point posOfImage(FRAME_WIDTH*0.5f, H_OFFSET + (FRAME_HEIGHT*0.90f));
	image->setPosition(posOfImage);
	

	this->addChild(image, kGameSceneTagImg, kGameSceneTagImg);

	Sprite* faceBtn = Sprite::create("UI4HD/boxboySmile-hd.png");
	Point posOfBox(FRAME_WIDTH*0.88f, H_OFFSET + (FRAME_HEIGHT*0.55f));
	faceBtn->setPosition(posOfBox);
	this->addChild(faceBtn, kGameSceneTagAvatar, kGameSceneTagAvatar);


	const Point posOfLevel(FRAME_WIDTH*0.82f, H_OFFSET + (FRAME_HEIGHT*0.82f));
	std::string levelLable = StringUtils::format("%d Level", level);
	levelLable = UTF8(levelLable);
	PrintStyle(this, levelLable, FRAME_WIDTH*0.07f, posOfLevel);

	this->DrowApple(false, false);

	

	// 텍스트 번튼을 만들어서 넣는다.
	auto btn = Sprite::create("UI4HD/wordBG-hd.png");
	auto btnSize = btn->getContentSize();
	int buttonSize = btnSize.width;
	int offsetWith = FRAME_WIDTH*0.14f;
	int offsetHeight = H_OFFSET + (FRAME_HEIGHT*0.09f);
	for (int line = 0; line < 2; ++line)
	{
		for (int i = 0; i < 4; i++)
		{
			std::string word = WordFactory::Instance()->RandomWord();
			Point posOfButton((buttonSize*i) + ((buttonSize*i)*0.28f) + offsetWith, (buttonSize*line) + ((buttonSize*line)*0.28f) + offsetHeight);
			TextLayer* tmpLayer = TextLayer::createWithWordText(this, posOfButton, word);
			m_arrayTextLayer.push_back(tmpLayer);
			this->addChild(tmpLayer, kGameSceneTagTextBtn, kGameSceneTagTextBtn);
		}
	}

	// 정답문자를 랜덤한 위치에 박아넣는다.
	for (int i = 0; i<lenth; ++i)
	{
		int randID = GetRandNum();
		TextLayer *tmpLayer = m_arrayTextLayer[randID];		
		std::string singleWord = m_wordName.substr(i*3, 3);
		tmpLayer->setWorldText(singleWord);
		m_arrayTextLayer.push_back(tmpLayer);
		
		m_arrayAnswerLayer.push_back(tmpLayer);
	}


	// 정답문자를 위치시킬 빈상자의 배열
	int offsetAnswerX = FRAME_WIDTH * 0.145f;
	int offsetAnswerY = H_OFFSET + (FRAME_HEIGHT* 0.28f);
	for (int i = 0; i< 4; ++i)
	{
		arrayPoint[i] = Point((buttonSize*i)+((buttonSize*i)*0.28f) + offsetAnswerX, (buttonSize + offsetAnswerY));

		// 정답칸에 테두리 친다.
		if (i < lenth)
		{
			auto wordFrame = Sprite::create("UI4HD/wordBG_Frame-hd.png");
			wordFrame->setPosition(arrayPoint[i]);
			this->addChild(wordFrame, kGameSceneTagAnswerFrame, kGameSceneTagAnswerFrame);
		}
	}



	// 터치 이벤트를 담당할 Layer를 만든 후 GameScene에 넣습니다.
	TouchedHandleLayer* touchHandlerLayer = TouchedHandleLayer::create(this);
	this->addChild(touchHandlerLayer, kGameSceneTagTouchHandlingLayer, kGameSceneTagTouchHandlingLayer);
	touchHandlerLayer->OnEnter();

	if(PointManager::Instance()->GetHintOption())
	{
		this->ShowHint();
	}


	if (0 == (rand() % 25))	
	{
		this->TimeRun(10);
	}
	

	//사운드를 출력한다.	
	auto delay = DelayTime::create(0.8f);
	auto callFunc = CallFunc::create(CC_CALLBACK_0(StudyScene::PlayWordSound, this));
	auto actions = Sequence::create(delay, callFunc, NULL);
	this->runAction(actions);

}

Point		StudyScene::getBoxPoint(int index)
{
	return arrayPoint[index];
}

void		StudyScene::OnPassed() 
{
	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_pass_effect);

	PointManager* pointManager = PointManager::Instance();
	pointManager->CheckMast(m_wordName);
	pointManager->AddPoint(1);	
	this->DrowApple(true, true);

	float invervalTime = 1.0;
	int point = pointManager->GetPoint();
	if (point > 0)
	{
		invervalTime = 1.0;
	}
	else
	{
		invervalTime = 4.0;
	}

	//////////////////////////////////////////////
	// 씬 액션함수
	auto action = [&](const std::function<void()> func)
	{
		// 아래 액션에 대한 설명..
		// 1. 시간 딜레이하고
		DelayTime* delay = DelayTime::create(invervalTime);
		// 2. 함수를 호출
		CallFunc* callFunc = CallFunc::create(func);
		// 3. 1, 2 액션을 하나의 시퀀스로 묶어서
		Sequence* squence = Sequence::create(delay, callFunc, NULL);
		// 4. 액션 호출!!
		this->runAction(squence);
	};
	//////////////////////////////////////////////

	if (m_isLavar == true && m_timeFuncAction && m_lavar)
	{
		CharacterFactory* characterFactory = CharacterFactory::Instance();
		Character* pCharacter = characterFactory->NewCharacter();
		if (pCharacter)
		{
			pCharacter->type = CT_LAVER;
		}

		//this->stopAction(m_timeFuncAction);
		//m_timeFuncAction->release();
		//m_timeFuncAction = NULL;

		m_lavar->stopAllActions();


		action(CC_CALLBACK_0(StudyScene::GoAppleTreeScene, this));

	}
	else if (point == 0)
	{
		action(CC_CALLBACK_0(StudyScene::GoAppleTreeScene, this));
	}
	else
	{
		action(CC_CALLBACK_0(StudyScene::TurnPage, this));
	}
}
void		StudyScene::OnSkip()
{
	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_skip_effect);


	this->ShowHint();


	//포인트를 초기화시킴
	PointManager* pointManager = PointManager::Instance();
	pointManager->SetPoint(0);


	// 아래 액션에 대한 설명..
	// 1. 시간 딜레이하고
	DelayTime* delay = DelayTime::create(2.0);
	// 2. 함수를 호출
	CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(StudyScene::TurnPage, this));
	// 3. 1, 2 액션을 하나의 시퀀스로 묶어서
	Sequence* squence = Sequence::create(delay, callFunc, NULL);
	// 4. 액션 호출!!
	this->runAction(squence);
	
}

void		StudyScene::TurnPage()
{
	PointManager* pointManager = PointManager::Instance();
	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_turnpage_effect);
		
	pointManager->GetNextScene(false, true);
}
void		StudyScene::GoAppleTreeScene()
{
	auto appleScene = AppleTreeScene::createScene(true);
	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.5, appleScene);
	
	Director* pDirector = Director::getInstance();
	pDirector->replaceScene(sceneSlide);	
}
void		StudyScene::OnExitStudy()
{
	auto mainScene = MainMenuScene::createScene();

	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.5, mainScene);

	Director* pDirector = Director::getInstance();
	pDirector->replaceScene(sceneSlide);	
}
void		StudyScene::ChangeEmotion(int emotionID)
{
	Sprite* faceBtn = NULL;
	this->removeChildByTag(kGameSceneTagAvatar, true);
	
	SoundFactory* sound = SoundFactory::Instance();

	int jumpCount = 2;
	switch (emotionID)
	{
	case EMOTION_ID_DEFUALT:

		faceBtn = Sprite::create("UI4HD/boxboySmile-hd.png");
		break;
	case EMOTION_ID_HAPPY:
		faceBtn = Sprite::create("UI4HD/boxboyHappy-hd.png");
		sound->play(SOUND_FILE_bulle_effect);
		break;
	case EMOTION_ID_QUEST:
		faceBtn = Sprite::create("UI4HD/boxboyQuest-hd.png");
		break;
	case EMOTION_ID_SAD:
		faceBtn = Sprite::create("UI4HD/boxboySad-hd.png");
		jumpCount = 4;
		break;
	default:
		break;
	}

	
	const Point posOfBox(FRAME_WIDTH*0.88f, H_OFFSET + (FRAME_HEIGHT*0.55f));
	faceBtn->setPosition(posOfBox);
	auto actionTo = JumpTo::create(jumpCount / 2, posOfBox, FRAME_HEIGHT*0.03f, jumpCount);
	faceBtn->runAction(actionTo);
	this->addChild(faceBtn, kGameSceneTagAvatar, kGameSceneTagAvatar);
}

void		StudyScene::DrowApple(bool showEffect, bool isRedrow)
{
	
	const Point posOfBigApple(FRAME_WIDTH*0.15f, H_OFFSET + (FRAME_HEIGHT*0.85f));
	const int sizeOfCountFont = FRAME_WIDTH*0.065f;
	

	//BIG사과를 그려준다.
	Sprite* bigapple = Sprite::create("UI4HD/appleScore-hd.png");
	bigapple->setPosition(posOfBigApple);
	const Point posOfCountFont(bigapple->getContentSize().width*0.5f, bigapple->getContentSize().height*0.22f);
		
	int totalyAppleCount = CharacterFactory::Instance()->GetCountWithType(CT_APPLE);
	std::string countOfBig = StringUtils::format("X%d", totalyAppleCount);

	Label* specialPoint = Label::createWithSystemFont(countOfBig, "Arial", sizeOfCountFont);
	specialPoint->setColor(Color3B(255, 255, 255));
	specialPoint->setAnchorPoint(Point(0.0, 0.0));
	specialPoint->setPosition(posOfCountFont);	
	bigapple->addChild(specialPoint);
	this->addChild(bigapple, kGameSceneTagAppleSpecial, kGameSceneTagAppleSpecial);

	//사과를 그려준다.
	float appleOffset = FRAME_WIDTH*0.1f;
	int point = PointManager::Instance()->GetPoint();
	if (point > 0)
	{
		const int yPos = H_OFFSET + (FRAME_HEIGHT*0.5f);
		for (int i = 0; i < point; ++i)
		{
			Sprite* apple = Sprite::create("UI4HD/applePart-hd.png");
			float spaceWith = apple->getContentSize().width;
			apple->setPosition(Point(appleOffset, yPos));
			appleOffset += spaceWith;

			this->addChild(apple, kGameSceneTagApplePoint, kGameSceneTagApplePoint + i);
			

			if (isRedrow == true && i == point - 1)
			{
				auto actionTo = JumpTo::create(0.3f, Point(apple->getPosition().x, apple->getPosition().y), 5, 1);
				apple->runAction(actionTo);
			}
		}
	}
	else
	{
	}
}
void		StudyScene::DrowStar() 
{

}
void		StudyScene::ShowHint()
{
	const int sizeOfFont = FRAME_WIDTH*0.12f;
	int lenth = m_wordName.size()/3;
	for (int i = 0; i< lenth; ++i)
	{		
		std::string singleWord = m_wordName.substr(i * 3, 3);
		Label* singleWordLayer = Label::createWithSystemFont(singleWord, "Arial", sizeOfFont);
		singleWordLayer->setColor(Color3B(120, 120, 120));
		singleWordLayer->setPosition(arrayPoint[i]);
		this->addChild(singleWordLayer, kGameSecceTagHintLayer + i, kGameSecceTagHintLayer + i);
	}
}
void		StudyScene::PlayWordSound()
{
	std::string path = StringUtils::format("word_card_sound/%s.mp3", m_wordName.c_str());	
	SoundFactory::Instance()->play(const_cast<char*>(path.c_str()) );
}
void		StudyScene::TimeRun(int sec)
{
	m_lavar = Sprite::create("UI4HD/lavar01-hd.png");

	auto animationLavar = Animation::create();
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar01-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar02-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar03-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar04-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar05-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/lavar06-hd.png");
	animationLavar->setDelayPerUnit(0.2f); 

	Animate *animate = Animate::create(animationLavar);
	RepeatForever *repeat = RepeatForever::create(animate);
	m_lavar->runAction(repeat);

	const Point posOfBeggin(Point(FRAME_WIDTH*0.0f, H_OFFSET+(FRAME_HEIGHT*0.5f)));
	const Point posOfEnd(Point(FRAME_WIDTH*1.0f, H_OFFSET+(FRAME_HEIGHT*0.5f)));

	m_lavar->setPosition(posOfBeggin);
	Point destPos = posOfEnd;

	MoveTo* moveToAction = MoveTo::create((float)sec, destPos);
	m_lavar->runAction(moveToAction);
	this->addChild(m_lavar, kGameSceneTagWatch, kGameSceneTagWatch);

	
	DelayTime* delay = DelayTime::create((float)sec);
	CallFunc* callFunc = CallFunc::create(CC_CALLBACK_0(StudyScene::RemoveLavar, this));
		
	m_timeFuncAction = Sequence::create(delay, callFunc, NULL);
	this->runAction(m_timeFuncAction);

	// Sound
	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_dingling_effect);

	m_isLavar = true;
}
void		StudyScene::RemoveLavar()
{
	this->removeChildByTag(kGameSceneTagWatch, true);
	m_isLavar = false;
}

bool		StudyScene::checkWord()
{
	int max_size = m_wordQueue.size();
	std::string tempStr;	
	for (int i = 0; i < max_size; ++i)
	{
		TextLayer* pTextLayer = m_wordQueue[i];
		if(WordFactory::Instance()->GetEmptyLayer() == pTextLayer)
		{
			continue;
		}
		tempStr.append(pTextLayer->m_textStr);
	}

	if (tempStr == m_wordName)
	{
		return true;
	}
	return false;
}


int			StudyScene::GetEmptyBoxID() 
{
	for (int i = 0; i < 4; ++i)
	{
		TextLayer* pCurrPlacedLayer = m_wordQueue[i];
		if (WordFactory::Instance()->GetEmptyLayer() == pCurrPlacedLayer)
		{
			return i;
		}
	}
	return -1;
}




void StudyScene::callbackOnPushedPrevBtnItem(Ref* sender)
{
	if (m_isSuccessed == true) return;

	PointManager::Instance()->GetPrevSecene();
	m_isSuccessed = true;
}

void StudyScene::callbackOnPushedNextBtnItem(Ref* sender)
{
	if (m_isSuccessed == true) return;

	int sizeOfArray = m_arrayAnswerLayer.size();
	for (int i = 0; i<sizeOfArray; ++i)
	{
		TextLayer* pAnswerLayer = m_arrayAnswerLayer[i];
		auto actionTo = JumpTo::create(2.0f, Point(0, 0), FRAME_HEIGHT*0.03f, 4);
		pAnswerLayer->runAction(actionTo);
	}

	this->ChangeEmotion(EMOTION_ID_SAD);
	this->OnSkip();

	m_isSuccessed = true;
}

void StudyScene::callbackOnPushedHomeBtnItem(Ref* sender)
{
	if (m_isSuccessed == true) return;

	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_goodjob_voice);
	this->OnExitStudy();
	m_isSuccessed = true;
}

void StudyScene::callbackOnPushedHintBtnItem(Ref* sender)
{
	if (m_isSuccessed == true) return;

	this->ChangeEmotion(EMOTION_ID_QUEST);

	int sizeOfArray = m_arrayAnswerLayer.size();
	for (int i = 0; i<sizeOfArray; ++i)
	{
		TextLayer* pAnswerLayer = m_arrayAnswerLayer[i];

		auto actionTo = JumpTo::create(1.0f, Point::ZERO, FRAME_HEIGHT*0.03f, 2);
		pAnswerLayer->runAction(actionTo);

		SoundFactory* soundFactory = SoundFactory::Instance();
		soundFactory->play(SOUND_FILE_dingling_effect);
	}

	// 포인트 까기
	int currPoint = PointManager::Instance()->DelPoint(m_level);
	// 사과 지우기
	this->removeChildByTag(kGameSceneTagApplePoint + currPoint, true);
}


void StudyScene::callbackOnPushedBuyMenuItem(Ref* sender)
{
	//NSURL *appStoreUrl = [NSURL URLWithString : @"http://itunes.apple.com/kr/app/id363678361?mt=8"];
//		[[UIApplication sharedApplication] openURL:appStoreUrl];
}



void		StudyScene::popCallback_Ok(Ref* pSender)
{
	UIPopupWindow *pPopup = (UIPopupWindow *)pSender; //현재 팝업에 대한 클래스로 캐스팅 

	// 여기에서 콜백 받을때 어떤 버튼이 클릭됐는지 알수있으면 좋겠죠?												  
	int nTag = pPopup->getResult();
	//혹은 콜백을 다르게 선업하셔도 됩니다. 그건 여러분 몫으로 콜백2 있으니 참고해서 만드심 됍니다
	if (nTag == 1)
	{
		//닫기 버튼 이다~~
	}
	pPopup->closePopup(); //팝업을 닫습니다. !! 팝업을 닫을시 필히 호출해주세요 이거 안해주면 팝업창 안사라집니다.  
}