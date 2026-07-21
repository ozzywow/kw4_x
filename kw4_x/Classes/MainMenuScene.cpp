#include "stdafx.h"

#include "MainMenuScene.h"
#include "SoundFactory.h"
#include "PointManager.h"
#include "AppleTreeScene.h"
#include "InfoScene.h"
#include "NativeShare.h"


MainMenuScene::MainMenuScene()
{
}

MainMenuScene::~MainMenuScene()
{

}


bool MainMenuScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	CCLOG("MainMenuScene::init()");

	LayerColor* layer = LayerColor::create();	
	layer->initWithColor(Color4B(255, 255, 255, 255));
	this->addChild(layer, 0);


	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	auto frameSize = glview->getDesignResolutionSize();
	const float ACTIVE_HEIGHT = CalcActiveHeight(frameSize.height);
	const float H_OFFSET = CalcHOffset(frameSize.height);

	// Always FIXED_WIDTH: use -hdx background which is taller and covers all aspect ratios
	Sprite* background = Sprite::create("UI4HD/main_bg-hdx.png");
	
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	this->addChild(background, 0, 0);

	MenuItemImage* playMenuItem = MenuItemImage::create("UI4HD/startBtn_n-hd.png", "UI4HD/startBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedNewGameMenuItem, this));
	MenuItemImage* infoMenuItem = MenuItemImage::create("UI4HD/infoBtn_n-hd.png", "UI4HD/infoBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedControlGameMenuItem, this));
	MenuItemImage* appleTreeMenuItem = MenuItemImage::create("UI4HD/treeBtn_n-hd.png", "UI4HD/treeBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedAppleTreeGameMenuItem, this));

	Menu* mainMenu = Menu::create(playMenuItem, infoMenuItem, appleTreeMenuItem, NULL);
	mainMenu->alignItemsVerticallyWithPadding(20);
	mainMenu->setPosition(Point((FRAME_WIDTH * 0.5f), H_OFFSET+(ACTIVE_HEIGHT * 0.3f)));
		
	this->addChild(mainMenu, 0, 0);


	// ── 앱 소개 페이지 공유 버튼 ([칭찬나무] 아래 중앙) ──
	{
		// 위 세 버튼과 같은 금색 판. 글자("알려주기")는 다른 메뉴 버튼들과 마찬가지로
		// 이미지에 구워져 있어 별도 폰트 없이도 동일하게 렌더된다.
		MenuItemImage* shareItem = MenuItemImage::create(
			"UI4HD/shareBtn_n-hd.png", "UI4HD/shareBtn_s-hd.png",
			CC_CALLBACK_1(MainMenuScene::cfShare, this));

		const Size shareSize = shareItem->getContentSize();

		Menu* shareMenu = Menu::create(shareItem, NULL);
		// 세로 3버튼 메뉴와 같은 간격(20)으로 이어 붙여 네 번째 버튼처럼 보이게 한다
		const float mainMenuHeight = playMenuItem->getContentSize().height*3 + 20.0f*2;
		const float mainMenuBottom = (H_OFFSET + (ACTIVE_HEIGHT*0.3f)) - mainMenuHeight*0.5f;
		shareMenu->setPosition(Point(FRAME_WIDTH*0.5f,
									 mainMenuBottom - 20.0f - shareSize.height*0.5f));
		this->addChild(shareMenu, 0, 0);
	}


	// 유료 정식버전 안내 배너 (iOS 전용). 전체 잠금해제를 이미 구매했으면 띄우지 않는다.
#ifdef LITE_VER
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
	if (false == PointManager::Instance()->GetCartWithPID(PID_TOTAL))
	{
		MenuItemImage* itemApp = MenuItemImage::create("UI4HD/kw_banner-hd.png", "UI4HD/kw_banner-hd.png", CC_CALLBACK_1(MainMenuScene::cfFullVersion, this));

		Menu* appMenu = Menu::create(itemApp, NULL);

		appMenu->alignItemsHorizontallyWithPadding(5);
		appMenu->setAnchorPoint(Point::ANCHOR_MIDDLE);
		this->addChild(appMenu, 2, 2);

		// 화면 위쪽 밖에서 슬라이드로 내려온다.
		// 배경 아트(main_bg-hdx)가 레터박스를 포함한 프레임 전체에 그려지므로
		// 배너 위치도 ACTIVE_HEIGHT가 아니라 frameSize 기준이어야 타이틀 로고 위에 걸린다.
		const Point posOfBeggin = Point(frameSize.width*0.5f, frameSize.height);
		const Point posOfEnd = Point(frameSize.width*0.5f, frameSize.height*0.9f);

		appMenu->setPosition(posOfBeggin);

		MoveTo* moveToAction = MoveTo::create(2.0, posOfEnd);
		appMenu->runAction(moveToAction);
	}
#endif
#endif


	// 날아다니는 나비 (장식)
	Sprite* pCharacterSprite = Sprite::create("UI4HD/change_fly_6-hd.png");
	pCharacterSprite->setFlippedX(true);

	Animation* animationLavar = Animation::create();
	animationLavar->addSpriteFrameWithFile("UI4HD/change_fly_6-hd.png");
	animationLavar->addSpriteFrameWithFile("UI4HD/change_fly_7-hd.png");
	animationLavar->setDelayPerUnit(0.3f);

	Animate* animationLavarPlay = Animate::create(animationLavar);
	RepeatForever* repeatForever = RepeatForever::create(animationLavarPlay);
	pCharacterSprite->runAction(repeatForever);

	this->addChild(pCharacterSprite, 3, 3);
	// 배경 아트(main_bg-hdx, 640x1385)가 프레임 중앙에 원본 크기로 깔리므로,
	// 나비도 화면이 아니라 아트 중앙 기준으로 배치해야 어느 화면비에서도 타이틀 로고를 피한다.
	// (frameSize*0.6 으로 두면 9:20 폰처럼 긴 화면에서 로고 글자 위로 올라온다)
	pCharacterSprite->setPosition(Point(frameSize.width*0.8f, frameSize.height*0.5f + 50.0f));


	return true;

}


void MainMenuScene::onExitTransitionDidStart()
{
	CCLOG("MainMenuScene::onExitTransitionDidStart()");
}

void MainMenuScene::callbackOnPushedNewGameMenuItem(Ref* pSender)
{	
	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_fighting_voice);
	PointManager::Instance()->GetNextScene(true, false);
}



void MainMenuScene::callbackOnPushedControlGameMenuItem(Ref* pSender)
{
	auto infoScene = InfoScene::createScene();
	Director::getInstance()->replaceScene(infoScene);
}


void MainMenuScene::callbackOnPushedAppleTreeGameMenuItem(Ref* pSender)
{

	AppleTreeScene* appleScene = (AppleTreeScene*)AppleTreeScene::createScene(false);

	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.5, appleScene);
	
	auto director = Director::getInstance();	
	director->replaceScene(sceneSlide);
}

