#ifndef _MKSTOREMANAGER_MM_H
#define _MKSTOREMANAGER_MM_H

// IMPORTANT: cocos2d.h (or CCPlatformConfig.h) must be included BEFORE the
// CC_TARGET_PLATFORM checks below. Without it the platform macros are
// undefined (== 0), so "CC_TARGET_PLATFORM == CC_PLATFORM_WIN32" evaluates
// true on every platform and the empty WIN32 stubs get compiled even on
// Android — silently disabling in-app purchases.
#include "platform/CCPlatformConfig.h"

//#include "MKStoreManager.h"
#include "Singleton.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
#include "AndroidBilling.h"
#endif

class MKStoreManagerDelegate;
class iosLink_MKStoreManager
{
public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
	static bool isFeaturePurchased(std::string featureId) { return false; };
	static void buyFeature(std::string featureId) {};
	static void setDelegate(MKStoreManagerDelegate* delegate) {};
	static void restorePreviousTransactions(){};
#elif CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID
	static bool isFeaturePurchased(std::string featureId) { return AndroidBilling_isFeaturePurchased(featureId); };
	static void buyFeature(std::string featureId) { AndroidBilling_buyFeature(featureId); };
	static void setDelegate(MKStoreManagerDelegate* delegate) { AndroidBilling_setDelegate(delegate); };
	static void restorePreviousTransactions() { AndroidBilling_restorePurchases(); };
#else
	static bool isFeaturePurchased(std::string featureId);
	static void buyFeature(std::string featureId);
	static void setDelegate(MKStoreManagerDelegate* delegate);
	static void restorePreviousTransactions();
#endif
};


//class UIView;
//class UIActivityIndicatorView;
//class UIViewController;
class iosUI : public Singleton<iosUI>
{
public:
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX) || (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
	iosUI() {};
	~iosUI() {};
	void	ToggleIndicator(bool lock) {};
#else
	iosUI();
	~iosUI();
	void	ToggleIndicator(bool lock);
#endif

	

};


#endif //_MKSTOREMANAGER_MM_H
