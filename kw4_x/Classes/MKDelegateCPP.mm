#import "MKDelegateCPP.h"

@implementation InterfaceMKStoreKitDelegate()
-(void)setdeletegate:(MKStoreManagerDelegate*) d
{
	self._cppDelegate = d;
}
-(void)productFetchComplete
{
	self._cppDelegate->productFetchComplete();
}

-(void)productPurchased:(NSString *)productId
{
	self._cppDelegate->productPurchased(productId);
}

-(void)transactionCanceled
{
	self._cppDelegate->transactionCanceled();
}
@end