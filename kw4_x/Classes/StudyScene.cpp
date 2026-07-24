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


// 정답 글자를 8개 버튼 중 겹치지 않는 위치에 랜덤 배치하기 위한 상태.
// 유일 사용처가 이 파일뿐이라 헤더(common_define.h) 대신 여기 파일 지역으로 둔다.
static bool arrRandFlag[8];

static void InitRandNum()
{
	memset(arrRandFlag, 0, sizeof(arrRandFlag));
}

static int GetRandNum(int max = 8)
{
	// 우선 랜덤으로 빈 슬롯을 찾는다.
	for (int i = 0; i< 100; ++i) {
		int ran = rand() % max;
		if (arrRandFlag[ran] == false)
		{
			arrRandFlag[ran] = true;
			return ran;
		}
	}
	// 랜덤 시도가 모두 실패해도(빈 슬롯이 거의 남지 않았을 때) 선형 탐색으로 확실히 찾는다.
	// 이 보정이 없으면 -1 이 반환되어 호출부에서 배열 범위 밖 접근(크래시) 위험이 있다.
	for (int i = 0; i < max; ++i) {
		if (arrRandFlag[i] == false)
		{
			arrRandFlag[i] = true;
			return i;
		}
	}
	return -1; // 모든 슬롯이 사용됨(요청이 슬롯 수를 초과한 경우)
}


StudyScene::StudyScene()
{
	m_timeFuncAction = NULL;
	m_lavar = NULL;
	m_isSuccessed = false;
}
StudyScene::~StudyScene()
{
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
	H_OFFSET = CalcHOffset(frameSize.height);
	

	return true;
}

