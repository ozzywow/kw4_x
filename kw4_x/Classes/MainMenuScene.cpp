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

	PointManager::Instance()->LoadXML();

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getFrameSize();// glview->getFrameSize();

	Sprite* background = Sprite::create("UI4HD/main_bg-hd.png");
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	this->addChild(background, 0, 0);

	MenuItemImage* playMenuItem = MenuItemImage::create("UI4HD/startBtn_n-hd.png", "UI4HD/startBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedNewGameMenuItem, this));
	MenuItemImage* infoMenuItem = MenuItemImage::create("UI4HD/infoBtn_n-hd.png", "UI4HD/infoBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedControlGameMenuItem, this));
	MenuItemImage* appleTreeMenuItem = MenuItemImage::create("UI4HD/treeBtn_n-hd.png", "UI4HD/treeBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedAppleTreeGameMenuItem, this));
	//MenuItemImage* buyMenuItem = MenuItemImage::create("UI4HD/buyBtn_n-hd.png", "UI4HD/buyBtn_s-hd.png", CC_CALLBACK_1(MainMenuScene::callbackOnPushedBuyGameMenuItem, this));
		
	Menu* mainMenu = Menu::create(playMenuItem, infoMenuItem, appleTreeMenuItem, NULL);
	mainMenu->alignItemsVerticallyWithPadding(15);
	mainMenu->setPosition(Point((frameSize.width * 0.5f), (frameSize.height * 0.3f)));
		
	this->addChild(mainMenu, 0, 0);

#define LITE_VER 
#ifdef LITE_VER
	MenuItemImage* itemApp = MenuItemImage::create("kw_banner.png", "kw_banner.png", CC_CALLBACK_1(MainMenuScene::cfFullVersion, this));

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

#endif
	return true;

}


void MainMenuScene::callbackOnPushedNewGameMenuItem(Ref* pSender)
{	
	SoundFactory* sound = SoundFactory::Instance();
	sound->play(SOUND_FILE_fighting_voice);
	PointManager::Instance()->GetNextScene(true);
}

void MainMenuScene::callbackOnPushedContinueMenuItem(Ref* pSender)
{
	PointManager::Instance()->GetNextScene(true);
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
	auto appleScene = AppleTreeScene::createScene(false);

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
	//NSURL *appStoreUrl = [NSURL URLWithString : @"http://itunes.apple.com/app/id509909625?mt=8"];
	//[[UIApplication sharedApplication] openURL:appStoreUrl];

}

