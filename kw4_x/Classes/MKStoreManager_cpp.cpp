#include "MKStoreManager_cpp.h"
#include "MKStoreManager_mm.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)
USING_NS_CC;
#endif 

CMKStoreManager::CMKStoreManager()
{
	_mkdelegate = NULL;
}

CMKStoreManager::~CMKStoreManager()
{

}

bool CMKStoreManager::isFeaturePurchased(std::string featureId)
{
    return iosLink_MKStoreManager::isFeaturePurchased(featureId);
}

void CMKStoreManager::buyFeature(std::string featureId)
{
    iosLink_MKStoreManager::buyFeature(featureId);
}

void CMKStoreManager::SetDelegate(void* delegate)
{

}

void CMKStoreManager::ToggleIndicator(bool lock)
{
	iosUI::Instance()->ToggleIndicator(lock);
}
