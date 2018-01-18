
#include "stdafx.h"
#include "AppDelegate.h"
#include "HelloWorldScene.h"
#include "MainMenuScene.h"
#include "PointManager.h"
#include "MKStoreManager_cpp.h"


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


typedef struct tagResource
{
    cocos2d::CCSize size;
    char directory[128];
} Resource;

static Resource			resource1 = { cocos2d::Size(640, 960), "iphone4" };
static Resource			resource2 = { cocos2d::Size(640, 1136), "iphoneSE5678" };
static Resource			resource3 = { cocos2d::Size(1536/2, 2048/2), "ipad" };
static Resource			resource4 = { cocos2d::Size(960, 1704), "iphone6p" };
static Resource			resource5 = { cocos2d::Size(1125/2, 2436/2), "iphoneX" };



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

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};
    
    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("kw4_iphone", cocos2d::Rect(0, 0, resource3.size.width, resource3.size.height));
#else
        glview = GLViewImpl::create("kw4_iphone");
#endif
        director->setOpenGLView(glview);
    }

	
	    
    // turn on display FPS
    director->setDisplayStats(false);
    
    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);
    
    
	auto frame_size = glview->getDesignResolutionSize();
	float sourceRate = designResolutionSize.height / designResolutionSize.width;
	float targetRate = frame_size.height / frame_size.width;
	if (sourceRate < targetRate)
	{
		// Set the design resolution
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::FIXED_WIDTH);		
	}
	else
	{
		// Set the design resolution
		glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::SHOW_ALL);
	}
    
    
	


    
    // 데모에서는, 우리는 프레임의 세로길이에 따라 리소스를 선택한다.
    // 만약 리소스 사이즈가 디자인 해상도사이즈로 부터 다르다면, 개발자는 contentScaleFactor 설정이 필요하다.
    // 우리는 디자인 해상도의 세로길이와 리소스 새로길이의 비율을 사용한다.
    // 이부분은 디자인 해상도의 세로길이에 리소스의 세로길이를 맞출수 있다.
    
    /*
     auto frameSize = glview->getFrameSize();
     // if the frame's height is larger than the height of medium size.
     // 만약 프레임의 세로길이가 중간 리소스 사이즈의 세로길이보다 크다면,큰 리소스를 선택한다.
     if (frameSize.height > mediumResolutionSize.height)
     {
     director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
     }
     // if the frame's height is larger than the height of small size.
     // 만약 프레임의 세로길이가 작은 리소스 사이즈의 길이보다 크다면, 중간 사이즈 리소스를 선택한다.
     else if (frameSize.height > smallResolutionSize.height)
     {
     director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
     }
     // if the frame's height is smaller than the height of medium size.
     // 만약 프레임의 세로길이가 중간 리소스사이즈의 세로길이보다 작다면, 작은 리소스를 선택한다.
     else
     {
     director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
     }
     */
    
    //director->setContentScaleFactor(1.0);
    
    
    register_all_packages();
    

	PointManager::Instance()->LoadXML();

    // create a scene. it's an autorelease object
    //auto scene = HelloWorld::createScene();	
    auto scene = MainMenuScene::createScene();    
    // run
    director->runWithScene(scene);
    

#ifdef LITE_VER
    CMKStoreManager::Instance()->restorePreviousTransactions();
    if( CMKStoreManager::Instance()->isFeaturePurchased(ckProductIdTotal) )
    {
        PointManager::Instance()->SetCartWithPID(PID_STEP2, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP3, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP4, true);
        PointManager::Instance()->SetCartWithPID(PID_STEP5, true);
        PointManager::Instance()->SetCartWithPID(PID_TOTAL, true);
    }
    else
    {
        PointManager::Instance()->SetCartWithPID(PID_TOTAL, false);
        
        if(CMKStoreManager::Instance()->isFeaturePurchased(ckProductIdStep2))
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP2, true);
        }
        else
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP2, false);
        }
        
        if(CMKStoreManager::Instance()->isFeaturePurchased(ckProductIdStep3))
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP3, true);
        }
        else
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP3, false);
        }
        
        if(CMKStoreManager::Instance()->isFeaturePurchased(ckProductIdStep4))
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP4, true);
        }
        else
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP4, false);
        }
        
        if(CMKStoreManager::Instance()->isFeaturePurchased(ckProductIdStep5))
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP5, true);
        }
        else
        {
            PointManager::Instance()->SetCartWithPID(PID_STEP5, false);
        }
    }
#endif //LITE_VER
    
    return true;
}

// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();
    
#if USE_AUDIO_ENGINE
    AudioEngine::pauseAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
    SimpleAudioEngine::getInstance()->pauseAllEffects();
#endif
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();
    
#if USE_AUDIO_ENGINE
    AudioEngine::resumeAll();
#elif USE_SIMPLE_AUDIO_ENGINE
    SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
    SimpleAudioEngine::getInstance()->resumeAllEffects();
#endif
}
