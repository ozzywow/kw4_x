#pragma once

#include "cocos2d.h"
#include "StudyScene.h"
#include "TextLayer.h"

class TouchedHandleLayer : public Layer
{
private:
	StudyScene*		m_studyScene;
	TextLayer*		m_touchedHandlerLayer;


	EventListenerTouchOneByOne* _touchListener;


public:
	TouchedHandleLayer();
	~TouchedHandleLayer();

	static TouchedHandleLayer* create(StudyScene* studyScene);
	bool initWithVal(StudyScene *studyScene) 
	{ 
		m_studyScene = studyScene; 
		m_touchedHandlerLayer = NULL;
		return true;
	}

	TextLayer* GetTouchedLayer(Point location);
	void ReplaceLayerToAnswerBox(TextLayer* pLayer, Point location);

	void OnEnter();

	virtual bool onTouchBegan(Touch* touch, Event* unused_event); 
	virtual void onTouchMoved(Touch* touch, Event* unused_event); 
	virtual void onTouchCancelled(Touch* touch, Event* unused_event); 
	virtual void onTouchEnded(Touch* touch, Event *unused_event);


};
