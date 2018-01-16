#ifndef _MKSTOREMANAGER_MM_H
#define _MKSTOREMANAGER_MM_H

#include "Singleton.h"

class MKStoreManagerDelegate;
class iosLink_MKStoreManager
{
public:
	static bool isFeaturePurchased(std::string featureId);
	static void buyFeature(std::string featureId);
	static void setDelegate(MKStoreManagerDelegate* delegate);
};


class UIView;
class UIActivityIndicatorView;
class UIViewController;
class iosUI : public Singleton<iosUI>
{
public:
	iosUI() {};
	~iosUI() {};

	UIView*		m_rootView;
	UIActivityIndicatorView* m_activity;

	UIViewController* getRootViewController();

	void	ToggleIndicator(bool lock);

};


@protocol MKStoreKitDelegate <NSObject>
@optional
- (void)productFetchComplete;
-(void)productPurchased:(NSString *)productId;
-(void)transactionCanceled;
// as a matter of UX, don't show a "User Canceled transaction" alert view here
// use this only to "enable/disable your UI or hide your activity indicator view etc.,
@end

@interface InterfaceMKStoreKitDelegate : NSObject<MKStoreKitDelegate>

-(void)productFetchComplete;
-(void)productPurchased:(NSString *)productId;
-(void)transactionCanceled;
@end
#endif //_MKSTOREMANAGER_MM_H
