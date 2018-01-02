#include "stdafx.h"
#include "AppleTreeScene.h"
#include "PointManager.h"
#include "CharacterFactory.h"
#include "SoundFactory.h"
#include "MainMenuScene.h"



AppleTreeScene::AppleTreeScene()
{
	m_isPlay = false;	
	_touchedLayerID = -1;
	_touchedHandlerLayer = NULL;
}
AppleTreeScene::~AppleTreeScene()
{
	
}

bool AppleTreeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	return true;	
}


bool AppleTreeScene::initWithVal(bool isPlay)
{
	m_isPlay = isPlay;
	PointManager* pPointManager = PointManager::Instance();
	pPointManager->SaveData();

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	//frameSize = glview->getFrameSize();
	frameSize = glview->getDesignResolutionSize();

	Sprite* background = Sprite::create("UI4HD/tree_bg-hd.png");
	background->setAnchorPoint(Point::ANCHOR_MIDDLE);
	background->setPosition(frameSize.width*0.5f, frameSize.height*0.5f);
	this->addChild(background, 0, 0);

	MenuItemImage* homeBtnItem = NULL;

	if (m_isPlay)
	{
		homeBtnItem = MenuItemImage::create("UI4HD/nextBtn_n-hd.png", "UI4HD/nextBtn_s-hd.png", CC_CALLBACK_1(AppleTreeScene::callbackOnPushedNewGameMenuItem, this));
	}
	else {
		homeBtnItem = MenuItemImage::create("UI4HD/homeBtn_n-hd.png", "UI4HD/homeBtn_s-hd.png", CC_CALLBACK_1(AppleTreeScene::callbackOnPushedHomeMenuItem, this));
	}


	const Point posOfMenu(frameSize.width*0.2f, frameSize.height*0.9f);

	Menu* mainMenu = Menu::create(homeBtnItem, NULL);
	mainMenu->alignItemsHorizontallyWithPadding(0);
	mainMenu->setAnchorPoint(Point::ANCHOR_TOP_LEFT);
	mainMenu->setPosition(posOfMenu);
	this->addChild(mainMenu, 2, 2);


	CharacterFactory* pCharacterFactory = CharacterFactory::Instance();
	for (int i = 0; i<MAX_SIZE_OF_CHARACTER_POOL; ++i)
	{
		Character* pCharacter = pCharacterFactory->GetCharacterWithID(i);
		if (NULL == pCharacter)
		{
			break;
		}

		Sprite* pCharacterSprite = NULL;
		if (pCharacter->type == CT_NONE)
		{
			continue;
		}
		else if (pCharacter->type == CT_APPLE)
		{
			pCharacterSprite = Sprite::create("UI4HD/apple-hd.png");
		}
		else if (pCharacter->type == CT_LAVER)
		{
			pCharacterSprite = Sprite::create("UI4HD/lavar01-hd.png");
			if (0 == (rand() % 2))
			{
				pCharacterSprite->setFlippedX(true);
			}

			Animation* animationLavar = Animation::create();
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar01-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar02-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar03-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar04-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar05-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/lavar06-hd.png");
			animationLavar->setDelayPerUnit(0.2f);

			Animate* animationLavarPlay = Animate::create(animationLavar);
			RepeatForever* repeatForever = RepeatForever::create(animationLavarPlay);
			pCharacterSprite->runAction(repeatForever);


			int biteCount = pCharacter->biteCount;
			if (biteCount > 0)
			{
				std::string countUIFileName = StringUtils::format("UI4HD/bate_count_%d-hd.png", biteCount);
				Sprite* pBiteCountSprite = Sprite::create(countUIFileName);
				if (pBiteCountSprite)
				{
					pBiteCountSprite->setPosition(pCharacterSprite->getContentSize().width*0.5f, pCharacterSprite->getContentSize().height*0.8f);
					pCharacterSprite->addChild(pBiteCountSprite, 0);
				}
			}
			

		}
		else if (pCharacter->type == CT_FLY)
		{
			pCharacterSprite = Sprite::create("UI4HD/change_fly_6-hd.png");
			if (0 == (rand() % 2))
			{
				pCharacterSprite->setFlippedX(true);
			}

			Animation* animationLavar = Animation::create();
			animationLavar->addSpriteFrameWithFile("UI4HD/change_fly_6-hd.png");
			animationLavar->addSpriteFrameWithFile("UI4HD/change_fly_7-hd.png");
			animationLavar->setDelayPerUnit(0.3f);

			Animate* animationLavarPlay = Animate::create(animationLavar);
			RepeatForever* repeatForever = RepeatForever::create(animationLavarPlay);
			pCharacterSprite->runAction(repeatForever);

			int biteCount = pCharacter->biteCount;
			std::string countUIFileName = StringUtils::format("UI4HD/bate_count_%d-hd.png", biteCount);
		}

		pCharacterSprite->setUserData((void*)pCharacter);
		this->addChild(pCharacterSprite, 1, 1);
		m_arrSprites.push_back(pCharacterSprite);
		Point applePos((float)pCharacter->posX, (float)pCharacter->posY);
		pCharacterSprite->setPosition(applePos);
	}

	// 터치 이벤트를 담당할 Layer를 만든 후 GameScene에 넣습니다.
	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = CC_CALLBACK_2(AppleTreeScene::onTouchBegan, this);
	listener->onTouchMoved = CC_CALLBACK_2(AppleTreeScene::onTouchMoved, this);
	listener->onTouchEnded = CC_CALLBACK_2(AppleTreeScene::onTouchEnded, this);
	listener->onTouchCancelled = CC_CALLBACK_2(AppleTreeScene::onTouchCancelled, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_bgm);

	return true;
}


