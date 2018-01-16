#ifndef MKDELEGATE_CPP_H
#define MKDELEGATE_CPP_H

#include "MKStoreManager.h"

#if __cplusplus
#include "MKStoreManagerDelegate.h"

@interface InterfaceMKStoreKitDelegate : NSObject<MKStoreKitDelegate>
{	
}
@end

@interface InterfaceMKStoreKitDelegate()
@property (nonatomic, string) MKStoreManagerDelegate* _cppDelegate;
-(void)setdeletegate:(MKStoreManagerDelegate*) d;
-(void)productFetchComplete;
-(void)productPurchased:(NSString *)productId;
-(void)transactionCanceled;
@end

#endif //__cplusplus
#endif //MKDELEGATE_CPP_H