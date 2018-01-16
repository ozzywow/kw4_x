#include <string>
#include "MKStoreManager_mm.h"
#include "MKStoreManager.h"
#include "MKStoreManagerDelegate.h"

USING_NS_CC;

static MKStoreManagerDelegate* _mkdelegate;

bool iosLink_MKStoreManager::isFeaturePurchased(std::string featureId)
{
    NSString* obejctiveString =  [NSString stringWithUTF8String:featureId.c_str()];
    BOOL bRes = (bool)[MKStoreManager isFeaturePurchased:obejctiveString];
    return (bool)bRes;
}

void iosLink_MKStoreManager::buyFeature(std::string featureId, void(*fp)(std::string))
{
    NSString* obejctiveString =  [NSString stringWithUTF8String:featureId.c_str()];
	[[MKStoreManager sharedManager] buyFeature:obejctiveString];
}

void iosLink_MKStoreManager::setDelegate(MKStoreManagerDelegate* delegate)
{
	_mkdelegate = delegate;
}




iosUI::iosUI() 
{
	UIViewController* uiView = getRootViewController();
	m_rootView = uiView.view;
}

iosUI::~iosUI() 
{

}

UIViewController* iosUI::getRootViewController()
{
	return [UIApplication sharedApplication].keyWindow.rootViewController;
}


void iosUI::ToggleIndicator(bool lock)
{
	
	if (nil == m_activity)
	{
		m_activity
			= [[UIActivityIndicatorView alloc] initWithFrame:CGRectMake(320 / 2, 480 / 2, 40.0, 40.0)];

		[m_rootView addSubview : m_activity];
	}

	if (false == Lock)
		[m_activity stopAnimating];
	else
		[m_activity startAnimating];

	return;
	
}



- (void)productFetchComplete
{
	_mkdelegate->productFetchComplete();
}

- (void)productPurchased:(NSString *)productId
{    
	_mkdelegate->productPurchased(productId);
}

- (void)transactionCanceled
{
	_mkdelegate->transactionCanceled();
}