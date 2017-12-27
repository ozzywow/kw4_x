#pragma once

#include "cocos2d.h"
using namespace cocos2d;

class AppleTreeScene;
class AppleTouchedHandleLayer : public EventListenerTouchOneByOne
{
public:
	AppleTreeScene*			_appleTreeScene;

	int						_touchedLayerID;
	Sprite*					_touchedHandlerLayer;


	AppleTouchedHandleLayer();
	~AppleTouchedHandleLayer();

	virtual bool init()
	{
		_appleTreeScene = NULL;
		_touchedLayerID = -1;
		_touchedHandlerLayer = NULL;
		return true;
	}

	static AppleTouchedHandleLayer* createAppleTouchedHandleLayer(AppleTreeScene* p)
	{
		AppleTouchedHandleLayer* pRes = AppleTouchedHandleLayer::create();
		pRes->_appleTreeScene = p;
		return pRes;
	}
	CREATE_FUNC(AppleTouchedHandleLayer);

	Sprite* GetTouchedLayer(Point location);

	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch* touch, Event* unused_event);
	virtual void onTouchCancelled(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event *unused_event);

};