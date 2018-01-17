#pragma once
#include <stdlib.h>
#include "cocos2d.h"
using namespace cocos2d;

#define LITE_VER // LITE version

#define MAX_SIZE_OF_CHARACTER_POOL  128

#define FRAME_HEIGHT				960.0f
#define FRAME_WIDTH					640.0f


#define ckConsumableBaseFeatureId "com.ozzywow.kw4iphonelite"
#define ckProductIdStep2 "com.ozzywow.kw4iphonelite.step2"
#define ckProductIdStep3 "com.ozzywow.kw4iphonelite.step3"
#define ckProductIdStep4 "com.ozzywow.kw4iphonelite.step4"
#define ckProductIdStep5 "com.ozzywow.kw4iphonelite.step5"
#define ckProductIdTotal "com.ozzywow.kw4iphonelite.total"


enum 
{
	kGameSceneTagImg,
	kGameSceneTagBackground,
	kGameSceneTagTouchHandlingLayer,
	kGameSceneTagTextBtn,
	kGameSecceTagHintLayer,
	kGameSceneTagFuncBtn = kGameSecceTagHintLayer + 10,
	kGameSceneTagAnswerShadow,
	kGameSceneTagAnswerText,
	kGameSceneTagAnswerFrame,
	kGameSceneTagAvatar,
	kGameSceneTagStar,
	kGameSceneTagAppleSpecial,
	kGameSceneTagApplePoint,
	kGameSceneTagTouchedBtn = kGameSceneTagApplePoint + 10,
	kGameSceneTagPopup,
	kGameSceneTagEffect,
	kGameSceneTagGoodjob,
	kGameSceneTagWatch,
};


//static int MAX_WORD_NUM = 8;
static bool arrRandFlag[8];

static void InitRandNum()
{
	memset(arrRandFlag, 0, sizeof(arrRandFlag));
}

static int GetRandNum(int max = 8)
{
	for (int i = 0; i< 100; ++i) {
		int ran = rand() % max;
		if (arrRandFlag[ran] == false)
		{
			arrRandFlag[ran] = true;
			return ran;
		}
	}
	return -1;
}



static void PrintStyle(Node* parent, std::string& str, int fontSize, Point pos)
{
	auto label0 = Label::createWithSystemFont(str, "Arial", fontSize);
	label0->setPosition(pos.x - 1, pos.y);
	label0->setColor(Color3B::BLACK);
	parent->addChild(label0, kGameSceneTagAnswerText);

	auto label1 = Label::createWithSystemFont(str, "Arial", fontSize);
	label1->setPosition(pos.x + 1, pos.y);
	label1->setColor(Color3B::BLACK);
	parent->addChild(label1, kGameSceneTagAnswerText);

	auto label2 = Label::createWithSystemFont(str, "Arial", fontSize);
	label2->setPosition(pos.x, pos.y - 1);
	label2->setColor(Color3B::BLACK);
	parent->addChild(label2, kGameSceneTagAnswerText);

	auto label3 = Label::createWithSystemFont(str, "Arial", fontSize);
	label3->setPosition(pos.x, pos.y + 1);
	label3->setColor(Color3B::BLACK);
	parent->addChild(label3, kGameSceneTagAnswerText);

	auto label = Label::createWithSystemFont(str, "Arial", fontSize);
	label->setPosition(pos.x, pos.y + 1);
	label->setColor(Color3B::WHITE);
	parent->addChild(label, kGameSceneTagAnswerText);
}


static std::string replace_all(
	const std::string &message,
	const std::string &pattern,
	const std::string &replace
) {

	std::string result = message;
	std::string::size_type pos = 0;
	std::string::size_type offset = 0;

	while ((pos = result.find(pattern, offset)) != std::string::npos)
	{
		result.replace(result.begin() + pos, result.begin() + pos + pattern.size(), replace);
		offset = pos + replace.size();
	}

	return result;
}



static inline unsigned long timeGetTimeEx()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
