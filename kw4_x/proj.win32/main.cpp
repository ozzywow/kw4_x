#include "main.h"
#include "AppDelegate.h"
#include "cocos2d.h"
#include <map>
#include <string>

USING_NS_CC;

// 기종별 해상도 프리셋 (물리 포인트 기준, 데스크탑 시뮬용 75% 스케일 적용)
// 사용법: kw4_x.exe <기종명>
// 예시:   kw4_x.exe iphone17
//         kw4_x.exe ipadpro13
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

int AppDelegate::s_windowWidth  = 516;  // 기본값: iPad Pro 13" 시뮬
int AppDelegate::s_windowHeight = 688;

int WINAPI _tWinMain(HINSTANCE hInstance,
                     HINSTANCE hPrevInstance,
                     LPTSTR    lpCmdLine,
                     int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);

    // 커맨드라인에서 기종명 파싱
    if (lpCmdLine && lpCmdLine[0] != '\0')
    {
        // LPTSTR -> std::string 변환 후 소문자화
        int len = WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, nullptr, 0, nullptr, nullptr);
        std::string arg(len, '\0');
        WideCharToMultiByte(CP_ACP, 0, lpCmdLine, -1, &arg[0], len, nullptr, nullptr);
        // 앞뒤 공백/따옴표 제거
        while (!arg.empty() && (arg.front() == ' ' || arg.front() == '"')) arg.erase(arg.begin());
        while (!arg.empty() && (arg.back()  == ' ' || arg.back()  == '"' || arg.back() == '\0')) arg.pop_back();
        // 소문자 변환
        for (char& c : arg) c = (char)tolower((unsigned char)c);

        auto it = kDevicePresets.find(arg);
        if (it != kDevicePresets.end())
        {
            AppDelegate::s_windowWidth  = it->second.first;
            AppDelegate::s_windowHeight = it->second.second;
        }
        else
        {
            // WxH 형식 직접 입력 지원: 예) kw4_x.exe 390x844
            int w = 0, h = 0;
            if (sscanf(arg.c_str(), "%dx%d", &w, &h) == 2 && w > 0 && h > 0)
            {
                AppDelegate::s_windowWidth  = w;
                AppDelegate::s_windowHeight = h;
            }
        }
    }

    AppDelegate app;
    return Application::getInstance()->run();
}
