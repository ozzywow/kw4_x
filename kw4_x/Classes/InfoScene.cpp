#include "stdafx.h"

#include "InfoScene.h"
#include "PointManager.h"
#include "SoundFactory.h"
#include "MainMenuScene.h"
#include "UI_GameResultWindow.h"




InfoScene::InfoScene()
{

}

InfoScene::~InfoScene()
{
	//this->removeAllChildrenWithCleanup(true);
}

bool InfoScene::init()
{
	if (!Scene::init())
	{
		return false;
	}

	Sprite* background = Sprite::create("UI4HD/info_bg-hd.png") ;

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	auto frameSize = glview->getDesignResolutionSize();
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);

	this->addChild(background, 0, 0);

	this->DrawItemBox();

	return true;
}



void InfoScene::DrawItemBox()
{
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();

	int level = PointManager::Instance()->GetLevel();

	const int		sizeOfFont = frameSize.width*0.07f;
	const int       sizeofFont_s = frameSize.width*0.04f;
	const Point	posOfMainMenu = Point(frameSize.width*0.2f, frameSize.height*0.9f);

	const Point	posOfLable_level = Point(frameSize.width*0.5f, frameSize.height*0.85f);
	const Point	posOfSelectMenu1 = Point(frameSize.width*0.5f, frameSize.height*0.72f);
	const Point	posOfSelectMenu2 = Point(frameSize.width*0.5f, frameSize.height*0.49f);

	const Point	posOfLable_hint = Point(frameSize.width*0.5f, frameSize.height*0.32f);

	const Point	posOfOptonMenu = Point(frameSize.width*0.5f, frameSize.height*0.2f);
	const int       blinkTime = 80000;

	Sprite* btn = Sprite::create("UI4HD/btn_level_1_n-hd.png");
	int nWidth = btn->getContentSize().width;
	int nHeight = btn->getContentSize().height;
	const Point	posOfDesc = Point(nWidth*0.5f, nHeight*-0.1f);

	std::string strLevel("Select Step");
	strLevel = UTF8(strLevel);
	std::string strStep("Setting");
	strStep = UTF8(strStep);
	PrintStyle(this, strLevel, sizeOfFont, posOfLable_level);
	PrintStyle(this, strStep, sizeOfFont, posOfLable_hint);


//#define LITE_VER 
#ifdef LITE_VER


	m_btnLevel1 = MenuItemImage::create("btn_level_1_n-hd.png", "btn_level_1_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel1, this));	
	PrintStyle(m_btnLevel1, "1 Step", sizeofFont_s, posOfDesc);

	if (false == PointManager::Instance()->GetCartWithPID(PID_TOTAL))
	{

		m_btnLevel6 = MenuItemImage::create("btn_level_total_n-hd.png", "btn_level_total_s-hd.png", CC_CALLBACK_1(InfoScene::cfTotal, this));
		PrintStyle(m_btnLevel6, "Whole steps($2.99)", sizeofFont_s, posOfDesc);

		Sprite* lock = Sprite::create("lock_icon-hd.png");
		lock->setOpacity(64);
		lock->setPosition(nWidth / 2, nHeight / 2);
		m_btnLevel6->addChild(lock);
		auto action = Blink::create(blinkTime, 100000);
		lock->runAction(action);


		//step2
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP2))
		{
			m_btnLevel2 = MenuItemImage::create("btn_level_2_n-hd.png", "btn_level_2_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep2, this));
			PrintStyle(m_btnLevel2, "2 Step($9.99)", sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("lock_icon-hd.png");
			lock->setOpacity(64);
			lock->setPosition(nWidth / 2, nHeight / 2);
			m_btnLevel2->addChild(lock);

			if (level == 2)
			{
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}

		}
		else
		{
			m_btnLevel2 = MenuItemImage::create("btn_level_2_n-hd.png", "btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
			PrintStyle(m_btnLevel2, "2 Step", sizeofFont_s, posOfDesc);
		}



		//step3
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP3))
		{
			m_btnLevel3 = MenuItemImage::create("btn_level_3_n-hd.png", "btn_level_3_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep3, this))
				PrintStyle(m_btnLevel3, "3 Step($0.99)", sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("lock_icon-hd.png");
			lock->setOpacity(64);
			lock->setPosition(nWidth / 2, nHeight / 2);
			m_btnLevel3->addChild(lock);

			if (level == 3)
			{
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}

		}
		else
		{
			m_btnLevel3 = MenuItemImage::create("btn_level_3_n-hd.png", "btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
			PrintStyle(m_btnLevel3, "3 Step", sizeofFont_s, posOfDesc);
		}



		//step4
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP4))
		{
			m_btnLevel4 = MenuItemImage::create("btn_level_4_n-hd.png", "btn_level_4_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep4, this))
				PrintStyle(m_btnLevel4, "4 Step($0.99)", sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("lock_icon-hd.png");
			lock->setOpacity(64);
			lock->setPosition(nWidth / 2, nHeight / 2);
			m_btnLevel4->addChild(lock);

			if (level == 4)
			{
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}

		}
		else
		{
			m_btnLevel4 = MenuItemImage::create("btn_level_4_n-hd.png", "btn_level_4_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this))
				PrintStyle(m_btnLevel4, "4 Step", sizeofFont_s, posOfDesc);
		}


		//step5
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP5))
		{
			m_btnLevel5 = MenuItemImage::create("btn_level_5_n-hd.png", "btn_level_5_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep5, this))
				PrintStyle(m_btnLevel5, "5 Step($0.99)", sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("lock_icon-hd.png");
			lock->setOpacity(64);
			lock->setPosition(nWidth / 2, nHeight / 2);
			m_btnLevel5->addChild(lock);

			if (level == 5)
			{
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}

		}
		else
		{
			m_btnLevel4 = MenuItemImage::create("btn_level_5_n-hd.png", "btn_level_5_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
			PrintStyle(m_btnLevel4, "5 Step", sizeofFont_s, posOfDesc);
		}
	}

	else
	{
		m_btnLevel2 = MenuItemImage::create("btn_level_2_n-hd.png", "btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
		m_btnLevel3 = MenuItemImage::create("btn_level_3_n-hd.png", "btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
		m_btnLevel4 = MenuItemImage::create("btn_level_4_n-hd.png", "btn_level_4_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this));
		m_btnLevel5 = MenuItemImage::create("btn_level_5_n-hd.png", "btn_level_5_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
		m_btnLevel6 = MenuItemImage::create("btn_level_6_n-hd.png", "btn_level_6_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel6, this));

		PrintStyle(m_btnLevel1, "1 Step", sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel2, "2 Step", sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel3, "3 Step", sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel4, "4 Step", sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel5, "5 Step", sizeofFont_s, posOfDesc);
	}

#else //LITE_VER

	m_btnLevel1 = MenuItemImage::create("UI4HD/btn_level_1_n-hd.png", "UI4HD/btn_level_1_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel1, this));
	m_btnLevel2 = MenuItemImage::create("UI4HD/btn_level_2_n-hd.png", "UI4HD/btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
	m_btnLevel3 = MenuItemImage::create("UI4HD/btn_level_3_n-hd.png", "UI4HD/btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
	m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_4_n-hd.png", "UI4HD/btn_level_4_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this));
	m_btnLevel5 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
	m_btnLevel6 = MenuItemImage::create("UI4HD/btn_level_x_n-hd.png", "UI4HD/btn_level_x_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel6, this));

	std::string strStep1("1 Step");
	strStep1 = UTF8(strStep1);
	std::string strStep2("2 Step");
	strStep2 = UTF8(strStep2);
	std::string strStep3("3 Step");
	strStep3 = UTF8(strStep3);
	std::string strStep4("4 Step");
	strStep4 = UTF8(strStep4);
	std::string strStep5("5 Step");
	strStep5 = UTF8(strStep5);
	PrintStyle(m_btnLevel1, strStep1, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel2, strStep2, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel3, strStep3, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel4, strStep4, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel5, strStep5, sizeofFont_s, posOfDesc);


#endif //LITE_VER


	ReflushSelectedMenuItem(level);



	Menu* levelSelectMenu = Menu::create(m_btnLevel1, m_btnLevel2, m_btnLevel3, NULL);
	levelSelectMenu->alignItemsHorizontallyWithPadding(0);
	levelSelectMenu->setAnchorPoint(Point(0, 0));
	levelSelectMenu->setPosition(posOfSelectMenu1);
	this->addChild(levelSelectMenu, 2, 2);

	Menu* levelSelectMenu2 = Menu::create(m_btnLevel4, m_btnLevel5, m_btnLevel6, NULL);
	levelSelectMenu2->alignItemsHorizontallyWithPadding(0);
	levelSelectMenu2->setAnchorPoint(Point(0, 0));
	levelSelectMenu2->setPosition(posOfSelectMenu2);
	this->addChild(levelSelectMenu2, 2, 2);


	MenuItemImage* homeBtnItem = MenuItemImage::create("UI4HD/homeBtn_n-hd.png", "UI4HD/homeBtn_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedHomeMenuItem, this));


	Menu* mainMenu = Menu::create(homeBtnItem, NULL);
	mainMenu->alignItemsHorizontallyWithPadding(5);
	mainMenu->setAnchorPoint(Point(0, 0));
	mainMenu->setPosition(posOfMainMenu);
	this->addChild(mainMenu, 2, 2);


	std::string strHintOpt("SHOW HINT");
	strHintOpt = UTF8(strHintOpt);
	m_btnHintOpt = MenuItemImage::create("UI4HD/btn_hint_n-hd.png", "UI4HD/btn_hint_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedHintMenuItem, this));		
	PrintStyle(m_btnHintOpt, strHintOpt, sizeofFont_s, posOfDesc);


	std::string strReset("RESET");
	strReset = UTF8(strReset);
	MenuItemImage* resetBtnItem = MenuItemImage::create("UI4HD/btn_eraser_n-hd.png", "UI4HD/btn_eraser_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedResetMenuItem, this));		
	PrintStyle(resetBtnItem, strReset, sizeofFont_s, posOfDesc);

	Sprite* hintOptImage = NULL;
	bool hintOption = PointManager::Instance()->GetHintOption();
	if (hintOption == true)
	{		
		hintOptImage = Sprite::create("UI4HD/btn_hint_s-hd.png");
	}
	else
	{
		hintOptImage = Sprite::create("UI4HD/btn_hint_d-hd.png");
	}

	m_btnHintOpt->setNormalImage(hintOptImage);

	Menu* optionMenu = Menu::create(m_btnHintOpt, resetBtnItem, NULL);
	optionMenu->alignItemsHorizontallyWithPadding(50);
	optionMenu->setAnchorPoint(Point(0, 0));
	optionMenu->setPosition(posOfOptonMenu);
	this->addChild(optionMenu, 2, 2);
}





void InfoScene::ReflushSelectedMenuItem(int selectedLevel)
{
	Sprite* btn = Sprite::create("UI4HD/btn_level_1_n-hd.png");
	int nWidth = btn->getContentSize().width;
	int nHeight = btn->getContentSize().height;
	const int       blinkTime = 80000;
	

	int currLevel = PointManager::Instance()->GetLevel();
	std::string selectLevelImageName = StringUtils::format("UI4HD/btn_level_%d_s-hd.png", selectedLevel);		
	Sprite* selectLevelImage = Sprite::create(selectLevelImageName);

	std::string revertLevelImgName = StringUtils::format("UI4HD/btn_level_%d_n-hd.png", currLevel);		
	Sprite* revertLevelImage = Sprite::create(revertLevelImgName);		

	const int opacityVal = 80;
	switch (selectedLevel)
	{
	case 1:
		m_btnLevel1->setNormalImage(selectLevelImage);
		break;
	case 2:
		m_btnLevel2->setNormalImage(selectLevelImage);
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP2))
		{
			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
			lock->setOpacity(opacityVal);
			lock->setPosition(Point(nWidth / 2, nHeight / 2));
			m_btnLevel2->addChild(lock);
			auto action = Blink::create(blinkTime, 100000);
			lock->runAction(action);
		}
		break;
	case 3:
		m_btnLevel3->setNormalImage(selectLevelImage);
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP3))
		{
			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
			lock->setOpacity(opacityVal);
			lock->setPosition(Point(nWidth / 2, nHeight / 2));
			m_btnLevel3->addChild(lock);
			auto action = Blink::create(blinkTime, 100000);
			lock->runAction(action);
		}		
		break;
	case 4:
		m_btnLevel4->setNormalImage(selectLevelImage);
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP4))
		{
			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
			lock->setOpacity(opacityVal);
			lock->setPosition(Point(nWidth / 2, nHeight / 2));
			m_btnLevel4->addChild(lock);
			auto action = Blink::create(blinkTime, 100000);
			lock->runAction(action);
		}
		break;
	case 5:
		m_btnLevel5->setNormalImage(selectLevelImage);
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP5))
		{
			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
			lock->setOpacity(opacityVal);
			lock->setPosition(Point(nWidth / 2, nHeight / 2));
			m_btnLevel5->addChild(lock);
			auto action = Blink::create(blinkTime, 100000);
			lock->runAction(action);
		}
		break;
	default:
		break;
	}


	if (selectedLevel != currLevel)
	{
		switch (currLevel)
		{
		case 1:
			m_btnLevel1->setNormalImage(revertLevelImage);
			break;
		case 2:
			m_btnLevel2->setNormalImage(revertLevelImage);
			if (false == PointManager::Instance()->GetCartWithPID(PID_STEP2))
			{
				Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
				lock->setOpacity(opacityVal);
				lock->setPosition(Point(nWidth / 2, nHeight / 2));
				m_btnLevel2->addChild(lock);
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}			
			break;
		case 3:
			m_btnLevel3->setNormalImage(revertLevelImage);
			if (false == PointManager::Instance()->GetCartWithPID(PID_STEP3))
			{
				Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
				lock->setOpacity(opacityVal);
				lock->setPosition(Point(nWidth / 2, nHeight / 2));
				m_btnLevel3->addChild(lock);
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}
			break;
		case 4:
			m_btnLevel4->setNormalImage(revertLevelImage);
			if (false == PointManager::Instance()->GetCartWithPID(PID_STEP4))
			{
				Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
				lock->setOpacity(64);
				lock->setPosition(Point(nWidth / 2, nHeight / 2));
				m_btnLevel4->addChild(lock);
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}
			break;
		case 5:
			m_btnLevel5->setNormalImage(revertLevelImage);
			if (false == PointManager::Instance()->GetCartWithPID(PID_STEP5))
			{
				Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
				lock->setOpacity(opacityVal);
				lock->setPosition(Point(nWidth / 2, nHeight / 2));
				m_btnLevel5->addChild(lock);
				auto action = Blink::create(blinkTime, 100000);
				lock->runAction(action);
			}
			break;
		default:
			break;
		}
	}
	PointManager::Instance()->SetLevel(selectedLevel);
}

void InfoScene::PlayBuySound()
{
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);
}


void InfoScene::callbackOnPushedHomeMenuItem(Ref* sender)
{	
	auto mainScene = MainMenuScene::createScene();
	auto tr = TransitionSlideInR::create(0.5, mainScene);
	Director::getInstance()->replaceScene(tr);
}

void InfoScene::callbackOnPushedResetMenuItem(Ref* sender)
{	

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	const int		sizeOfFont = frameSize.width*0.05f;


	UIPopupWindow *pPopupOK = UIPopupWindow::create(Sprite::create("UI4HD/black_bg.png"), Sprite::create("UI4HD/pop_common.png"));
	pPopupOK->setCallBackFunc(CC_CALLBACK_1(InfoScene::popCallback_ResetOk, this)); //콕백을 받을 함수를 설정해주시면 됩니다

																				//버튼을 추가해야겠죠 닫기 버튼!!,
	pPopupOK->addButton("UI4HD/btn_ok_s_00.png", "UI4HD/btn_ok_s_01.png", "", ui::Widget::TextureResType::LOCAL, Point(0, -70), "", 1);

	std::string strWarning("Touch OK Butten.\n If you want to reset\n game data.");
	strWarning = UTF8(strWarning);
	pPopupOK->setFontSize_Msg(sizeOfFont);
	pPopupOK->setColor_Msg(Color3B::BLACK);
	pPopupOK->setMessageString(strWarning); // 메시지 출력부분이죠 그외 타이틀도 출력가능하구요, 위치또한 바꿀수있는 멤버함수가 존재합니다.
	pPopupOK->showPopup(NULL);  //마지막으로 화면에 띄우주면 끝~  showPopup()함수의 인자는 자신이 부모다~ 라는걸 넣어주는겁니다 현재 실행되는 클래스겠죠(Layer가 아닌경우는 필히 NULL을 입력하세요)

}


void InfoScene::popCallback_ResetOk(Ref* pSender)
{
	UIPopupWindow *pPopup = (UIPopupWindow *)pSender; //현재 팝업에 대한 클래스로 캐스팅 

													  // 여기에서 콜백 받을때 어떤 버튼이 클릭됐는지 알수있으면 좋겠죠?												  
	int nTag = pPopup->getResult();
	//혹은 콜백을 다르게 선업하셔도 됩니다. 그건 여러분 몫으로 콜백2 있으니 참고해서 만드심 됍니다
	if (nTag == 1)
	{
		PointManager::Instance()->ResetMast();
	}
	pPopup->closePopup(); //팝업을 닫습니다. !! 팝업을 닫을시 필히 호출해주세요 이거 안해주면 팝업창 안사라집니다.  
}


void InfoScene::callbackOnPushedHintMenuItem(Ref* pSender)
{
	Sprite* hintOptImage = NULL;
	bool hintOption = PointManager::Instance()->GetHintOption();
	if (hintOption == true)
	{		
		hintOptImage = Sprite::create("UI4HD/btn_hint_d-hd.png");
		PointManager::Instance()->SetHintOption(false);
	}
	else
	{
		hintOptImage = Sprite::create("UI4HD/btn_hint_n-hd.png");
		PointManager::Instance()->SetHintOption(true);		
	}

	m_btnHintOpt->setNormalImage(hintOptImage);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}


void InfoScene::callbackOnPushedLevel1(Ref* sender)
{
	ReflushSelectedMenuItem(1);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel2(Ref* sender)
{
	int level = PointManager::Instance()->GetLevel();
	std::string revertLevelImgName = StringUtils::format("UI4HD/btn_level_%d_n-hd.png", level);
	Sprite* revertLevelImage = Sprite::create(revertLevelImgName);
	m_btnLevel2->setNormalImage(revertLevelImage);

	ReflushSelectedMenuItem(2);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel3(Ref* sender)
{
	int level = PointManager::Instance()->GetLevel();
	std::string revertLevelImgName = StringUtils::format("btn_level_%d_n-hd.png", level);
	Sprite* revertLevelImage = Sprite::create(revertLevelImgName);
	m_btnLevel3->setNormalImage(revertLevelImage);

	ReflushSelectedMenuItem(3);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel4(Ref* sender)
{
	int level = PointManager::Instance()->GetLevel();
	std::string revertLevelImgName = StringUtils::format("UI4HD/btn_level_%d_n-hd.png", level);
	Sprite* revertLevelImage = Sprite::create(revertLevelImgName);
	m_btnLevel4->setNormalImage(revertLevelImage);

	ReflushSelectedMenuItem(4);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel5(Ref* sender)
{
	int level = PointManager::Instance()->GetLevel();
	std::string revertLevelImgName = StringUtils::format("UI4HD/btn_level_%d_n-hd.png", level);
	Sprite* revertLevelImage = Sprite::create(revertLevelImgName);
	m_btnLevel5->setNormalImage(revertLevelImage);

	ReflushSelectedMenuItem(5);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel6(Ref* sender)
{

}




void InfoScene::cfStep2(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	PointManager::Instance()->ToggleIndicator(true);	
	//[[MKStoreManager sharedManager] buyFeature:kProductIdStep2];

}

void InfoScene::cfStep3(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	PointManager::Instance()->ToggleIndicator(true);
	//[[MKStoreManager sharedManager] buyFeature:kProductIdStep3];
}

void InfoScene::cfStep4(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	PointManager::Instance()->ToggleIndicator(true);
	//[[MKStoreManager sharedManager] buyFeature:kProductIdStep4];
}

void InfoScene::cfStep5(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	PointManager::Instance()->ToggleIndicator(true);
	//[[MKStoreManager sharedManager] buyFeature:kProductIdStep5];
}

void InfoScene::cfTotal(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	PointManager::Instance()->ToggleIndicator(true);
	//[[MKStoreManager sharedManager] buyFeature:kProductIdStep6];
}


