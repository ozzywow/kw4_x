#include "stdafx.h"
#include "TextLayer.h"

TextLayer::TextLayer()
{	
}

TextLayer::~TextLayer()
{

}

TextLayer* TextLayer::createWithWordText(StudyScene* scene, Point destPos, std::string& wordText)
{
	auto ret = new (std::nothrow) TextLayer();
	if (ret && ret->initWithVal(scene, destPos, wordText))
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

bool TextLayer::initWithVal(StudyScene* scene, Point destPos, std::string& wordText)
{
	m_textStr = wordText;
	m_genPos = destPos;
	m_studyScene = scene;

	auto director = Director::getInstance();
	auto glview = director->getOpenGLView();	
	auto frameSize = glview->getDesignResolutionSize();

	
	m_backGround = Sprite::create("UI4HD/wordBG-hd.png");
	m_backGround->setPosition(destPos);
	this->addChild(m_backGround);

	auto size = m_backGround->getContentSize();
	m_hanWord = Label::createWithSystemFont(m_textStr, "Arial", frameSize.width*0.12f);
	m_hanWord->setAnchorPoint(Point::ANCHOR_MIDDLE);
	m_hanWord->setPosition(size.width*0.5f, size.height*0.5f);
	m_backGround->addChild(m_hanWord);
	

	m_lastPlacedAnswerBoxID = -1;
	m_touchBeganTime = timeGetTimeEx();

	return true;
}


void	TextLayer::setWorldText(std::string& wordText)
{
	m_textStr = wordText;
	m_hanWord->setString(m_textStr);
}

void	TextLayer::replaceToGenPlace()
{
	m_backGround->setPosition(m_genPos);	
	m_lastPlacedAnswerBoxID = -1;
}

void	TextLayer::replaceToPoint(Point destPos)
{	
	m_backGround->setPosition(destPos);
}