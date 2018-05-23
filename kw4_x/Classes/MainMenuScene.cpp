#include "stdafx.h"

#include "MainMenuScene.h"
#include "SoundFactory.h"
#include "PointManager.h"
#include "AppleTreeScene.h"
#include "InfoScene.h"


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

	PointManager::Instance()->SaveData();
	

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	auto frameSize = glview->getDesignResolutionSize();	
	auto H_OFFSET = (frameSize.height - FRAME_HEIGHT)*0.5;

	Sprite* background = NULL;
	if (ResolutionPolicy::FIXED_WIDTH == glview->getResolutionPolicy())
	{
		background = Sprite::create("UI4HD/main_bg-hdx.png");
	}
	else
	{
		background = Sprite::create("UI4HD/main_bg-hd.png");
	}
	
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	this->addChild(background, 0, 0);

	MenuItemImage* playMenuItem = MenuItemImage::create("UI4HD/startBtn_n-hd.png", "UI4HD/startBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedNewGameMenuItem, this));
	MenuItemImage* infoMenuItem = MenuItemImage::create("UI4HD/infoBtn_n-hd.png", "UI4HD/infoBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedControlGameMenuItem, this));
	MenuItemImage* appleTreeMenuItem = MenuItemImage::create("UI4HD/treeBtn_n-hd.png", "UI4HD/treeBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedAppleTreeGameMenuItem, this));
	//MenuItemImage* buyMenuItem = MenuItemImage::create("UI4HD/buyBtn_n-hd.png", "UI4HD/buyBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedBuyGameMenuItem, this));
		
	Menu* mainMenu = Menu::create(playMenuItem, infoMenuItem, appleTreeMenuItem, NULL);
	mainMenu->alignItemsVerticallyWithPadding(20);
	mainMenu->setPosition(Point((FRAME_WIDTH * 0.5f), H_OFFSET+(FRAME_HEIGHT * 0.3f)));
		
	this->addChild(mainMenu, 0, 0);


#ifdef LITE_VER	
	if (false == PointManager::Instance()->GetCartWithPID(PID_TOTAL))
	{

		MenuItemImage* itemApp = MenuItemImage::create("UI4HD/kw_banner-hd.png", "UI4HD/kw_banner-hd.png", CC_CALLBACK_1(MainMenuScene::cfFullVersion, this));

		Menu* appMenu = Menu::create(itemApp, NULL);
		appMenu->alignItemsHorizontallyWithPadding(5);
		appMenu->setAnchorPoint(Point::ANCHOR_MIDDLE);
		appMenu->setPosition(Point(frameSize.width*0.5f, frameSize.height*0.9f));
		this->addChild(appMenu, 2, 2);

		const Point posOfBeggin = Point(frameSize.width*0.5f, frameSize.height);
		const Point posOfEnd = Point(frameSize.width*0.5f, frameSize.height*0.9f);

		appMenu->setPosition(posOfBeggin);
		Point destPos = posOfEnd;

		MoveTo* moveToAction = MoveTo::create(2.0, destPos);
		appMenu->runAction(moveToAction);

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
		pCharacterSprite->setPosition(Point(frameSize.width*0.8f, frameSize.height*0.6f));
	}
#endif


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
	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.2, infoScene);

	auto director = Director::getInstance();
	director->replaceScene(infoScene);
}


void MainMenuScene::callbackOnPushedAppleTreeGameMenuItem(Ref* pSender)
{

	AppleTreeScene* appleScene = (AppleTreeScene*)AppleTreeScene::createScene(false);
	appleScene->initWithVal(false);

	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.5, appleScene);
	
	auto director = Director::getInstance();	
	director->replaceScene(sceneSlide);
}

void MainMenuScene::callbackOnPushedBuyGameMenuItem(Ref* pSender)
{
	//BuyScene* buyScene = [[BuyScene node] init];
	//[[CCDirector sharedDirector] replaceScene:[CCTransitionSlideInL transitionWithDuration : 0.2 scene : buyScene]];	//슬라이드
}



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
