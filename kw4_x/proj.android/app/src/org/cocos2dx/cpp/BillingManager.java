package org.cocos2dx.cpp;

import android.app.Activity;
import android.util.Log;

import androidx.annotation.NonNull;

import com.android.billingclient.api.AcknowledgePurchaseParams;
import com.android.billingclient.api.BillingClient;
import com.android.billingclient.api.BillingClientStateListener;
import com.android.billingclient.api.BillingFlowParams;
import com.android.billingclient.api.BillingResult;
import com.android.billingclient.api.ProductDetails;
import com.android.billingclient.api.Purchase;
import com.android.billingclient.api.PurchasesUpdatedListener;
import com.android.billingclient.api.QueryProductDetailsParams;
import com.android.billingclient.api.QueryPurchasesParams;

import java.util.ArrayList;
import java.util.HashSet;
import java.util.List;
import java.util.Set;

public class BillingManager implements PurchasesUpdatedListener {
    private static final String TAG = "BillingManager";

    public static final String[] PRODUCT_IDS = {
        "com.ozzywow.kw4android.step2",
        "com.ozzywow.kw4android.step3",
        "com.ozzywow.kw4android.step4",
        "com.ozzywow.kw4android.step5",
        "com.ozzywow.kw4android.total"
    };

    private final Activity activity;
    private BillingClient billingClient;
    private final Set<String> purchasedProducts = new HashSet<>();
    private List<ProductDetails> productDetailsList = new ArrayList<>();

    public BillingManager(Activity activity) {
        this.activity = activity;
        billingClient = BillingClient.newBuilder(activity)
            .setListener(this)
            .enablePendingPurchases()
            .build();
        connectToStore();
    }

    private void connectToStore() {
        billingClient.startConnection(new BillingClientStateListener() {
            @Override
            public void onBillingSetupFinished(@NonNull BillingResult result) {
                Log.i(TAG, "onBillingSetupFinished: " + result.getResponseCode()
                    + " / " + result.getDebugMessage());
                if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    // Startup auto-query: populate ownership state only, do not
                    // re-fire purchase callbacks (the game persists the unlock
                    // locally). The explicit Restore button uses notify=true.
                    queryExistingPurchases(false);
                    queryProductDetails();
                }
            }

            @Override
            public void onBillingServiceDisconnected() {
                Log.w(TAG, "Billing service disconnected");
            }
        });
    }

    private void queryExistingPurchases(boolean notify) {
        billingClient.queryPurchasesAsync(
            QueryPurchasesParams.newBuilder()
                .setProductType(BillingClient.ProductType.INAPP)
                .build(),
            (result, purchases) -> {
                Log.i(TAG, "queryExistingPurchases result: " + result.getResponseCode()
                    + ", owned=" + purchases.size() + ", notify=" + notify);
                if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    for (Purchase purchase : purchases) {
                        // notify=true -> fires productPurchased() per owned item,
                        // which unlocks and persists it in the game.
                        processPurchase(purchase, notify);
                    }
                    nativeOnRestoreComplete(purchases.size());
                }
            }
        );
    }

    private void queryProductDetails() {
        List<QueryProductDetailsParams.Product> productList = new ArrayList<>();
        for (String id : PRODUCT_IDS) {
            productList.add(
                QueryProductDetailsParams.Product.newBuilder()
                    .setProductId(id)
                    .setProductType(BillingClient.ProductType.INAPP)
                    .build()
            );
        }
        billingClient.queryProductDetailsAsync(
            QueryProductDetailsParams.newBuilder().setProductList(productList).build(),
            (result, details) -> {
                Log.i(TAG, "queryProductDetails result: " + result.getResponseCode()
                    + " / " + result.getDebugMessage()
                    + ", loaded " + details.size() + " products");
                if (result.getResponseCode() == BillingClient.BillingResponseCode.OK) {
                    productDetailsList = details;
                    for (ProductDetails d : details) {
                        Log.i(TAG, "  product: " + d.getProductId());
                    }
                }
            }
        );
    }

    public void purchase(String productId) {
        Log.i(TAG, "purchase() called for " + productId
            + ", productDetailsList size=" + productDetailsList.size());

        ProductDetails target = null;
        for (ProductDetails d : productDetailsList) {
            if (d.getProductId().equals(productId)) {
                target = d;
                break;
            }
        }
        if (target == null) {
            Log.w(TAG, "Product not found: " + productId + ", retrying query. "
                + "Check that the product is ACTIVE in Play Console and the app "
                + "is published to a test track.");
            queryProductDetails();
            // Release the C++ isProgress lock so the button is not stuck.
            nativeOnTransactionCanceled();
            return;
        }

        List<BillingFlowParams.ProductDetailsParams> params = new ArrayList<>();
        params.add(BillingFlowParams.ProductDetailsParams.newBuilder()
            .setProductDetails(target)
            .build());

        BillingResult launchResult = billingClient.launchBillingFlow(activity,
            BillingFlowParams.newBuilder().setProductDetailsParamsList(params).build());
        Log.i(TAG, "launchBillingFlow result: " + launchResult.getResponseCode()
            + " / " + launchResult.getDebugMessage());
    }

    public void restorePurchases() {
        queryExistingPurchases(true);
    }

    public boolean isFeaturePurchased(String productId) {
        return purchasedProducts.contains(productId);
    }

    @Override
    public void onPurchasesUpdated(@NonNull BillingResult result, List<Purchase> purchases) {
        Log.i(TAG, "onPurchasesUpdated: " + result.getResponseCode()
            + " / " + result.getDebugMessage()
            + ", purchases=" + (purchases == null ? "null" : purchases.size()));
        if (result.getResponseCode() == BillingClient.BillingResponseCode.OK && purchases != null) {
            for (Purchase purchase : purchases) {
                processPurchase(purchase, true);
            }
        } else {
            // USER_CANCELED, ITEM_ALREADY_OWNED, ERROR, etc.
            // Always release the C++ isProgress lock so the button stays responsive.
            nativeOnTransactionCanceled();
        }
    }

    private void processPurchase(Purchase purchase, boolean notify) {
        Log.i(TAG, "processPurchase: state=" + purchase.getPurchaseState()
            + ", products=" + purchase.getProducts() + ", notify=" + notify);
        if (purchase.getPurchaseState() != Purchase.PurchaseState.PURCHASED) return;

        if (!purchase.isAcknowledged()) {
            billingClient.acknowledgePurchase(
                AcknowledgePurchaseParams.newBuilder()
                    .setPurchaseToken(purchase.getPurchaseToken())
                    .build(),
                r -> {}
            );
        }

        for (String productId : purchase.getProducts()) {
            purchasedProducts.add(productId);
            if (notify) {
                nativeOnPurchaseSuccess(productId);
            }
        }
    }

    private native void nativeOnPurchaseSuccess(String productId);
    private native void nativeOnTransactionCanceled();
    private native void nativeOnRestoreComplete(int count);
}
