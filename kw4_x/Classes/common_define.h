#pragma once
#include <stdlib.h>
#include "cocos2d.h"
using namespace cocos2d;

#define LITE_VER // LITE version
//#define TEST_MODE
//#define IPAD_VER // IPAD version

#define MAX_SIZE_OF_CHARACTER_POOL  64

#define FRAME_HEIGHT				960.0f
#define FRAME_WIDTH					640.0f

// ACTIVE_HEIGHT: actual usable vertical space for this device.
// On tall phones (iPhone 5~17): equals FRAME_HEIGHT (960) — H_OFFSET pads top/bottom.
// On wide-ratio iPads (iPad Pro 13"): design height < 960 — UI compresses to fit.
// Use ACTIVE_HEIGHT instead of FRAME_HEIGHT for all Y-position ratios.
// Use H_OFFSET as the Y baseline (always >= 0).
static inline float CalcActiveHeight(float designHeight)
{
    return designHeight < FRAME_HEIGHT ? designHeight : FRAME_HEIGHT;
}
static inline float CalcHOffset(float designHeight)
{
    return (designHeight - CalcActiveHeight(designHeight)) * 0.5f;
}
static inline float CalcCenterX()
{
    return FRAME_WIDTH * 0.5f;
}
static inline float CalcCenterY()
{
    float h = Director::getInstance()->getOpenGLView()->getDesignResolutionSize().height;
    return h * 0.5f;
}


#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#define ckConsumableBaseFeatureId "com.ozzywow.kw4android"
#define ckProductIdStep2 "com.ozzywow.kw4android.total"
#define ckProductIdStep3 "com.ozzywow.kw4android.total"
#define ckProductIdStep4 "com.ozzywow.kw4android.total"
#define ckProductIdStep5 "com.ozzywow.kw4android.total"
#define ckProductIdTotal "com.ozzywow.kw4android.total"
#elif defined(IPAD_VER)
#define ckConsumableBaseFeatureId "com.ozzywow.kw4ipadlite"
#define ckProductIdStep2 "com.ozzywow.kw4ipadlite.step2"
#define ckProductIdStep3 "com.ozzywow.kw4ipadlite.step3"
#define ckProductIdStep4 "com.ozzywow.kw4ipadlite.step4"
#define ckProductIdStep5 "com.ozzywow.kw4ipadlite.step5"
#define ckProductIdTotal "com.ozzywow.kw4ipadlite.total"
#else
#define ckConsumableBaseFeatureId "com.ozzywow.kw4iphonelite"
#define ckProductIdStep2 "com.ozzywow.kw4iphonelite.step2"
#define ckProductIdStep3 "com.ozzywow.kw4iphonelite.step3"
#define ckProductIdStep4 "com.ozzywow.kw4iphonelite.step4"
#define ckProductIdStep5 "com.ozzywow.kw4iphonelite.step5"
#define ckProductIdTotal "com.ozzywow.kw4iphonelite.total"
#endif

// 유료 정식버전(별도 앱)의 App Store 페이지. 메인메뉴 배너에서만 사용하며 iOS 전용이다.
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define BUY_AT_STORE_URL "https://apps.apple.com/app/id509909625"
#endif

// 앱 소개 페이지. 메인메뉴 공유 버튼이 이 링크를 내보낸다.
#define SHARE_URL "https://ozzywow.github.io/kw4_x/"


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
	auto label0 = Label::createWithTTF(str, "fonts/arial.ttf", fontSize);
	label0->setPosition(pos.x - 1, pos.y);
	label0->setColor(Color3B::BLACK);
	parent->addChild(label0, kGameSceneTagAnswerText);

	auto label1 = Label::createWithTTF(str, "fonts/arial.ttf", fontSize);
	label1->setPosition(pos.x + 1, pos.y);
	label1->setColor(Color3B::BLACK);
	parent->addChild(label1, kGameSceneTagAnswerText);

	auto label2 = Label::createWithTTF(str, "fonts/arial.ttf", fontSize);
	label2->setPosition(pos.x, pos.y - 1);
	label2->setColor(Color3B::BLACK);
	parent->addChild(label2, kGameSceneTagAnswerText);

	auto label3 = Label::createWithTTF(str, "fonts/arial.ttf", fontSize);
	label3->setPosition(pos.x, pos.y + 1);
	label3->setColor(Color3B::BLACK);
	parent->addChild(label3, kGameSceneTagAnswerText);

	auto label = Label::createWithTTF(str, "fonts/arial.ttf", fontSize);
	label->setPosition(pos.x, pos.y);
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



#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
static inline unsigned long timeGetTimeEx()
{
	return (unsigned long)GetTickCount();
}
#else
static inline unsigned long timeGetTimeEx()
{
	struct timeval tv;
	gettimeofday(&tv, 0);
	return ((tv.tv_sec * 1000) + (tv.tv_usec / 1000));
}
#endif
