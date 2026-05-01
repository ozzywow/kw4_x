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

    // ===== iPhone (출시 순) =====
    { "iphone4",        { 240, 360 } },  // 320x480
    { "iphone5",        { 240, 426 } },  // 320x568
    { "iphone5s",       { 240, 426 } },
    { "iphone6",        { 281, 500 } },  // 375x667
    { "iphone6plus",    { 311, 552 } },  // 414x736
    { "iphone6s",       { 281, 500 } },
    { "iphone6splus",   { 311, 552 } },
    { "iphone7",        { 281, 500 } },
    { "iphone7plus",    { 311, 552 } },
    { "iphone8",        { 281, 500 } },
    { "iphone8plus",    { 311, 552 } },

    { "iphonex",        { 281, 609 } },  // 375x812
    { "iphonexr",       { 311, 672 } },  // 414x896
    { "iphonexs",       { 281, 609 } },
    { "iphonexsmax",    { 311, 672 } },

    { "iphone11",       { 311, 672 } },
    { "iphone11pro",    { 281, 609 } },
    { "iphone11promax", { 311, 672 } },

    { "iphone12mini",   { 270, 585 } },  // 360x780
    { "iphone12",       { 293, 633 } },  // 390x844
    { "iphone12pro",    { 293, 633 } },
    { "iphone12promax", { 321, 695 } },  // 428x926

    { "iphone13mini",   { 270, 585 } },
    { "iphone13",       { 293, 633 } },
    { "iphone13pro",    { 293, 633 } },
    { "iphone13promax", { 321, 695 } },

    { "iphone14",       { 293, 633 } },
    { "iphone14plus",   { 321, 695 } },
    { "iphone14pro",    { 295, 639 } },  // 393x852
    { "iphone14promax", { 323, 699 } },  // 430x932

    { "iphone15",       { 295, 639 } },
    { "iphone15plus",   { 323, 699 } },
    { "iphone15pro",    { 295, 639 } },
    { "iphone15promax", { 323, 699 } },

    { "iphone16",       { 295, 639 } },
    { "iphone16plus",   { 323, 699 } },
    { "iphone17",       { 295, 639 } },
    { "iphone17plus",   { 323, 699 } },
    { "iphone17pro",    { 302, 656 } },  // 402x874
    { "iphone17promax", { 330, 717 } },  // 440x956

    // ===== iPad (출시 순) =====
    { "ipad2",          { 288, 384 } },  // 1024x768
    { "ipad3",          { 288, 384 } },
    { "ipad4",          { 288, 384 } },

    { "ipadair",        { 288, 384 } },
    { "ipadair2",       { 288, 384 } },

    { "ipadmini",       { 288, 384 } },
    { "ipadmini2",      { 288, 384 } },
    { "ipadmini3",      { 288, 384 } },
    { "ipadmini4",      { 288, 384 } },

    { "ipadpro97",      { 288, 384 } },
    { "ipadpro105",     { 313, 417 } },  // 834x1112
    { "ipadpro129",     { 384, 512 } },  // 1024x1366 → 75% 적용

    { "ipad6",          { 288, 384 } },
    { "ipad7",          { 304, 405 } },  // 810x1080
    { "ipad8",          { 304, 405 } },
    { "ipad9",          { 304, 405 } },

    { "ipad10",         { 615, 885 } },  // (기존값 유지 요청 반영)

    { "ipadmini6",      { 558, 850 } },  // (기존값 유지)

    { "ipadair4",       { 615, 885 } },
    { "ipadair5",       { 615, 885 } },

    { "ipadpro11",      { 626, 896 } },  // (기존값 유지)
    { "ipadpro129_2",   { 384, 512  } },
    { "ipadpro129_3",   { 384, 512  } },

    { "ipadpro13",      { 516, 688 } }   // 기존값 유지
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
