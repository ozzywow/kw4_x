#include "MKStoreManager_cpp.h"
#include "MKStoreManager_mm.h"

USING_NS_CC;

CMKStoreManager::CMKStoreManager()
{

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
