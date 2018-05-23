#include "stdafx.h"

#include "InfoScene.h"
#include "PointManager.h"
#include "CharacterFactory.h"
#include "SoundFactory.h"
#include "MainMenuScene.h"
#include "UI_GameResultWindow.h"
#include "MKStoreManager_cpp.h"


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

	CCLOG("InfoScene::init()");

	Sprite* background = Sprite::create("UI4HD/info_bg-hdx.png") ;

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	auto frameSize = glview->getDesignResolutionSize();
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width* 0.5f, frameSize.height * 0.5f);

	this->addChild(background, 0, 0);

	this->DrawItemBox();

	this->isProgress = false;
    this->isRestored = false;   


	return true;
}

void InfoScene::onExitTransitionDidStart()
{
#ifdef LITE_VER
	CCLOG("InfoScene::onExitTransitionDidStart()");
	CMKStoreManager::Instance()->SetDelegate(NULL);
	CMKStoreManager::Instance()->ToggleIndicator(false);
#endif
}

void InfoScene::DrawItemBox()
{
    CCLOG("DrawItemBox");
    this->removeChildByTag(2);
    this->removeChildByTag(2);
    this->removeChildByTag(2);
    this->removeChildByTag(2);
    this->removeChildByTag(2);
    
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	float H_OFFSET = (frameSize.height - FRAME_HEIGHT)*0.5;

	int level = PointManager::Instance()->GetLevel();

	const int		sizeOfFont = FRAME_WIDTH*0.07f;
	const int       sizeofFont_s = FRAME_WIDTH*0.04f;
	const Point	posOfMainMenu = Point(FRAME_WIDTH*0.2f, H_OFFSET+(FRAME_HEIGHT*0.9f));

	const Point	posOfLable_level = Point(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.85f));
	const Point	posOfSelectMenu1 = Point(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.72f));
	const Point	posOfSelectMenu2 = Point(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.49f));

	const Point	posOfLable_hint = Point(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.32f));

	const Point	posOfOptonMenu = Point(FRAME_WIDTH*0.5f, H_OFFSET+(FRAME_HEIGHT*0.2f));
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

