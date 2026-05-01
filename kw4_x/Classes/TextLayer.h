#pragma once

#include "cocos2d.h"
#include <string>

using namespace cocos2d;
class StudyScene;

class TextLayer : public Node
{
public:
	Label*			m_hanWord;
	Sprite*			m_backGround;
	StudyScene*		m_studyScene;
	Point			m_genPos;
	std::string		m_textStr;
	int				m_lastPlacedAnswerBoxID;
	long			m_touchBeganTime;


public:
	TextLayer();
	~TextLayer();

	static TextLayer* createWithWordText(StudyScene* scene, Point destPos, std::string& wordText);

	bool	initWithVal(StudyScene* scene, Point destPos, std::string& wordText);

	void	setWorldText(std::string& wordText);
	void	replaceToGenPlace();
	void	replaceToPoint(Point destPos);

	bool isPlacedInAnswerBox() { return m_lastPlacedAnswerBoxID >= 0; }
	bool isPlacedInGenPos() { return m_lastPlacedAnswerBoxID == -1; }
	bool isPlaced() { return m_lastPlacedAnswerBoxID != -1; }
	bool isEmpty() { return m_textStr.empty(); }
	bool isSameText(std::string& otherText) { return m_textStr == otherText; }	
};