Sprite* AppleTreeScene::ChangeToFly(int laverID)
{
	Sprite* pChangeSprite = NULL;
	Character* pCharacter = NULL;
	for (auto itr = m_arrSprites.begin(); itr != m_arrSprites.end(); ++itr)
	{
		pChangeSprite = *itr;
		pCharacter = (Character*)pChangeSprite->getUserData();
		if (pCharacter->index == laverID)
		{
			pCharacter->type = CT_FLY;
			break;
		}
		else
		{
			pChangeSprite = NULL;
		}
	}


	// 나비로 변신
	if (pChangeSprite)
	{
		Point genPos = pChangeSprite->getPosition();
		m_arrSprites.remove(pChangeSprite);
		this->removeChild(pChangeSprite, true);


		pChangeSprite = Sprite::create("UI4HD/change_fly_0-hd.png");
		pChangeSprite->setPosition(genPos);
		if (0 == (rand() % 2))
		{
			pChangeSprite->setFlippedX(true);
		}

		pChangeSprite->setUserData(pCharacter);

		Animation* animationChange = Animation::create();
		animationChange->addSpriteFrameWithFile("UI4HD/change_fly_1-hd.png");
		animationChange->addSpriteFrameWithFile("UI4HD/change_fly_2-hd.png");
		animationChange->addSpriteFrameWithFile("UI4HD/change_fly_3-hd.png");
		animationChange->addSpriteFrameWithFile("UI4HD/change_fly_4-hd.png");
		animationChange->addSpriteFrameWithFile("UI4HD/change_fly_5-hd.png");
		

		Animate* animationFlyChange = Animate::create(animationChange);
		Repeat* repeatChange = Repeat::create(animationFlyChange, 1);
		


		Animation* animationFly = Animation::create();
		animationFly->addSpriteFrameWithFile("UI4HD/change_fly_6-hd.png");
		animationFly->addSpriteFrameWithFile("UI4HD/change_fly_7-hd.png");
		Animate* animationFlyPlay = Animate::create(animationFly);
		RepeatForever* repeatFly = RepeatForever::create(animationFlyPlay);

		Sequence* pSeq = Sequence::create(repeatChange, repeatFly, NULL);
		pChangeSprite->runAction(pSeq);


		this->addChild(pChangeSprite, 0, 0);
		m_arrSprites.push_back(pChangeSprite);


		SoundFactory* sound = SoundFactory::Instance();
		sound->play(SOUND_FILE_wind_effect);

	}

	return pChangeSprite;
}

void	AppleTreeScene::RemoveApple(int appleID)
{
	
	Sprite* apple = NULL;
	for (auto itr = m_arrSprites.begin(); itr != m_arrSprites.end(); ++itr)
	{
		apple = *itr;
		Character* pCharacter = (Character*)apple->getUserData();
		if (pCharacter->index == appleID)
		{
			pCharacter->type = CT_NONE;
			m_arrSprites.erase(itr);			
			break;
		}		
	}

	if (apple)
	{
		this->removeChild(apple, true);
	}
}

Sprite* AppleTreeScene::GetAppleSprite(int appleID)
{
	Sprite* apple = NULL;
	for (auto itr = m_arrSprites.begin(); itr != m_arrSprites.end(); ++itr)
	{
		apple = *itr;
		Character* pCharacter = (Character*)apple->getUserData();
		if (pCharacter->index == appleID)
		{
			return apple;
		}
	}	
	return NULL;
}

