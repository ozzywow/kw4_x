#import "MKStoreManager_mm.h"
#import "MKStoreManager.h"

bool iosLink_MKStoreManager::isFeaturePurchased(std::string* featureId)
{
	return [[MKStoreManager sharedManager] isFeaturePurchased:featureId];
}

void iosLink_MKStoreManager::buyFeature(std::string* featureId)
{
	[[MKStoreManager sharedManager] buyFeature:featureId];
}