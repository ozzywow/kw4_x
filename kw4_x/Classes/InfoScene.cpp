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
    this->isRestoringPurchases = false;
#ifdef LITE_VER
	CMKStoreManager::Instance()->SetDelegate(this);
#endif


	return true;
}

void InfoScene::onExitTransitionDidStart()
{
#ifdef LITE_VER
	CCLOG("InfoScene::onExitTransitionDidStart()");
	CMKStoreManager::Instance()->ToggleIndicator(false);
#endif
}

void InfoScene::DrawItemBox()
{
    CCLOG("DrawItemBox");

    // Remove previously built UI (menus, labels) while keeping the background (tag 0)
    // and scene-internal nodes like the default Camera (which has no tag / tag -1).
    {
        Vector<Node*> toRemove;
        for (auto child : this->getChildren())
        {
            if (child->getTag() != 0 && dynamic_cast<Camera*>(child) == nullptr)
                toRemove.pushBack(child);
        }
        for (auto child : toRemove)
            this->removeChild(child, true);
    }

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	const float H_OFFSET = CalcHOffset(frameSize.height);

	int level = PointManager::Instance()->GetLevel();

	const int		sizeOfFont = FRAME_WIDTH*0.07f;
	const int       sizeofFont_s = FRAME_WIDTH*0.04f;
	const Point	posOfMainMenu = Point(FRAME_WIDTH*0.2f, H_OFFSET+(ACTIVE_HEIGHT*0.9f));

	const Point	posOfLable_level = Point(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.85f));
	const Point	posOfSelectMenu1 = Point(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.72f));
	const Point	posOfSelectMenu2 = Point(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.49f));

	const Point	posOfLable_hint = Point(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.32f));

	const Point	posOfOptonMenu = Point(FRAME_WIDTH*0.5f, H_OFFSET+(ACTIVE_HEIGHT*0.2f));
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
			m_btnLevel5 = MenuItemImage::create("UI4HD/btn_level_5_n-hd.png", "UI4HD/btn_level_5_n-hd.png", CC_CALLBACK_1(InfoScene::callbackOnPushedLevel5, this));
			PrintStyle(m_btnLevel5, strStep5, sizeofFont_s, posOfDesc);
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
	auto tr = TransitionSlideInR::create(SCENE_TRANSITION_TIME, mainScene);
	Director::getInstance()->replaceScene(tr);
}

void InfoScene::callbackOnPushedResetMenuItem(Ref* sender)
{	

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getDesignResolutionSize();
	const int		sizeOfFont = FRAME_WIDTH*0.05f;


	UIPopupWindow *pPopupOK = UIPopupWindow::create(Sprite::create("UI4HD/black_bg.png"), Sprite::create("UI4HD/pop_common.png"));
	pPopupOK->setCallBackFunc(CC_CALLBACK_1(InfoScene::popCallback_ResetOk, this));
	pPopupOK->addButton("UI4HD/btn_ok_s_00.png", "UI4HD/btn_ok_s_00.png", "", ui::Widget::TextureResType::LOCAL, Point(-100, -70), "", 0);
	pPopupOK->addButton("UI4HD/btn_ok_s_01.png", "UI4HD/btn_ok_s_01.png", "", ui::Widget::TextureResType::LOCAL, Point(100, -70), "", 1);

	// 한국 마켓 앱인데 안내가 영어(+오타)였다 → 한글로 교체.
	// 이 파일은 UTF-8(pragma 없음)이라 UTF8() 매크로는 Win32 에서 이중변환으로 깨진다.
	// StudyScene 의 "단계"처럼 UTF-8 바이트를 직접 지정하고 malgun 으로 렌더한다.
	std::string strWarning =
		"\xEA\xB2\x8C\xEC\x9E\x84 "                                          // 게임
		"\xEB\x8D\xB0\xEC\x9D\xB4\xED\x84\xB0\xEB\xA5\xBC "                  // 데이터를
		"\xEC\xB4\x88\xEA\xB8\xB0\xED\x99\x94\xED\x95\xA0\xEA\xB9\x8C\xEC\x9A\x94?"; // 초기화할까요?
	pPopupOK->setFontName_Msg(KR_FONT_TTF);
	pPopupOK->setFontSize_Msg(sizeOfFont);
	pPopupOK->setColor_Msg(Color3B::BLACK);
	pPopupOK->setMessageString(strWarning);
	pPopupOK->showPopup(NULL);
}