void	AppleTreeScene::ChangeBiteCount(int ID)
{	
	Sprite* pChangeSprite = NULL;
	Character* pCharacter = NULL;
	for (auto itr = m_arrSprites.begin(); itr != m_arrSprites.end(); ++itr)
	{
		pChangeSprite = *itr;
		pCharacter = (Character*)pChangeSprite->getUserData();
		if (pCharacter->index == ID)
		{
			pCharacter->type = CT_LAVER;
			break;
		}
		else
		{
			pChangeSprite = NULL;
		}
	}

	//카운트 올리기
	if (pChangeSprite)
	{
		pChangeSprite->removeAllChildrenWithCleanup(true);

		int biteCount = pCharacter->biteCount;
		std::string countUIFileName = StringUtils::format("UI4HD/bate_count_%d-hd.png", biteCount);
		

		Sprite* pBiteCountSprite = Sprite::create(countUIFileName);			
		if (pBiteCountSprite)
		{			
			pChangeSprite->addChild(pBiteCountSprite, 0, 0);			
			pBiteCountSprite->setPosition(pChangeSprite->getContentSize().width*0.5f, pChangeSprite->getContentSize().height*0.8f);
		}

	}
}



void	AppleTreeScene::callbackOnPushedHomeMenuItem(Ref* pSender)
{
	auto  mainScene = MainMenuScene::createScene();	
	TransitionSlideInL* sceneSlide = TransitionSlideInL::create(0.5, mainScene);
	auto director = Director::getInstance();
	director->replaceScene(sceneSlide);
}

void	AppleTreeScene::callbackOnPushedNewGameMenuItem(Ref* pSender)
{
	PointManager::Instance()->GetNextScene(true);
}

void	AppleTreeScene::callbackOnPushedBuyMenuItem(Ref* pSender)
{
}


Sprite* AppleTreeScene::GetTouchedLayer(Point location)
{
	for (auto itr = this->m_arrSprites.begin(); itr != this->m_arrSprites.end(); ++itr)
	{
		Sprite* touchedLayer = *itr;
		if (NULL == touchedLayer) break;

		// 네모 sprite의 사이즈의 반을 계산합니다.

		float halfWidth = touchedLayer->getContentSize().width / 2.0;
		float halfHeight = touchedLayer->getContentSize().height / 2.0;


		Point pos = touchedLayer->getPosition();
		// 터치된 위치가 네모 안에 들어오는 지 계산합니다.
		if (location.x < (pos.x + halfWidth) &&
			location.x >(pos.x - halfWidth) &&
			location.y > (pos.y - halfHeight) &&
			location.y < (pos.y + halfHeight))
		{
			Character* pCharacter = (Character*)touchedLayer->getUserData();
			_touchedLayerID = pCharacter->index;

			return touchedLayer;
		}

	}

	_touchedLayerID = -1;
	return NULL;
}

bool AppleTreeScene::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (_touchedHandlerLayer != NULL) return false;


	Point location = touch->getLocation();


	// 텍스트버튼이 터치되었는지 검사한다.
	Sprite* touchedLayer = GetTouchedLayer(location);
	if (NULL == touchedLayer)
	{
		return false;
	}


	this->reorderChild(touchedLayer, 0);


	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_giggle_effect);

	_touchedHandlerLayer = touchedLayer;

	return true;
}

void AppleTreeScene::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	_touchedHandlerLayer->setPosition(location);

	return;
}


void AppleTreeScene::onTouchEnded(Touch* touch, Event *unused_event)
{
	if (_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	if (_touchedLayerID >= 0)
	{
		Character* pCharacter = (Character*)_touchedHandlerLayer->getUserData();
		if (pCharacter == NULL) return;

		// 사과이면..
		if (pCharacter->type == CT_APPLE || pCharacter->type == CT_FLY)
		{

			pCharacter->posX = (int)location.x;
			pCharacter->posY = (int)location.y;
		}
		// 애벌레면..
		else if (pCharacter->type == CT_LAVER)
		{

			int appleBiteCount = pCharacter->biteCount;

			CharacterFactory* pCharacterFactory = CharacterFactory::Instance();
			int biteAppleID = pCharacterFactory->FindAppleIDByPos(location);
			if (biteAppleID >= 0)
			{
				appleBiteCount += 1;
				if (appleBiteCount >= 10)
				{
					_touchedHandlerLayer = this->ChangeToFly(_touchedLayerID);
				}
				else
				{
					pCharacter->biteCount = appleBiteCount;
					this->ChangeBiteCount(_touchedLayerID);
				}

				// 사과를 지운다.
				Character* pApple = pCharacterFactory->GetCharacterWithID(biteAppleID);
				if (pApple)
				{
					pApple->type = CT_NONE;
				}
				this->RemoveApple(biteAppleID);

			}
			else
			{
				pCharacter->posX = (int)location.x;
				pCharacter->posY = (int)location.y;
			}

		}



		const float heightOfJump = frameSize.height*0.03f;
		Point jump2Point(_touchedHandlerLayer->getPosition());
		auto actionTo = JumpTo::create(0.2f, jump2Point, heightOfJump, 1);
		_touchedHandlerLayer->runAction(actionTo);
	}

	_touchedLayerID = -1;
	_touchedHandlerLayer = NULL;



	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_puck_effect);


	return;
}


void AppleTreeScene::onTouchCancelled(Touch* touch, Event* unused_event)
{

}
