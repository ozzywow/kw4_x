#include "stdafx.h"
#include "AppleTouchedHandleLayer.h"
#include "AppleTreeScene.h"
#include "Character.h"
#include "CharacterFactory.h"
#include "SoundFactory.h"



AppleTouchedHandleLayer::AppleTouchedHandleLayer()
{
	init();
}

AppleTouchedHandleLayer::~AppleTouchedHandleLayer()
{

}

Sprite* AppleTouchedHandleLayer::GetTouchedLayer(Point location)
{
	for( auto itr = _appleTreeScene->m_arrSprites.begin(); itr != _appleTreeScene->m_arrSprites.end(); ++itr )
	{
		Sprite* touchedLayer = *itr;
		if (NULL == touchedLayer) break;

		// �׸� sprite�� �������� ���� ����մϴ�.

		float halfWidth = touchedLayer->getContentSize().width / 2.0;
		float halfHeight = touchedLayer->getContentSize().height / 2.0;


		Point pos = touchedLayer->getPosition();
		// ��ġ�� ��ġ�� �׸� �ȿ� ������ �� ����մϴ�.
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

bool AppleTouchedHandleLayer::onTouchBegan(Touch* touch, Event* unused_event)
{
	if (_touchedHandlerLayer != NULL) return false;


	Point location = touch->getLocation();	


	// �ؽ�Ʈ��ư�� ��ġ�Ǿ����� �˻��Ѵ�.
	Sprite* touchedLayer = GetTouchedLayer(location);
	if (NULL == touchedLayer)
	{
		return false;
	}
	

	_appleTreeScene->reorderChild(touchedLayer, 0);


	SoundFactory* soundFactory = SoundFactory::Instance();
	soundFactory->play(SOUND_FILE_giggle_effect);

	_touchedHandlerLayer = touchedLayer;

	return true;
}

void AppleTouchedHandleLayer::onTouchMoved(Touch* touch, Event* unused_event)
{
	if (_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	_touchedHandlerLayer->setPosition(location);

	return;
}


void AppleTouchedHandleLayer::onTouchEnded(Touch* touch, Event *unused_event)
{
	if (_touchedHandlerLayer == NULL) return;

	Point location = touch->getLocation();

	if (_touchedLayerID >= 0)
	{
		Character* pCharacter = (Character*)_touchedHandlerLayer->getUserData();
		if (pCharacter == NULL) return;

		// ����̸�..
		if (pCharacter->type == CT_APPLE || pCharacter->type == CT_FLY)
		{

			pCharacter->posX = (int)location.x;
			pCharacter->posY = (int)location.y;
		}
		// �ֹ�����..
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
					_touchedHandlerLayer = _appleTreeScene->ChangeToFly(_touchedLayerID);
				}
				else
				{
					pCharacter->biteCount = appleBiteCount;
					_appleTreeScene->ChangeBiteCount(_touchedLayerID);
				}

				// ����� �����.
				Character* pApple = pCharacterFactory->GetCharacterWithID(biteAppleID);
				if (pApple)
				{
					pApple->type = CT_NONE;
				}
				_appleTreeScene->RemoveApple(biteAppleID);

			}
			else
			{
				pCharacter->posX = (int)location.x;
				pCharacter->posY = (int)location.y;
			}

		}



		const float heightOfJump = 10.0;
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


void AppleTouchedHandleLayer::onTouchCancelled(Touch* touch, Event* unused_event)
{

}