void StudyScene::initVal(std::string& worldName, int level, std::string& text)
{
	CCLOG("worldName:%s, level:%d, text:%s", worldName.c_str(), level, text.c_str());

	InitRandNum();

	
	for (auto* layer : m_wordQueue) { layer->release(); }
	m_wordQueue.clear();
	for(int i = 0; i < 4; ++i)
	{
		TextLayer* pEmptyLayer = WordFactory::Instance()->CreateEmptyLayer();
		pEmptyLayer->retain();
		m_wordQueue.push_back(pEmptyLayer);
	}		
	
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	// 짧은 디바이스(iPad/데스크탑)는 -hd.png(960px), 긴 디바이스는 -hdx.png(1136px) 사용.
	// 이후 design space(frameSize)에 맞게 스케일하여 상하 잘림 방지.
	// 화면 비율(height/width)로 판단: 1.6 초과 시 긴 디바이스 (iPhone 5 이상 ~1.775)
	// iPad mini 6 등 비율 ~1.52 기기는 false로 처리됨.
	bool useHdx = (frameSize.height / frameSize.width > 1.6f);
	Sprite* backGround = NULL;
	if (level == 5)
	{
		backGround = Sprite::create(useHdx ? "UI4HD/playScene_5-hdx.png" : "UI4HD/playScene_5-hd.png");

		Point posOfTextLabel(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.6f));
		int sizeOfTextFont = FRAME_WIDTH*0.08f;
		text = replace_all(text, "/n", "\n");
		PrintStyle(this, text, sizeOfTextFont, posOfTextLabel);
	}
	else
	{
		backGround = Sprite::create(useHdx ? "UI4HD/playScene-hdx.png" : "UI4HD/playScene-hd.png");
	}

	backGround->setAnchorPoint(Point::ANCHOR_MIDDLE);
	backGround->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	float bgScale;
	{
		float bgScaleX = frameSize.width  / backGround->getContentSize().width;
		float bgScaleY = frameSize.height / backGround->getContentSize().height;
		bgScale = std::max(bgScaleX, bgScaleY);
		backGround->setScale(bgScale);
	}
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
	const Point posOfTopMenu(FRAME_WIDTH*0.5f, H_OFFSET + (ACTIVE_HEIGHT*0.95f));
	mainMenu->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	mainMenu->setPosition(posOfTopMenu);

	this->addChild(mainMenu, kGameSceneTagFuncBtn, kGameSceneTagFuncBtn);

	Sprite* image = Sprite::create(m_fileName);
	if (image == NULL)
	{
		CCLOG("can not found image file (%s)", m_fileName.c_str());
		PointManager::Instance()->CheckMast(m_wordName);
		return ;
	}
	image->setAnchorPoint(Point::ANCHOR_MIDDLE_TOP);
	Point posOfImage(FRAME_WIDTH*0.5f, H_OFFSET + (ACTIVE_HEIGHT*0.90f));
	image->setPosition(posOfImage);

	// 배경 프레임이 이미지보다 위에 그려지며(z: Background=1 > Img=0), 프레임에는 그림이
	// 비쳐 보이는 투명 창이 있다. 창은 원본 PNG(640px 기준) 폭 583 / 높이 375이고,
	// 배경과 함께 bgScale로 확대된다. 그림이 창보다 작으면 그림 아래(클리어컬러=검정)가
	// 양옆에 비치므로, 창을 완전히 덮도록 스케일(cover)하고 넘치는 부분은 프레임이 가린다.
	{
		const float WINDOW_W = 583.0f;
		const float WINDOW_H = 375.0f;
		float imgW = image->getContentSize().width;
		float imgH = image->getContentSize().height;
		if (imgH > 0 && imgW > 0)
		{
			float scale = std::max((WINDOW_W * bgScale) / imgW, (WINDOW_H * bgScale) / imgH);
			image->setScale(scale);
		}
	}

	this->addChild(image, kGameSceneTagImg, kGameSceneTagImg);

	Sprite* faceBtn = Sprite::create("UI4HD/boxboySmile-hd.png");
	Point posOfBox(FRAME_WIDTH*0.88f, H_OFFSET + (FRAME_HEIGHT*0.55f));
	faceBtn->setPosition(posOfBox);
	this->addChild(faceBtn, kGameSceneTagAvatar, kGameSceneTagAvatar);


	// 카드 우상단 레벨 표시: "n 단계". 프레임 상단에 가깝게 위로 올리고 폰트는 작게.
	// "단계"는 UTF-8 바이트(\xEB\x8B\xA8\xEA\xB3\x84)로 직접 지정한다. 소스 인코딩/컴파일러
	// charset에 의존하지 않도록 UTF8() 매크로를 거치지 않으며, 한글 렌더를 위해 KR_FONT_TTF 사용.
	const Point posOfLevel(FRAME_WIDTH*0.82f, H_OFFSET + (ACTIVE_HEIGHT*0.87f));
	std::string levelLable = StringUtils::format("%d \xEB\x8B\xA8\xEA\xB3\x84", level); // "%d 단계"
	PrintStyle(this, levelLable, FRAME_WIDTH*0.055f, posOfLevel, KR_FONT_TTF);

	this->DrowApple(false, false);

	

	// 텍스트 버튼을 화면에 뿌린다.
	auto btn = Sprite::create("UI4HD/wordBG-hd.png");
	auto btnSize = btn->getContentSize();
	int buttonSize = btnSize.width;  // X 간격은 고정 (가로 640 기준)
	// Y 간격은 ACTIVE_HEIGHT에 비례 스케일하여 해상도별 균일한 레이아웃 유지
	float heightRatio = ACTIVE_HEIGHT / FRAME_HEIGHT;
	float btnSizeY = buttonSize * heightRatio;
	int offsetWith = FRAME_WIDTH*0.14f;
	int offsetHeight = H_OFFSET + (int)(ACTIVE_HEIGHT*0.09f);
	for (int line = 0; line < 2; ++line)
	{
		for (int i = 0; i < 4; i++)
		{
			std::string word = WordFactory::Instance()->RandomWord();
			Point posOfButton((buttonSize*i) + ((buttonSize*i)*0.28f) + offsetWith,
							  (btnSizeY*line) + ((btnSizeY*line)*0.28f) + offsetHeight);
			TextLayer* tmpLayer = TextLayer::createWithWordText(this, posOfButton, word);
			m_arrayTextLayer.push_back(tmpLayer);
			this->addChild(tmpLayer, kGameSceneTagTextBtn, kGameSceneTagTextBtn);
		}
	}

	// 정답문자를 선택한 위치에 꺼내 놓는다.
	for (int i = 0; i<lenth; ++i)
	{
		int randID = GetRandNum();
		// 빈 버튼이 없으면(정답 글자 수가 버튼 수를 초과 등) 더 배치하지 않는다.
		// GetRandNum 이 -1 을 돌려줄 때 배열 범위 밖 접근으로 크래시하던 것을 방어한다.
		if (randID < 0 || randID >= (int)m_arrayTextLayer.size())
		{
			break;
		}
		TextLayer *tmpLayer = m_arrayTextLayer[randID];
		std::string singleWord = m_wordName.substr(i*3, 3);
		tmpLayer->setWorldText(singleWord);
		m_arrayTextLayer.push_back(tmpLayer);

		m_arrayAnswerLayer.push_back(tmpLayer);
	}


	// 정답문자를 위치시킬 빈자리 배열
	// wordBG_Frame이 wordBG보다 크므로 왼쪽 가장자리가 text button과 맞도록 center offset을 보정
	auto wordFrameForSize = Sprite::create("UI4HD/wordBG_Frame-hd.png");
	float frameSpriteW = wordFrameForSize->getContentSize().width;
	float frameSpriteH = wordFrameForSize->getContentSize().height;
	// X: wordBG_Frame 왼쪽 가장자리가 text button 왼쪽 가장자리와 맞도록 center offset 보정
	int offsetAnswerX = offsetWith + (int)((frameSpriteW - buttonSize) * 0.5f);
	// Y: wordBG_Frame 하단 가장자리가 text button 하단 가장자리와 맞도록 center offset 보정
	int offsetAnswerY = H_OFFSET + (int)(ACTIVE_HEIGHT * 0.28f) + (int)((frameSpriteH - buttonSize) * 0.5f);
	for (int i = 0; i< 4; ++i)
	{
		arrayPoint[i] = Point((buttonSize*i)+((buttonSize*i)*0.28f) + offsetAnswerX, (int)(btnSizeY + offsetAnswerY));

		// 빈칸에 테두리를 친다.
		if (i < lenth)
		{
			auto wordFrame = Sprite::create("UI4HD/wordBG_Frame-hd.png");
			wordFrame->setPosition(arrayPoint[i]);
			this->addChild(wordFrame, kGameSceneTagAnswerFrame, kGameSceneTagAnswerFrame);
		}
	}



	// 터치 이벤트를 처리할 Layer를 만든 후 GameScene에 붙인다.
	TouchedHandleLayer* touchHandlerLayer = TouchedHandleLayer::create(this);
	this->addChild(touchHandlerLayer, kGameSceneTagTouchHandlingLayer, kGameSceneTagTouchHandlingLayer);
	touchHandlerLayer->OnEnter();

	if(PointManager::Instance()->GetHintOption())
	{
		this->ShowHint();
	}

	if(CharacterFactory::Instance()->GetCountOfLaver() < CharacterFactory::s_maxLaverCount)
	{
		if (0 == (rand() % 30))	
		{
			this->TimeRun(10);
		}
	}
	
	

	if (this->m_level < 5)
	{
		//소리를 재생한다.
		auto delay = DelayTime::create(0.5);
		auto callFunc = CallFunc::create(CC_CALLBACK_0(StudyScene::PlayWordSound, this));
		auto actions = Sequence::create(delay, callFunc, NULL);
		this->runAction(actions);
	}

