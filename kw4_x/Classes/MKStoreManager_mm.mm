#include <string>
#include "MKStoreManager_mm.h"
#include "MKStoreManager.h"

USING_NS_CC;

bool iosLink_MKStoreManager::isFeaturePurchased(std::string featureId)
{
    NSString* obejctiveString =  [NSString stringWithUTF8String:featureId.c_str()];
    BOOL bRes = (bool)[MKStoreManager isFeaturePurchased:obejctiveString];
    return (bool)bRes;
}

void iosLink_MKStoreManager::buyFeature(std::string featureId)
{
    NSString* obejctiveString =  [NSString stringWithUTF8String:featureId.c_str()];
	[[MKStoreManager sharedManager] buyFeature:obejctiveString];
}