#ifdef LITE_VER
	std::string strSaleTotal("Whole steps");
	strSaleTotal = UTF8(strSaleTotal);
	std::string strSaleStep1("1 Step)");
	strSaleStep1 = UTF8(strSaleStep1);
	std::string strSaleStep2("2 Step");
	strSaleStep2 = UTF8(strSaleStep2);
	std::string strSaleStep3("3 Step");
	strSaleStep3 = UTF8(strSaleStep3);
	std::string strSaleStep4("4 Step");
	strSaleStep4 = UTF8(strSaleStep4);
	std::string strSaleStep5("5 Step");
	strSaleStep5 = UTF8(strSaleStep5);

	m_btnLevel1 = MenuItemImage::create("UI4HD/btn_level_1_n-hd.png", "UI4HD/btn_level_1_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel1, this));	
	PrintStyle(m_btnLevel1, strStep1, sizeofFont_s, posOfDesc);

	if (false == PointManager::Instance()->GetCartWithPID(PID_TOTAL))
	{

		m_btnLevel6 = MenuItemImage::create("UI4HD/btn_level_total_n-hd.png", "UI4HD/btn_level_total_s-hd.png", CC_CALLBACK_1(InfoScene::cfTotal, this));
		PrintStyle(m_btnLevel6, strSaleTotal, sizeofFont_s, posOfDesc);

		Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
		lock->setOpacity(64);
		lock->setPosition(nWidth / 2, nHeight / 2);
		m_btnLevel6->addChild(lock);
		auto action = Blink::create(blinkTime, 100000);
		lock->runAction(action);


		//step2
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP2))
		{
			m_btnLevel2 = MenuItemImage::create("UI4HD/btn_level_2_n-hd.png", "UI4HD/btn_level_2_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep2, this));
			PrintStyle(m_btnLevel2, strSaleStep2, sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
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
			m_btnLevel2 = MenuItemImage::create("UI4HD/btn_level_2_n-hd.png", "UI4HD/btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
			PrintStyle(m_btnLevel2, strStep2, sizeofFont_s, posOfDesc);
		}



		//step3
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP3))
		{
			m_btnLevel3 = MenuItemImage::create("UI4HD/btn_level_3_n-hd.png", "UI4HD/btn_level_3_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep3, this));
			PrintStyle(m_btnLevel3, strSaleStep3, sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
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
			m_btnLevel3 = MenuItemImage::create("UI4HD/btn_level_3_n-hd.png", "UI4HD/btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
			PrintStyle(m_btnLevel3, strStep3, sizeofFont_s, posOfDesc);
		}



		//step4
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP4))
		{
			m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_4_n-hd.png", "UI4HD/btn_level_4_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep4, this));
			PrintStyle(m_btnLevel4, strSaleStep4, sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
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
			m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_4_n-hd.png", "UI4HD/btn_level_4_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this));
			PrintStyle(m_btnLevel4, strStep4, sizeofFont_s, posOfDesc);
		}


		//step5
		if (false == PointManager::Instance()->GetCartWithPID(PID_STEP5))
		{
			m_btnLevel5 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_n-hd.png", CC_CALLBACK_1(InfoScene::cfStep5, this));
			PrintStyle(m_btnLevel5, strSaleStep5, sizeofFont_s, posOfDesc);

			Sprite* lock = Sprite::create("UI4HD/lock_icon-hd.png");
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
			m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
			PrintStyle(m_btnLevel4, strStep5, sizeofFont_s, posOfDesc);
		}
	}
	else
	{
		m_btnLevel2 = MenuItemImage::create("UI4HD/btn_level_2_n-hd.png", "UI4HD/btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
		m_btnLevel3 = MenuItemImage::create("UI4HD/btn_level_3_n-hd.png", "UI4HD/btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
		m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_4_n-hd.png", "UI4HD/btn_level_4_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this));
		m_btnLevel5 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
		m_btnLevel6 = MenuItemImage::create("UI4HD/btn_level_x_n-hd.png", "UI4HD/btn_level_x_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel6, this));

		PrintStyle(m_btnLevel1, strStep1, sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel2, strStep2, sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel3, strStep3, sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel4, strStep4, sizeofFont_s, posOfDesc);
		PrintStyle(m_btnLevel5, strStep5, sizeofFont_s, posOfDesc);
	}

#else //LITE_VER

	m_btnLevel1 = MenuItemImage::create("UI4HD/btn_level_1_n-hd.png", "UI4HD/btn_level_1_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel1, this));
	m_btnLevel2 = MenuItemImage::create("UI4HD/btn_level_2_n-hd.png", "UI4HD/btn_level_2_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel2, this));
	m_btnLevel3 = MenuItemImage::create("UI4HD/btn_level_3_n-hd.png", "UI4HD/btn_level_3_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel3, this));
	m_btnLevel4 = MenuItemImage::create("UI4HD/btn_level_4_n-hd.png", "UI4HD/btn_level_4_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel4, this));
	m_btnLevel5 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
	m_btnLevel6 = MenuItemImage::create("UI4HD/btn_level_x_n-hd.png", "UI4HD/btn_level_x_s-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel6, this));

	PrintStyle(m_btnLevel1, strStep1, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel2, strStep2, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel3, strStep3, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel4, strStep4, sizeofFont_s, posOfDesc);
	PrintStyle(m_btnLevel5, strStep5, sizeofFont_s, posOfDesc);


#endif //LITE_VER


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

	std::string strRestore("Restore Purchases");
	strRestore = UTF8(strRestore);
	MenuItemImage* restoreBtnItem = MenuItemImage::create("UI4HD/btn_level_restore_s-hd.png", "UI4HD/btn_level_restore_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedRestoreMenuItem, this));
	PrintStyle(restoreBtnItem, strRestore, sizeofFont_s, posOfDesc);

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

#ifdef LITE_VER
	Menu* optionMenu = Menu::create(m_btnHintOpt, resetBtnItem, restoreBtnItem, NULL);
	optionMenu->alignItemsHorizontallyWithPadding(0);	
#else //LITE_VER
	Menu* optionMenu = Menu::create(m_btnHintOpt, resetBtnItem, NULL);
	optionMenu->alignItemsHorizontallyWithPadding(50);
#endif //LITE_VER
	optionMenu->setAnchorPoint(Point(0, 0));
	optionMenu->setPosition(posOfOptonMenu);
	this->addChild(optionMenu, 2, 2);

	ReflushSelectedMenuItem(level);
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
	const int		sizeOfFont = FRAME_WIDTH*0.05f;


	UIPopupWindow *pPopupOK = UIPopupWindow::create(Sprite::create("UI4HD/black_bg.png"), Sprite::create("UI4HD/pop_common.png"));
	pPopupOK->setCallBackFunc(CC_CALLBACK_1(InfoScene::popCallback_ResetOk, this)); //콕백을 받을 함수를 설정해주시면 됩니다

																				//버튼을 추가해야겠죠 닫기 버튼!!,
	pPopupOK->addButton("UI4HD/btn_ok_s_00.png", "UI4HD/btn_ok_s_00.png", "", ui::Widget::TextureResType::LOCAL, Point(-100, -70), "", 0);
	pPopupOK->addButton("UI4HD/btn_ok_s_01.png", "UI4HD/btn_ok_s_01.png", "", ui::Widget::TextureResType::LOCAL, Point(100, -70), "", 1);

	std::string strWarning("Touch OK Butten.\n If you want to reset\n game data.");
	strWarning = UTF8(strWarning);
	pPopupOK->setFontSize_Msg(sizeOfFont);
	pPopupOK->setColor_Msg(Color3B::BLACK);
	pPopupOK->setMessageString(strWarning); // 메시지 출력부분이죠 그외 타이틀도 출력가능하구요, 위치또한 바꿀수있는 멤버함수가 존재합니다.
	pPopupOK->showPopup(NULL);  //마지막으로 화면에 띄우주면 끝~  showPopup()함수의 인자는 자신이 부모다~ 라는걸 넣어주는겁니다 현재 실행되는 클래스겠죠(Layer가 아닌경우는 필히 NULL을 입력하세요)
}


void InfoScene::callbackOnPushedRestoreMenuItem(Ref* sender)
{
    if( isProgress == true ) return;
    isProgress = true;
    isRestored = false;
  
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->restorePreviousTransactions();
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
		CharacterFactory::Instance()->resetData();
	}
    if(nTag == 2)
    {
        this->DrawItemBox();
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
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->buyFeature(ckProductIdStep2);	

}

void InfoScene::cfStep3(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();	
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->buyFeature(ckProductIdStep3);	
}

void InfoScene::cfStep4(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();	
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->buyFeature(ckProductIdStep4);	
}

void InfoScene::cfStep5(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();	
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->buyFeature(ckProductIdStep5);	
}

void InfoScene::cfTotal(Ref* sender)
{
	if (isProgress == true) return;
	isProgress = true;

	this->PlayBuySound();
	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->buyFeature(ckProductIdTotal);	
}

void InfoScene::productFetchComplete()
{
	cocos2d::log("productFetchComplete");
	CMKStoreManager::Instance()->ToggleIndicator(false);
	isProgress = false;    
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);
}
void InfoScene::productPurchased(std::string productId)
{
	cocos2d::log("productPurchased /%s", productId.c_str());
    CMKStoreManager::Instance()->ToggleIndicator(false);
    isProgress = false;    

	if (productId == ckProductIdTotal)
	{
		PointManager::Instance()->SetCartWithPID(PID_STEP2, true);
		PointManager::Instance()->SetCartWithPID(PID_STEP3, true);
		PointManager::Instance()->SetCartWithPID(PID_STEP4, true);
		PointManager::Instance()->SetCartWithPID(PID_STEP5, true);
		PointManager::Instance()->SetCartWithPID(PID_TOTAL, true);
		PointManager::Instance()->SaveData();
	}
	else if (productId == ckProductIdStep2)
	{
		PointManager::Instance()->SetCartWithPID(PID_STEP2, true);
		PointManager::Instance()->SaveData();
	}
	else if (productId == ckProductIdStep3)
	{
		PointManager::Instance()->SetCartWithPID(PID_STEP3, true);
		PointManager::Instance()->SaveData();
	}
	else if (productId == ckProductIdStep4)
	{
		PointManager::Instance()->SetCartWithPID(PID_STEP4, true);
		PointManager::Instance()->SaveData();
	}
	else if (productId == ckProductIdStep5)
	{
		PointManager::Instance()->SetCartWithPID(PID_STEP5, true);
		PointManager::Instance()->SaveData();
	}

	this->DrawItemBox();
	SoundFactory::Instance()->play(SOUND_FILE_levelup_effect);
}
void InfoScene::transactionCanceled()
{
	cocos2d::log("transactionCanceled");
    CMKStoreManager::Instance()->ToggleIndicator(false);
    isProgress = false;    
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);
}

void InfoScene::restorePreviousTransactions(int count)
{
    if(true == isRestored){ return; }
    
    cocos2d::log("restorePreviousTransactions");
    
	CMKStoreManager::Instance()->ToggleIndicator(false);
	isRestored = true;
    isProgress = false;
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);

	//Purchase items restored.
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	const int		sizeOfFont = FRAME_WIDTH*0.05f;


	UIPopupWindow *pPopupOK = UIPopupWindow::create(Sprite::create("UI4HD/black_bg.png"), Sprite::create("UI4HD/pop_common.png"));
	pPopupOK->setCallBackFunc(CC_CALLBACK_1(InfoScene::popCallback_ResetOk, this)); //콕백을 받을 함수를 설정해주시면 됩니다
																					
	pPopupOK->addButton("UI4HD/btn_ok_s_01.png", "UI4HD/btn_ok_s_01.png", "", ui::Widget::TextureResType::LOCAL, Point(0, -70), "", 2);

    std::string strWarning;
    if(count > 0)
    {
        strWarning = "Purchase items\n Restored.";
    }
    else
    {
        strWarning = "No items purchased.";
    }
	
	strWarning = UTF8(strWarning);
	pPopupOK->setFontSize_Msg(sizeOfFont);
	pPopupOK->setColor_Msg(Color3B::BLACK);
	pPopupOK->setMessageString(strWarning); // 메시지 출력부분이죠 그외 타이틀도 출력가능하구요, 위치또한 바꿀수있는 멤버함수가 존재합니다.
	pPopupOK->showPopup(NULL);  //마지막으로 화면에 띄우주면 끝~  showPopup()함수의 인자는 자신이 부모다~ 라는걸 넣어주는겁니다 현재 실행되는 클래스겠죠(Layer가 아닌경우는 필히 NULL을 입력하세요)

}
