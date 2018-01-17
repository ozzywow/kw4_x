#ifndef _MKSTOREMANAGER_MM_H
#define _MKSTOREMANAGER_MM_H


//#include "MKStoreManager.h"
#include "Singleton.h"


class MKStoreManagerDelegate;
class iosLink_MKStoreManager
{
public:
#ifdef __cplusplus
	static bool isFeaturePurchased(std::string featureId) { return false; };
	static void buyFeature(std::string featureId) {};
	static void setDelegate(MKStoreManagerDelegate* delegate) {};
#else
	static bool isFeaturePurchased(std::string featureId);
	static void buyFeature(std::string featureId);
	static void setDelegate(MKStoreManagerDelegate* delegate);
#endif 
};


//class UIView;
//class UIActivityIndicatorView;
//class UIViewController;
class iosUI : public Singleton<iosUI>
{
public:
#ifdef __cplusplus
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