#ifdef TEST_MODE
	//testcode
	OnPassed();
#endif //TEST_MODE

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

	int point = pointManager->GetPoint();
	float invervalTime = (point > 0) ? 1.0f : 4.0f;

#ifdef TEST_MODE
	invervalTime = 0.5;
#endif //TEST_MODE

	auto action = [&](const std::function<void()> func)
	{
		auto seq = Sequence::create(DelayTime::create(invervalTime), CallFunc::create(func), NULL);
		this->runAction(seq);
	};

	if (m_isLavar == true && m_timeFuncAction && m_lavar)
	{
		CharacterFactory* characterFactory = CharacterFactory::Instance();
		Character* pCharacter = characterFactory->NewCharacter();
		if (pCharacter)
		{
			pCharacter->type = CT_LAVER;
		}

		m_lavar->stopAllActions();

#ifdef TEST_MODE
		action(CC_CALLBACK_0(StudyScene::TurnPage, this));
#else
		action(CC_CALLBACK_0(StudyScene::GoAppleTreeScene, this));
#endif
	}
	else if (point == 0)
	{
#ifdef TEST_MODE
		action(CC_CALLBACK_0(StudyScene::TurnPage, this));
#else
		action(CC_CALLBACK_0(StudyScene::GoAppleTreeScene, this));
#endif
	}
	else
	{
		action(CC_CALLBACK_0(StudyScene::TurnPage, this));
	}
}
void		StudyScene::OnSkip()
{
	SoundFactory::Instance()->play(SOUND_FILE_skip_effect);
	this->ShowHint();

	PointManager::Instance()->SetPoint(0);

	auto seq = Sequence::create(DelayTime::create(2.0f), CallFunc::create(CC_CALLBACK_0(StudyScene::TurnPage, this)), NULL);
	this->runAction(seq);
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
	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(SCENE_TRANSITION_TIME, appleScene);
	
	Director* pDirector = Director::getInstance();
	pDirector->replaceScene(sceneSlide);	
}
void		StudyScene::OnExitStudy()
{
	auto mainScene = MainMenuScene::createScene();

	// 홈으로 나가기(back) → 뒤로 가는 느낌의 SlideInR
	TransitionSlideInR* sceneSlide = TransitionSlideInR::create(SCENE_TRANSITION_TIME, mainScene);

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

	// 알 수 없는 emotionID(default) 이거나 스프라이트 로드 실패 시 faceBtn 이 NULL 이다.
	// 아래에서 바로 역참조하므로 여기서 막지 않으면 크래시한다. 아바타 갱신만 건너뛴다.
	if (faceBtn == nullptr)
	{
		return;
	}

	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	const Point posOfBox(FRAME_WIDTH*0.88f, H_OFFSET + (ACTIVE_HEIGHT*0.55f));
	faceBtn->setPosition(posOfBox);
	auto actionTo = JumpTo::create(jumpCount / 2, posOfBox, ACTIVE_HEIGHT*0.03f, jumpCount);
	faceBtn->runAction(actionTo);
	this->addChild(faceBtn, kGameSceneTagAvatar, kGameSceneTagAvatar);
}

