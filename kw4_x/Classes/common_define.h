#pragma once
#include <stdlib.h>
#include "cocos2d.h"
using namespace cocos2d;

// LITE_VER: 무료(라이트) 버전 표시. 기본은 라이트로 두고,
// 유료(정식) 빌드 타깃만 KW4_PAID 를 정의해 라이트 기능을 끈다(iOS CMake: kw4 타깃).
#ifndef KW4_PAID
#define LITE_VER // LITE version
#endif
//#define TEST_MODE

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
#else
#define ckConsumableBaseFeatureId "com.ozzywow.kw4iphonelite"
#define ckProductIdStep2 "com.ozzywow.kw4iphonelite.step2"
#define ckProductIdStep3 "com.ozzywow.kw4iphonelite.step3"
#define ckProductIdStep4 "com.ozzywow.kw4iphonelite.step4"
#define ckProductIdStep5 "com.ozzywow.kw4iphonelite.step5"
#define ckProductIdTotal "com.ozzywow.kw4iphonelite.total"
#endif

// 유료 정식버전(별도 앱)의 App Store 페이지. 메인메뉴 배너에서만 사용하며 iOS 전용이다.
// itms-apps:// 스킴은 Safari 우회 없이 App Store 앱을 직접 연다 (https:// 는 무반응 실패 가능).
#if CC_TARGET_PLATFORM == CC_PLATFORM_IOS
#define BUY_AT_STORE_URL "itms-apps://itunes.apple.com/app/id509909625"
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
	// 우선 랜덤으로 빈 슬롯을 찾는다.
	for (int i = 0; i< 100; ++i) {
		int ran = rand() % max;
		if (arrRandFlag[ran] == false)
		{
			arrRandFlag[ran] = true;
			return ran;
		}
	}
	// 랜덤 시도가 모두 실패해도(빈 슬롯이 거의 남지 않았을 때) 선형 탐색으로 확실히 찾는다.
	// 이 보정이 없으면 -1 이 반환되어 호출부에서 배열 범위 밖 접근(크래시) 위험이 있다.
	for (int i = 0; i < max; ++i) {
		if (arrRandFlag[i] == false)
		{
			arrRandFlag[i] = true;
			return i;
		}
	}
	return -1; // 모든 슬롯이 사용됨(요청이 슬롯 수를 초과한 경우)
}



// fontPath 기본값은 arial.ttf(한글 글리프 없음). 한글 문자열은 KR_FONT_TTF(malgun) 등을 넘겨야 한다.
static void PrintStyle(Node* parent, std::string& str, int fontSize, Point pos, const char* fontPath = "fonts/arial.ttf")
{
	auto label0 = Label::createWithTTF(str, fontPath, fontSize);
	label0->setPosition(pos.x - 1, pos.y);
	label0->setColor(Color3B::BLACK);
	parent->addChild(label0, kGameSceneTagAnswerText);

	auto label1 = Label::createWithTTF(str, fontPath, fontSize);
	label1->setPosition(pos.x + 1, pos.y);
	label1->setColor(Color3B::BLACK);
	parent->addChild(label1, kGameSceneTagAnswerText);

	auto label2 = Label::createWithTTF(str, fontPath, fontSize);
	label2->setPosition(pos.x, pos.y - 1);
	label2->setColor(Color3B::BLACK);
	parent->addChild(label2, kGameSceneTagAnswerText);

	auto label3 = Label::createWithTTF(str, fontPath, fontSize);
	label3->setPosition(pos.x, pos.y + 1);
	label3->setColor(Color3B::BLACK);
	parent->addChild(label3, kGameSceneTagAnswerText);

	auto label = Label::createWithTTF(str, fontPath, fontSize);
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
