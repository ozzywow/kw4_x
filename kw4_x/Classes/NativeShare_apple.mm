#include "NativeShare.h"
#include "platform/CCPlatformConfig.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC)

#import <TargetConditionals.h>

#if TARGET_OS_IOS
#import <UIKit/UIKit.h>

bool NativeShare::isSupported() { return true; }

// keyWindow 는 iOS 13 에서 deprecated 되었고 상황에 따라 nil 을 반환한다.
// UIScene 이 있으면 그쪽에서 먼저 찾고, 없을 때만(cocos2d-x 3.17 은 UIScene 미사용)
// 예전 방식으로 되돌아간다.
static UIWindow* kw4xKeyWindow()
{
	if (@available(iOS 13.0, *)) {
		for (UIScene* scene in [UIApplication sharedApplication].connectedScenes) {
			if (![scene isKindOfClass:[UIWindowScene class]]) continue;
			for (UIWindow* w in ((UIWindowScene*)scene).windows) {
				if (w.isKeyWindow) return w;
			}
		}
	}
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wdeprecated-declarations"
	UIWindow* w = [UIApplication sharedApplication].keyWindow;
	if (!w) w = [[UIApplication sharedApplication].windows firstObject];
#pragma clang diagnostic pop
	return w;
}

void NativeShare::share(const std::string& text)
{
	NSString* s = [NSString stringWithUTF8String:text.c_str()];
	if (!s) return;
	dispatch_async(dispatch_get_main_queue(), ^{
		UIViewController* root = kw4xKeyWindow().rootViewController;
		while (root.presentedViewController) root = root.presentedViewController;
		if (!root) return;

		UIActivityViewController* av =
			[[UIActivityViewController alloc] initWithActivityItems:@[s] applicationActivities:nil];

		// iPad: 공유 시트는 popover라 소스 지정 필요(없으면 크래시)
		if (av.popoverPresentationController) {
			av.popoverPresentationController.sourceView = root.view;
			av.popoverPresentationController.sourceRect =
				CGRectMake(root.view.bounds.size.width / 2, root.view.bounds.size.height / 2, 1, 1);
			av.popoverPresentationController.permittedArrowDirections = 0;
		}
		[root presentViewController:av animated:YES completion:nil];
	});
}

#else
// ── Mac 데스크톱: 공유 시트 미사용 → 호출측이 링크 열기로 폴백 ──
bool NativeShare::isSupported() { return false; }
void NativeShare::share(const std::string&) {}
#endif

#endif