void		StudyScene::DrowApple(bool showEffect, bool isRedrow)
{
	
	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	const Point posOfBigApple(FRAME_WIDTH*0.15f, H_OFFSET + (ACTIVE_HEIGHT*0.85f));
	const int sizeOfCountFont = FRAME_WIDTH*0.065f;
	

	//BIG애플을 그려준다.
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

	//애플을 그려준다.
	float appleOffset = FRAME_WIDTH*0.1f;
	int point = PointManager::Instance()->GetPoint();
	if (point > 0)
	{
		const int yPos = H_OFFSET + (ACTIVE_HEIGHT*0.5f);
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
		Label* singleWordLayer = Label::createWithTTF(singleWord, KR_FONT_TTF, sizeOfFont);
		// 힌트는 어두운 정답칸 위에 얹힌다. 기존 회색(120)은 유아에게 대비가 약해 흐릿했다.
		// 밝은 톤 + 약한 투명도로 "임시 힌트" 느낌은 유지하되 또렷하게 보이도록 한다.
		singleWordLayer->setColor(Color3B(240, 240, 240));
		singleWordLayer->setOpacity(180);
		singleWordLayer->setPosition(arrayPoint[i]);
		this->addChild(singleWordLayer, kGameSecceTagHintLayer + i, kGameSecceTagHintLayer + i);
	}
}
void		StudyScene::PlayWordSound()
{
	std::string path = StringUtils::format("word_card_sound/%s.mp3", m_wordName.c_str());	
	SoundFactory::Instance()->play(path.c_str());
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

	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	const Point posOfBeggin(Point(FRAME_WIDTH*0.0f, H_OFFSET+(ACTIVE_HEIGHT*0.5f)));
	const Point posOfEnd(Point(FRAME_WIDTH*1.0f, H_OFFSET+(ACTIVE_HEIGHT*0.5f)));

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
		if(pTextLayer->isEmpty())
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
		if (pCurrPlacedLayer->isEmpty())
		{
			return i;
		}
	}
	return -1;
}




void StudyScene::callbackOnPushedPrevBtnItem(Ref* sender)
{
	if (m_isSuccessed == true) return;

	SoundFactory::Instance()->play(SOUND_FILE_click_effect);
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
		auto actionTo = JumpTo::create(2.0f, Point(0, 0), CalcActiveHeight(frameSize.height)*0.03f, 4);
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

		auto actionTo = JumpTo::create(1.0f, Point::ZERO, CalcActiveHeight(frameSize.height)*0.03f, 2);
		pAnswerLayer->runAction(actionTo);

		SoundFactory* soundFactory = SoundFactory::Instance();
		soundFactory->play(SOUND_FILE_dingling_effect);
	}

	// 포인트 감소
	int currPoint = PointManager::Instance()->DelPoint(m_level);
	// 화면에서 삭제
	this->removeChildByTag(kGameSceneTagApplePoint + currPoint, true);
}


void		StudyScene::popCallback_Ok(Ref* pSender)
{
	UIPopupWindow *pPopup = (UIPopupWindow *)pSender;
	pPopup->closePopup();
}