-keep public class org.cocos2dx.** { *; }
-keep public class com.ozzywow.** { *; }

-dontwarn android.os.ServiceManager
-dontwarn android.util.Slog

-keep class com.android.billingclient.** { *; }
-keep class org.cocos2dx.cpp.BillingManager { *; }
-keep class org.cocos2dx.cpp.AppActivity { *; }
