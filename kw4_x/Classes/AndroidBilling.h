#pragma once

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include <string>

class MKStoreManagerDelegate;

void AndroidBilling_setDelegate(MKStoreManagerDelegate* delegate);
bool AndroidBilling_isFeaturePurchased(const std::string& productId);
void AndroidBilling_buyFeature(const std::string& productId);
void AndroidBilling_restorePurchases();

#endif // CC_PLATFORM_ANDROID
