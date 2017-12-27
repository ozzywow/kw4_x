#pragma once


#include "cocos2d.h"
using namespace cocos2d;

class InfoScene : public Scene
{
public:

	static Scene* createScene()
	{
		return InfoScene::create();
	}
	CREATE_FUNC(InfoScene);

	InfoScene();
	~InfoScene();

	MenuItemImage* m_btnLevel1;
	MenuItemImage* m_btnLevel2;
	MenuItemImage* m_btnLevel3;
	MenuItemImage* m_btnLevel4;
	MenuItemImage* m_btnLevel5;
	MenuItemImage* m_btnLevel6;

	MenuItemImage* m_btnHintOpt;


	bool isProgress;



	virtual bool init();
	void DrawItemBox();
	void ReflushSelectedMenuItem(int selectedLevel);
	void PlayBuySound();


	// callback func
	void callbackOnPushedHomeMenuItem(Ref* sender);
	void callbackOnPushedResetMenuItem(Ref* sender);
	void popCallback_ResetOk(Ref* sender);
	void callbackOnPushedHintMenuItem(Ref* sender);

	void callbackOnPushedLevel1(Ref* sender);
	void callbackOnPushedLevel2(Ref* sender);
	void callbackOnPushedLevel3(Ref* sender);
	void callbackOnPushedLevel4(Ref* sender);
	void callbackOnPushedLevel5(Ref* sender);
	void callbackOnPushedLevel6(Ref* sender);

	void cfStep2(Ref* sender);
	void cfStep3(Ref* sender);
	void cfStep4(Ref* sender);
	void cfStep5(Ref* sender);
	void cfTotal(Ref* sender);


};