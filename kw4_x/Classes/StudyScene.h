#pragma once

#include "cocos2d.h"
using namespace cocos2d;


enum {
	EMOTION_ID_DEFUALT,
	EMOTION_ID_HAPPY,
	EMOTION_ID_QUEST,
	EMOTION_ID_SAD,
};

class TextLayer;
class StudyScene : public Scene
{
public:

	static Scene* createScene(std::string& worldName, int level, std::string& text) 
	{ 
		StudyScene* pRes = StudyScene::create();
		pRes->initVal(worldName, level, text);
		return pRes;
	}
	CREATE_FUNC(StudyScene);

	std::vector<TextLayer*>		m_arrayTextLayer;
	std::vector<TextLayer*>		m_arrayAnswerLayer;
	std::vector<TextLayer*>		m_wordQueue;
	std::string					m_wordName;
	std::string					m_fileName;
	std::string					m_text;
	bool						m_isSuccessed;
	int							m_level;

	//Audio*		m_wordSound;
	Point						arrayPoint[4];
	bool						m_isLavar;
	Sequence*					m_timeFuncAction;
	Sprite*						m_lavar;

	cocos2d::Size				frameSize;

public:
	StudyScene();
	~StudyScene();

	virtual bool init();

	void initVal(std::string& worldName, int level, std::string& text);

	Point		getBoxPoint(int index);

	void		OnPassed();
	void		OnSkip();

	void		TurnPage();
	void		GoAppleTreeScene(void);
	void		OnExitStudy();
	void		ChangeEmotion(int emotionID);
	void		DrowApple(bool showEffect, bool isRedrow);
	void		DrowStar();
	void		ShowHint();
	void		PlayWordSound();
	void		TimeRun(int sec);
	void		RemoveLavar();

	bool		checkWord();
	int			GetEmptyBoxID();

	void		popCallback_Ok(Ref* pSender);



	void callbackOnPushedPrevBtnItem(Ref* sender);
	void callbackOnPushedNextBtnItem(Ref* sender);
	void callbackOnPushedHomeBtnItem(Ref* sender);
	void callbackOnPushedHintBtnItem(Ref* sender);
	void callbackOnPushedBuyMenuItem(Ref* sender);

};