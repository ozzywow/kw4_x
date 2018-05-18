#pragma once

#include "cocos2d.h"
#include "MKStoreManagerDelegate.h"

using namespace cocos2d;


class MainMenuScene : public Scene, public MKStoreManagerDelegate
{
public:
	static Scene* createScene()
	{ 
		return MainMenuScene::create();  
	}
	CREATE_FUNC(MainMenuScene);

	MainMenuScene();
	~MainMenuScene();

	virtual bool init();

	void callbackOnPushedNewGameMenuItem(Ref* pSender);
	void callbackOnPushedContinueMenuItem(Ref* pSender);
	void callbackOnPushedControlGameMenuItem(Ref* pSender);
	void callbackOnPushedAppleTreeGameMenuItem(Ref* pSender);
	void callbackOnPushedBuyGameMenuItem(Ref* pSender);
	void cfFullVersion(Ref* pSender);

	virtual void productFetchComplete();
	virtual void productPurchased(std::string productId);
	virtual void transactionCanceled();
	virtual void restorePreviousTransactions(int count);

};
