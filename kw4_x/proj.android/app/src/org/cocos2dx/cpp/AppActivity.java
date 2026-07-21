package org.cocos2dx.cpp;

import android.os.Bundle;
import android.view.View;
import android.view.Window;
import android.view.WindowManager;

import org.cocos2dx.lib.Cocos2dxActivity;

public class AppActivity extends Cocos2dxActivity {

    private static AppActivity instance;
    private static BillingManager billingManager;

    public static void purchaseProduct(final String productId) {
        instance.runOnUiThread(() -> {
            if (billingManager != null) billingManager.purchase(productId);
        });
    }

    public static void restorePurchases() {
        instance.runOnUiThread(() -> {
            if (billingManager != null) billingManager.restorePurchases();
        });
    }

    public static boolean isProductPurchased(String productId) {
        return billingManager != null && billingManager.isFeaturePurchased(productId);
    }

    // C++에서 JNI로 호출 — 앱 소개 페이지 링크를 공유 시트로 내보낸다
    public static void shareText(final String text) {
        if (instance == null) return;
        instance.runOnUiThread(() -> {
            android.content.Intent i = new android.content.Intent(android.content.Intent.ACTION_SEND);
            i.setType("text/plain");
            i.putExtra(android.content.Intent.EXTRA_TEXT, text);
            instance.startActivity(android.content.Intent.createChooser(i, null));
        });
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        instance = this;
        // GL 서피스가 생성되기 전에 전체화면 플래그를 적용하여
        // 네비게이션 바 높이가 H_OFFSET 계산에 영향을 주지 않도록 한다.
        requestWindowFeature(Window.FEATURE_NO_TITLE);
        getWindow().setFlags(
            WindowManager.LayoutParams.FLAG_FULLSCREEN,
            WindowManager.LayoutParams.FLAG_FULLSCREEN
        );
        getWindow().getDecorView().setSystemUiVisibility(
            View.SYSTEM_UI_FLAG_LAYOUT_STABLE
            | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
            | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
            | View.SYSTEM_UI_FLAG_FULLSCREEN
            | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
        );

        super.onCreate(savedInstanceState);
        billingManager = new BillingManager(this);
    }

    @Override
    public void onWindowFocusChanged(boolean hasFocus) {
        super.onWindowFocusChanged(hasFocus);
        if (hasFocus) {
            getWindow().getDecorView().setSystemUiVisibility(
                View.SYSTEM_UI_FLAG_LAYOUT_STABLE
                | View.SYSTEM_UI_FLAG_LAYOUT_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_LAYOUT_FULLSCREEN
                | View.SYSTEM_UI_FLAG_HIDE_NAVIGATION
                | View.SYSTEM_UI_FLAG_FULLSCREEN
                | View.SYSTEM_UI_FLAG_IMMERSIVE_STICKY
            );
        }
    }
}
