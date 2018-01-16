#ifndef _MKSTOREMANAGER_MM_H
#define _MKSTOREMANAGER_MM_H


//#include "MKStoreManager.h"
#include "Singleton.h"



@interface InterfaceMKStoreKitDelegate : NSObject<MKStoreKitDelegate>{
}

- (void)productFetchComplete;
- (void)productPurchased:(NSString *)productId;
- (void)transactionCanceled;
@end



class MKStoreManagerDelegate;
class iosLink_MKStoreManager
{
public:
	static bool isFeaturePurchased(std::string featureId);
	static void buyFeature(std::string featureId);
	static void setDelegate(MKStoreManagerDelegate* delegate);
};


//class UIView;
//class UIActivityIndicatorView;
//class UIViewController;
class iosUI : public Singleton<iosUI>
{
public:
	iosUI();
	~iosUI();

	void	ToggleIndicator(bool lock);

};


#endif //_MKSTOREMANAGER_MM_H
