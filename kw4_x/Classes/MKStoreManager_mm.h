#ifndef _MKSTOREMANAGER_MM_H
#define _MKSTOREMANAGER_MM_H

class iosLink_MKStoreManager
{
public:
	static bool isFeaturePurchased(std::string featureId);
	static void buyFeature(std::string featureId);
};
#endif //_MKSTOREMANAGER_MM_H