void MainMenuScene::callbackOnPushedBuyGameMenuItem(Ref* pSender)
{
}


// 앱 소개 페이지 링크 공유.
// 모바일은 네이티브 공유 시트(카톡·메시지 등), 데스크톱은 브라우저로 링크 열기.
void MainMenuScene::cfShare(Ref* pSender)
{
	SoundFactory::Instance()->play(SOUND_FILE_click_effect);

	if (NativeShare::isSupported())
	{
		std::string msg = UTF8("우리아이 한글떼기 - 그림과 소리로 배우는 우리 아이 첫 한글\n");
		msg += SHARE_URL;
		NativeShare::share(msg);
	}
	else
	{
		cocos2d::Application::getInstance()->openURL(SHARE_URL);
	}
}


// 유료 정식버전(별도 앱) 스토어 페이지로 이동
void MainMenuScene::cfFullVersion(Ref* pSender)
{
#if( CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	cocos2d::Application::getInstance()->openURL(BUY_AT_STORE_URL);
#endif
}


void MainMenuScene::productFetchComplete()
{
	cocos2d::log("productFetchComplete");	
}
void MainMenuScene::productPurchased(std::string productId)
{
	cocos2d::log("productPurchased /%s", productId.c_str());
	
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
}
void MainMenuScene::transactionCanceled()
{
	cocos2d::log("transactionCanceled");	
}

void MainMenuScene::restorePreviousTransactions(int count)
{
	cocos2d::log("restorePreviousTransactions");

}
