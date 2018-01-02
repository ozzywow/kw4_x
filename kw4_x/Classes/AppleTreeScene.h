#pragma once

#include <vector>
#include "cocos2d.h"
using namespace cocos2d;

class AppleTreeScene : public Scene
{
public:
	bool m_isPlay;

	int						_touchedLayerID;
	Sprite*					_touchedHandlerLayer;
		

	std::list<Sprite*>		m_arrSprites;

	cocos2d::Size			frameSize;

	AppleTreeScene();
	~AppleTreeScene();

	static Scene* createScene(bool isPlay)
	{
		AppleTreeScene* pRes = AppleTreeScene::create();
		pRes->initWithVal(isPlay);
		return pRes;
	}
	CREATE_FUNC(AppleTreeScene);


	virtual bool init();
	bool initWithVal(bool isPlay);

	Sprite* ChangeToFly(int laverID);
	void	RemoveApple(int appleID);
	Sprite* GetAppleSprite(int appleID);
	void	ChangeBiteCount(int ID);

	void	callbackOnPushedHomeMenuItem(Ref* pSender);
	void	callbackOnPushedNewGameMenuItem(Ref* pSender);
	void	callbackOnPushedBuyMenuItem(Ref* pSender);
	
	Sprite* GetTouchedLayer(Point location);
	virtual bool onTouchBegan(Touch* touch, Event* unused_event);
	virtual void onTouchMoved(Touch* touch, Event* unused_event);
	virtual void onTouchCancelled(Touch* touch, Event* unused_event);
	virtual void onTouchEnded(Touch* touch, Event *unused_event);
	
	
};
