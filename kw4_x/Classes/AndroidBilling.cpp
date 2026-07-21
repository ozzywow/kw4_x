#include "cocos2d.h"

#if CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID

#include "AndroidBilling.h"
#include "MKStoreManagerDelegate.h"
#include "platform/android/jni/JniHelper.h"
#include <jni.h>

#define APPACTIVITY_CLASS "org/cocos2dx/cpp/AppActivity"

static MKStoreManagerDelegate* s_delegate = nullptr;

void AndroidBilling_setDelegate(MKStoreManagerDelegate* delegate) {
    s_delegate = delegate;
}

bool AndroidBilling_isFeaturePurchased(const std::string& productId) {
    cocos2d::JniMethodInfo info;
    if (!cocos2d::JniHelper::getStaticMethodInfo(info, APPACTIVITY_CLASS,
            "isProductPurchased", "(Ljava/lang/String;)Z"))
        return false;

    jstring jId = info.env->NewStringUTF(productId.c_str());
    jboolean result = info.env->CallStaticBooleanMethod(info.classID, info.methodID, jId);
    info.env->DeleteLocalRef(jId);
    info.env->DeleteLocalRef(info.classID);
    return (bool)result;
}

void AndroidBilling_buyFeature(const std::string& productId) {
    cocos2d::log("[Billing] AndroidBilling_buyFeature -> %s", productId.c_str());
    cocos2d::JniMethodInfo info;
    if (!cocos2d::JniHelper::getStaticMethodInfo(info, APPACTIVITY_CLASS,
            "purchaseProduct", "(Ljava/lang/String;)V")) {
        cocos2d::log("[Billing] ERROR: purchaseProduct method not found (JNI)");
        return;
    }

    jstring jId = info.env->NewStringUTF(productId.c_str());
    info.env->CallStaticVoidMethod(info.classID, info.methodID, jId);
    info.env->DeleteLocalRef(jId);
    info.env->DeleteLocalRef(info.classID);
}

void AndroidBilling_restorePurchases() {
    cocos2d::log("[Billing] AndroidBilling_restorePurchases");
    cocos2d::JniMethodInfo info;
    if (!cocos2d::JniHelper::getStaticMethodInfo(info, APPACTIVITY_CLASS,
            "restorePurchases", "()V")) {
        cocos2d::log("[Billing] ERROR: restorePurchases method not found (JNI)");
        return;
    }

    info.env->CallStaticVoidMethod(info.classID, info.methodID);
    info.env->DeleteLocalRef(info.classID);
}

// JNI callbacks from BillingManager.java
extern "C" {

JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BillingManager_nativeOnPurchaseSuccess(
        JNIEnv* env, jobject thiz, jstring jProductId) {
    const char* productId = env->GetStringUTFChars(jProductId, nullptr);
    std::string pid(productId);
    env->ReleaseStringUTFChars(jProductId, productId);
    cocos2d::log("[Billing] nativeOnPurchaseSuccess <- %s (delegate=%p)", pid.c_str(), (void*)s_delegate);
    // BillingClient callbacks arrive on the Android UI thread; hop to the
    // cocos2d GL thread before touching scene nodes.
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([pid]() {
        if (s_delegate) {
            s_delegate->productPurchased(pid);
        }
    });
}

JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BillingManager_nativeOnTransactionCanceled(
        JNIEnv* env, jobject thiz) {
    cocos2d::log("[Billing] nativeOnTransactionCanceled (delegate=%p)", (void*)s_delegate);
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([]() {
        if (s_delegate) {
            s_delegate->transactionCanceled();
        }
    });
}

JNIEXPORT void JNICALL
Java_org_cocos2dx_cpp_BillingManager_nativeOnRestoreComplete(
        JNIEnv* env, jobject thiz, jint count) {
    int c = (int)count;
    cocos2d::log("[Billing] nativeOnRestoreComplete <- count=%d (delegate=%p)", c, (void*)s_delegate);
    cocos2d::Director::getInstance()->getScheduler()->performFunctionInCocosThread([c]() {
        if (s_delegate) {
            s_delegate->restorePreviousTransactions(c);
        }
    });
}

} // extern "C"

#endif // CC_PLATFORM_ANDROID
