#include "AppDelegate.h"
#include "cocos2d.h"
#include <map>
#include <string>
#include <algorithm>
#include <mach-o/dyld.h>  // _NSGetExecutablePath

// 기종별 해상도 프리셋 (물리 포인트 기준, 데스크탑 시뮬용)
// 사용법: kw4_x <기종명>  또는  kw4_x WxH
static const std::map<std::string, std::pair<int,int>> kDevicePresets = {
    // iPhone
    { "iphonese",     { 281, 500 } },  // iPhone SE 3rd  375x667
    { "iphone16",     { 295, 639 } },  // iPhone 16      393x852
    { "iphone17",     { 295, 639 } },  // iPhone 17      393x852
    { "iphone16plus", { 323, 699 } },  // iPhone 16 Plus 430x932
    { "iphone17plus", { 323, 699 } },  // iPhone 17 Plus 430x932
    // iPad
    { "ipadmini",     { 558, 850 } },  // iPad mini 6    744x1133
    { "ipadair",      { 615, 885 } },  // iPad Air       820x1180
    { "ipad10",       { 615, 885 } },  // iPad 10th gen  820x1180
    { "ipadpro11",    { 626, 896 } },  // iPad Pro 11"   834x1194
    { "ipadpro13",    { 516, 688 } },  // iPad Pro 13"  1024x1366 (default)
};

// macOS: 실행 파일 경로에서 .app 번들의 Contents/Resources 절대 경로를 반환
// ex) .../kw4_x.app/Contents/MacOS/kw4_x → .../kw4_x.app/Contents/Resources/
static std::string getBundleResourcePath() {
    char exePath[4096] = {};
    uint32_t size = sizeof(exePath);
    if (_NSGetExecutablePath(exePath, &size) != 0) return "";

    std::string path(exePath);
    // Contents/MacOS/kw4_x → Contents/MacOS/ 제거 → Contents/Resources/
    auto pos = path.rfind("/MacOS/");
    if (pos == std::string::npos) return "";
    return path.substr(0, pos) + "/Resources/";
}

int main(int argc, char *argv[]) {
    // macOS: NSBundle pathForResource:ofType:inDirectory: 는 한글 파일명+서브디렉토리
    // 조합을 올바르게 처리하지 못함. 번들 Resources 절대 경로를 명시적으로 지정하면
    // fileExistsAtPath 분기를 타서 한글 경로도 정상 동작함.
    std::string resPath = getBundleResourcePath();
    if (!resPath.empty()) {
        cocos2d::FileUtils::getInstance()->setDefaultResourceRootPath(resPath);
    }

    // 커맨드라인 인자로 기종명 또는 WxH 해상도 지정
    if (argc >= 2 && argv[1] && argv[1][0] != '\0') {
        std::string arg(argv[1]);
        std::transform(arg.begin(), arg.end(), arg.begin(),
                       [](unsigned char c){ return (char)tolower(c); });

        auto it = kDevicePresets.find(arg);
        if (it != kDevicePresets.end()) {
            AppDelegate::s_windowWidth  = it->second.first;
            AppDelegate::s_windowHeight = it->second.second;
        } else {
            // WxH 형식 직접 입력: 예) 390x844
            int w = 0, h = 0;
            if (sscanf(arg.c_str(), "%dx%d", &w, &h) == 2 && w > 0 && h > 0) {
                AppDelegate::s_windowWidth  = w;
                AppDelegate::s_windowHeight = h;
            }
        }
    }

    AppDelegate app;
    return cocos2d::Application::getInstance()->run();
}