void InfoScene::callbackOnPushedRestoreMenuItem(Ref* sender)
{
    if( isProgress == true ) return;
    isProgress = true;
    isRestored = false;
    isRestoringPurchases = true;  // productPurchased의 levelup 사운드 억제

	CMKStoreManager::Instance()->ToggleIndicator(true);
	CMKStoreManager::Instance()->restorePreviousTransactions();
}

void InfoScene::popCallback_ResetOk(Ref* pSender)
{
	UIPopupWindow *pPopup = (UIPopupWindow *)pSender;
	int nTag = pPopup->getResult();
	if (nTag == 1)
	{
		ReflushSelectedMenuItem(1);
		PointManager::Instance()->ResetMast();
		CharacterFactory::Instance()->resetData();
	}
	else if (nTag == 2)
	{
		this->DrawItemBox();
	}
	pPopup->closePopup();
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
	ReflushSelectedMenuItem(2);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel3(Ref* sender)
{

	ReflushSelectedMenuItem(3);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel4(Ref* sender)
{
	ReflushSelectedMenuItem(4);
	SoundFactory::Instance()->play(SOUND_FILE_tick_effect);
}

void InfoScene::callbackOnPushedLevel5(Ref* sender)
{
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
	cocos2d::log("[Billing] InfoScene::cfTotal clicked (isProgress=%d, id=%s)", (int)isProgress, ckProductIdTotal);
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
	cocos2d::log("[Billing] InfoScene::productPurchased <- %s (unlocking)", productId.c_str());
    CMKStoreManager::Instance()->ToggleIndicator(false);
    isProgress = false;

    // 신규개선 : 아무거나 구매해도 모든 단계 다 언락처리한다. -ozzywow-
    if (productId == ckProductIdTotal ||
        productId == ckProductIdStep2 ||
        productId == ckProductIdStep3 ||
        productId == ckProductIdStep4 ||
        productId == ckProductIdStep5 )
    {
        PointManager::Instance()->SetCartWithPID(PID_STEP2, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP3, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP4, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP5, true);
        PointManager::Instance()->SetCartWithPID(PID_TOTAL, true);
        PointManager::Instance()->SaveData();
    }
    /* 이전 코드
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
     */

	this->DrawItemBox();
    if (!isRestoringPurchases)
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

    cocos2d::log("restorePreviousTransactions count=%d", count);

    CMKStoreManager::Instance()->ToggleIndicator(false);
    isRestored = true;
    isProgress = false;
    isRestoringPurchases = false;

    // IAP 콜백은 백그라운드 스레드에서 올 수 있으므로 사운드·UI를 메인 스레드로 디스패치
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, count]()
    {
        if (count > 0)
            SoundFactory::Instance()->play(SOUND_FILE_wind_effect);
        else
            SoundFactory::Instance()->play(SOUND_FILE_dingling_effect);

        auto frameSize = Director::getInstance()->getOpenGLView()->getDesignResolutionSize();
        const float cx = frameSize.width  * 0.5f;
        const float cy = frameSize.height * 0.5f;
        const int sizeOfFont      = (int)(FRAME_WIDTH * 0.048f);
        const int sizeOfTitleFont = (int)(FRAME_WIDTH * 0.062f);

        // UIPopupWindow 없이 직접 Node 조합 — OK 버튼 없이 3초 후 자동 닫힘
        auto popup = Node::create();
        popup->setCascadeOpacityEnabled(true);
        popup->setPosition(cx, cy);
        this->addChild(popup, 20);

        // 반투명 어두운 오버레이
        auto overlay = Sprite::create("UI4HD/black_bg.png");
        overlay->setPosition(Vec2::ZERO);
        popup->addChild(overlay, 0);

        // 패널 배경 (사이즈를 먼저 확보한 뒤 z=2에 추가)
        auto panel = Sprite::create("UI4HD/pop_common.png");
        panel->setPosition(Vec2::ZERO);
        const Size ps = panel->getContentSize();

        // 황금빛 테두리: 패널보다 큰 채운 사각형 두 겹을 z=1에 배치
        // 외곽(황금 갈색) → 내측(밝은 금색) → 패널(z=2)이 중앙을 덮는 구조
        const float outerPad = 12.0f;
        const float innerPad =  4.0f;
        auto border = DrawNode::create();
        border->drawSolidRect(
            Vec2(-ps.width * 0.5f - outerPad, -ps.height * 0.5f - outerPad),
            Vec2( ps.width * 0.5f + outerPad,  ps.height * 0.5f + outerPad),
            Color4F(0.76f, 0.48f, 0.09f, 1.0f));   // 진한 황금 갈색
        border->drawSolidRect(
            Vec2(-ps.width * 0.5f - innerPad, -ps.height * 0.5f - innerPad),
            Vec2( ps.width * 0.5f + innerPad,  ps.height * 0.5f + innerPad),
            Color4F(0.98f, 0.87f, 0.52f, 1.0f));   // 밝은 금색 내측선
        popup->addChild(border, 1);
        popup->addChild(panel,  2);

        // 캐릭터 (패널 위쪽, 페이드인 + 살짝 튀어오름)
        const char* charImg = (count > 0) ? "UI4HD/boxboyHappy-hd.png" : "UI4HD/boxboySad-hd.png";
        Sprite* character = Sprite::create(charImg);
        if (character)
        {
            character->setPosition(Vec2(0.0f, 148.0f));
            character->setOpacity(0);
            popup->addChild(character, 3);
            character->runAction(Sequence::create(
                FadeIn::create(0.3f),
                EaseBackOut::create(MoveBy::create(0.3f, Vec2(0.0f, 12.0f))),
                nullptr));
        }

        // 제목 (진한 황금 갈색)
        std::string strTitle = (count > 0) ? "복구 완료!" : "구매 복구";
        auto titleLabel = Label::createWithTTF(UTF8(strTitle), "fonts/malgun.ttf", sizeOfTitleFont);
        titleLabel->setColor(Color3B(122, 66, 8));
        titleLabel->setPosition(Vec2(0.0f, 42.0f));
        popup->addChild(titleLabel, 3);

        // 메시지 (중간 갈색)
        std::string strMsg = (count > 0)
            ? "구매한 단계가\n모두 복구되었습니다!"
            : "복구할 구매 내역이\n없습니다.";
        auto msgLabel = Label::createWithTTF(UTF8(strMsg), "fonts/malgun.ttf", sizeOfFont);
        msgLabel->setColor(Color3B(82, 50, 18));
        msgLabel->setHorizontalAlignment(TextHAlignment::CENTER);
        msgLabel->setLineSpacing(6.0f);
        msgLabel->setPosition(Vec2(0.0f, -18.0f));
        popup->addChild(msgLabel, 3);

        // 등장: 스프링 팝업 → 2.4초 대기 → 부드럽게 사라짐
        popup->setScale(0.75f);
        popup->setOpacity(0);
        popup->runAction(Sequence::create(
            Spawn::create(
                EaseBackOut::create(ScaleTo::create(0.35f, 1.0f)),
                FadeIn::create(0.25f),
                nullptr),
            DelayTime::create(2.4f),
            Spawn::create(
                ScaleTo::create(0.3f, 0.88f),
                FadeOut::create(0.3f),
                nullptr),
            RemoveSelf::create(),
            nullptr));
    });
}
