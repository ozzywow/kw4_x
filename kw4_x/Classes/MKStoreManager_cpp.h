#pragma once
#include <string>
#include "Singleton.h"

class CMKStoreManager : public Singleton<CMKStoreManager>
{
public:
	CMKStoreManager();
	~CMKStoreManager();

	// this is a static method, since it doesn't require the store manager to be initialized prior to calling
	bool isFeaturePurchased(std::string featureId);
	void buyFeature(std::string featureId);


};
