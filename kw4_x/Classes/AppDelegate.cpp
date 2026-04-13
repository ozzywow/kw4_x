
#include "stdafx.h"
#include "AppDelegate.h"
#include "MainMenuScene.h"
#include "PointManager.h"


// #define USE_AUDIO_ENGINE 1
// #define USE_SIMPLE_AUDIO_ENGINE 1

#if USE_AUDIO_ENGINE && USE_SIMPLE_AUDIO_ENGINE
#error "Don't use AudioEngine and SimpleAudioEngine at the same time. Please just select one in your game!"
#endif

#if USE_AUDIO_ENGINE
#include "audio/include/AudioEngine.h"
using namespace cocos2d::experimental;
#elif USE_SIMPLE_AUDIO_ENGINE
#include "audio/include/SimpleAudioEngine.h"
using namespace CocosDenshion;
#endif


// Design: FIXED_WIDTH=640, height adapts. H_OFFSET centers the 640x960 game area.
// iPhone SE~17, iPad mini~Pro 12.9" all supported. On iPad H_OFFSET may be negative (slight clip).

static cocos2d::Size designResolutionSize = cocos2d::Size(FRAME_WIDTH, FRAME_HEIGHT);


USING_NS_CC;


AppDelegate::AppDelegate()
{
}

AppDelegate::~AppDelegate()
{
#if USE_AUDIO_ENGINE
    AudioEngine::end();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::end();
#endif
}

void AppDelegate::initGLContextAttrs()
{
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    GLView::setGLContextAttrs(glContextAttrs);
}

static int register_all_packages()
{
    return 0;
}

bool AppDelegate::applicationDidFinishLaunching() {
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("kw4_iphone", cocos2d::Rect(0, 0, s_windowWidth, s_windowHeight));
#else
        glview = GLViewImpl::create("kw4_iphone");
#endif
        director->setOpenGLView(glview);
    }

    director->setDisplayStats(false);
    director->setAnimationInterval(1.0f / 60);

    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);

    register_all_packages();

    // 크로스플랫폼 리소스 경로 설정
    auto fileUtils = FileUtils::getInstance();
    std::vector<std::string> searchPaths;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32)
    // Windows: 실행파일 디렉토리와 빌드 구조 기반 경로 검색
    searchPaths.push_back("Resources/");
    searchPaths.push_back("../Resources/");

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
    // iOS: 앱 번들 내 Resources (자동으로 처리되지만 명시적으로 설정)
    searchPaths.push_back("Resources/");

#elif (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
    // Mac: Contents/Resources 경로 (앱 번들 구조)
    searchPaths.push_back("Resources/");
    searchPaths.push_back("../Resources/");
#endif

    fileUtils->setSearchPaths(searchPaths);

    PointManager::Instance()->LoadXML();

    auto scene = MainMenuScene::createScene();
    director->runWithScene(scene);

    return true;
}

void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
