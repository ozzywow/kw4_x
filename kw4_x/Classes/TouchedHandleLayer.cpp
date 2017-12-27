#include "stdafx.h"
#include "TouchedHandleLayer.h"
#include "SoundFactory.h"
#include "wordFactory.h"

TouchedHandleLayer::TouchedHandleLayer()
{	
}

TouchedHandleLayer::~TouchedHandleLayer()
{
}

TouchedHandleLayer* TouchedHandleLayer::create(StudyScene* studyScene)
{
	auto ret = new (std::nothrow) TouchedHandleLayer();
	if (ret && ret->initWithVal(studyScene))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

TextLayer* TouchedHandleLayer::GetTouchedLayer(Point location)
{
	int countOfTouchButton = 8;
	if (m_studyScene->m_level == 5)
	{
		countOfTouchButton = 4;
	}

	for (auto itr = m_studyScene->m_arrayTextLayer.begin()
		; itr != m_studyScene->m_arrayTextLayer.end(); ++itr)
	{
		TextLayer* touchedLayer = *itr;		
		Sprite* touchedSprite = touchedLayer->m_backGround;

		float halfWidth = touchedSprite->getContentSize().width * 0.5f;
		float halfHeight = touchedSprite->getContentSize().height * 0.5f;


		// 터치된 위치가 네모 안에 들어오는 지 계산합니다.
		Point touchedSpritePos = touchedSprite->getPosition();
		if (location.x < (touchedSpritePos.x + halfWidth) &&
			location.x >(touchedSpritePos.x - halfWidth) &&
			location.y > (touchedSpritePos.y - halfHeight) &&
			location.y < (touchedSpritePos.y + halfHeight))
		{

			return touchedLayer;
		}
	}
	return NULL;
}

void TouchedHandleLayer::ReplaceLayerToAnswerBox(TextLayer* pLayer, Point location)
{
	// 네모상자 근처에 도달했으면 도킹해준다.
	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();
	auto frameSize = glview->getFrameSize();
	int offset = frameSize.width * 0.13f;	

	for (int i = 0; i < 4; ++i)
	{
		Point boxPoint = m_studyScene->getBoxPoint(i);
		if (location.x < boxPoint.x + offset && location.x > boxPoint.x - offset &&
			location.y < boxPoint.y + offset && location.y > boxPoint.y - offset)
		{
			pLayer->m_backGround->setPosition(boxPoint);

			TextLayer* pCurrPlacedLayer = m_studyScene->m_wordQueue[i];
			// 드랍클릭인지 검사한다.
			if (pCurrPlacedLayer == pLayer)
			{
				// 마지막 있었던 배열을 빈문자열로 채운다.
				if (pCurrPlacedLayer->m_lastPlacedAnswerBoxID >= 0)
				{
					TextLayer* emptyLayer = WordFactory::Instance()->GetEmptyLayer();
					m_studyScene->m_wordQueue[pCurrPlacedLayer->m_lastPlacedAnswerBoxID] = emptyLayer;
				}

				pCurrPlacedLayer->replaceToGenPlace();
			}
			// 밀어내기인지 검사한다.
			else if (WordFactory::Instance()->GetEmptyLayer() != pCurrPlacedLayer)
			{
				pCurrPlacedLayer->replaceToGenPlace();

				// 마지막 있었던 배열을 빈문자열로 채운다.
				if (pLayer->m_lastPlacedAnswerBoxID >= 0)
				{
					TextLayer* emptyLayer = WordFactory::Instance()->GetEmptyLayer();
					m_studyScene->m_wordQueue[pLayer->m_lastPlacedAnswerBoxID] = emptyLayer;						
				}
				// 새 위치로 이동시킨다.
				pLayer->replaceToPoint(boxPoint);				
				// 새 배열 위치에 넣어준다.
				m_studyScene->m_wordQueue[i] = pLayer;
				pLayer->m_lastPlacedAnswerBoxID = i;
				
			}
			else
			{

				// 새 위치로 이동시킨다
				pLayer->replaceToPoint(boxPoint);

				// 마지막 있었던 배열을 빈문자열로 채운다.
				if (pLayer->m_lastPlacedAnswerBoxID >= 0)
				{
					TextLayer* emptyLayer = WordFactory::Instance()->GetEmptyLayer();

					m_studyScene->m_wordQueue[pLayer->m_lastPlacedAnswerBoxID] = emptyLayer;
				}

				// 새 배열 위치에 넣어준다.
				m_studyScene->m_wordQueue[i] = pLayer;
				pLayer->m_lastPlacedAnswerBoxID = i;
			}

			return;

		}
	}

	if (pLayer->m_lastPlacedAnswerBoxID == -1)
	{
		//일반 클릭인지 검사한다.		
		float halfWidth = pLayer->m_backGround->getContentSize().width / 2.0;
		float halfHeight = pLayer->m_backGround->getContentSize().height / 2.0;
		if (location.x < (pLayer->m_genPos.x + halfWidth) &&
			location.x >(pLayer->m_genPos.x - halfWidth) &&
			location.y > (pLayer->m_genPos.y - halfHeight) &&
			location.y < (pLayer->m_genPos.y + halfHeight))
		{

			// 재빠르게 클릭했는지 검사한다.
			long intervalTime = timeGetTimeEx() - pLayer->m_touchBeganTime;
			if (intervalTime < 1000)
			{
				int boxID = m_studyScene->GetEmptyBoxID();
				if (boxID >= 0)
				{
					Point boxPoint = m_studyScene->getBoxPoint(boxID);
					pLayer->replaceToPoint(boxPoint);

					m_studyScene->m_wordQueue[boxID] = pLayer;
					pLayer->m_lastPlacedAnswerBoxID = boxID;

					return;
				}

			}


		}

	}


	// 되돌려놓기
	// 마지막 있었던 배열을 빈문자열로 채운다.
	if (pLayer->m_lastPlacedAnswerBoxID >= 0)
	{
		m_studyScene->m_wordQueue[pLayer->m_lastPlacedAnswerBoxID] = WordFactory::Instance()->GetEmptyLayer();
	}

	pLayer->replaceToGenPlace();

}

void TouchedHandleLayer::OnEnter()
{
	_touchListener = EventListenerTouchOneByOne::create();
	_touchListener->setSwallowTouches(true);
	
	_touchListener->onTouchBegan		= CC_CALLBACK_2(TouchedHandleLayer::onTouchBegan, this);
	_touchListener->onTouchMoved		= CC_CALLBACK_2(TouchedHandleLayer::onTouchMoved, this);
	_touchListener->onTouchCancelled	= CC_CALLBACK_2(TouchedHandleLayer::onTouchCancelled, this);
	_touchListener->onTouchEnded		= CC_CALLBACK_2(TouchedHandleLayer::onTouchEnded, this);

	EventDispatcher* dispatcher = Director::getInstance()->getEventDispatcher();
	dispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

}


bool TouchedHandleLayer::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (m_studyScene->m_isSuccessed == true) return false;
	if (m_touchedHandlerLayer ) return false;

	Point location = touch->getLocation();


	// 텍스트버튼이 터치되었는지 검사한다.
	TextLayer* touchedLayer = GetTouchedLayer(location);
	if (touchedLayer)
	{
		touchedLayer->m_touchBeganTime = timeGetTimeEx();

		SoundFactory* soundFactory = SoundFactory::Instance();
		soundFactory->play(SOUND_FILE_tick_effect);
				
		auto director = Director::getInstance();
		auto glview = director->getOpenGLView();
		auto frameSize = glview->getFrameSize();
		const int sizeOfPopFont = frameSize.width*0.12f;		
		Label* hanWord = Label::createWithSystemFont(touchedLayer->m_textStr, "Arial", sizeOfPopFont);
		hanWord->setColor(Color3B(255, 255, 0));
		hanWord->setAnchorPoint(Point::ZERO);		

		Sprite* popupBG = Sprite::create("UI4HD/pop-hd.png");
		popupBG->addChild(hanWord, 1);
		popupBG->setAnchorPoint(Point::ZERO);

		const Point posOfPopFont = Point(popupBG->getContentSize().width*0.1f, popupBG->getContentSize().height*0.2f);
		const Point posOfPopBG = Point(popupBG->getContentSize().width*0.0f, popupBG->getContentSize().width*0.6f);
		hanWord->setPosition(posOfPopFont);


		popupBG->setPosition(posOfPopBG);
		

		touchedLayer->m_backGround->addChild(popupBG, kGameSceneTagPopup, kGameSceneTagPopup);
		touchedLayer->m_studyScene->reorderChild(touchedLayer, kGameSceneTagTouchedBtn);

	}
	else 
	{		
		m_studyScene->PlayWordSound();
	}


	m_touchedHandlerLayer = touchedLayer;
	return true;
}

void TouchedHandleLayer::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (m_studyScene->m_isSuccessed == false)  return;
	if (m_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	TextLayer* touchedTextLayer = m_touchedHandlerLayer;	
	touchedTextLayer->replaceToPoint(location);

	return;
}

void TouchedHandleLayer::onTouchCancelled(Touch* touch, Event* unused_event)
{
	return;
}

void TouchedHandleLayer::onTouchEnded(Touch* touch, Event *unused_event)
{
	if (m_studyScene->m_isSuccessed == false)  return;
	if (m_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	this->ReplaceLayerToAnswerBox(m_touchedHandlerLayer, location);
	m_touchedHandlerLayer->m_backGround->removeChildByTag(kGameSceneTagPopup, true);

	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_click_effect);


	// 단어가 정확히 맞으면 통과시킴
	if (m_studyScene->checkWord())
	{
		m_studyScene->m_isSuccessed = true;
		m_studyScene->ChangeEmotion(EMOTION_ID_HAPPY);
		// 씬 넘기기	
		m_studyScene->OnPassed();
	}

	m_touchedHandlerLayer = NULL;


	return;
}