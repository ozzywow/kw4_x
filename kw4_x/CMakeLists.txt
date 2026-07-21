cmake_minimum_required(VERSION 3.10)

set(APP_NAME kw4_x)

project(${APP_NAME})

set(COCOS2DX_ROOT_PATH ${CMAKE_CURRENT_SOURCE_DIR}/../cocos2d-x)
set(CMAKE_MODULE_PATH ${COCOS2DX_ROOT_PATH}/cmake/Modules/)

include(CocosBuildSet)
add_subdirectory(${COCOS2DX_ROOT_PATH}/cocos ${ENGINE_BINARY_PATH}/cocos/core)

# Resources
set(GAME_RES_FOLDER "${CMAKE_CURRENT_SOURCE_DIR}/Resources")
if(APPLE OR WINDOWS)
    cocos_mark_multi_resources(common_res_files RES_TO "Resources" FOLDERS ${GAME_RES_FOLDER})
endif()

if(ANDROID)
    set(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} -Wl,--build-id")
endif()

# Cross-platform source files (exclude iOS/Mac Objective-C .mm/.m files)
set(GAME_SOURCE
    Classes/AppDelegate.cpp
    Classes/AppleTouchedHandleLayer.cpp
    Classes/AppleTreeScene.cpp
    Classes/Character.cpp
    Classes/CharacterFactory.cpp
    Classes/HelloWorldScene.cpp
    Classes/InfoScene.cpp
    Classes/MainMenuScene.cpp
    Classes/MKStoreManagerDelegate.cpp
    Classes/MKStoreManager_cpp.cpp
    Classes/NativeShare.cpp
    Classes/PointManager.cpp
    Classes/SoundFactory.cpp
    Classes/StudyScene.cpp
    Classes/TextLayer.cpp
    Classes/TouchedHandleLayer.cpp
    Classes/UI_GameResultWindow.cpp
    Classes/UI_PopupWindowBase.cpp
    Classes/pugixml.cpp
    Classes/stdafx.cpp
    Classes/wordFactory.cpp
)

set(GAME_HEADER
    Classes/AppDelegate.h
    Classes/AppleTouchedHandleLayer.h
    Classes/AppleTreeScene.h
    Classes/Character.h
    Classes/CharacterFactory.h
    Classes/HelloWorldScene.h
    Classes/InfoScene.h
    Classes/KoreanUTF8.h
    Classes/MainMenuScene.h
    Classes/MKStoreManagerDelegate.h
    Classes/MKStoreManager_cpp.h
    Classes/MKStoreManager_mm.h
    Classes/NativeShare.h
    Classes/PointManager.h
    Classes/Singleton.h
    Classes/SoundFactory.h
    Classes/StudyScene.h
    Classes/TextLayer.h
    Classes/TouchedHandleLayer.h
    Classes/UI_GameResultWindow.h
    Classes/UI_PopupWindowBase.h
    Classes/common_define.h
    Classes/pugiconfig.hpp
    Classes/pugixml.hpp
    Classes/smartAssert.h
    Classes/stdafx.h
    Classes/wordFactory.h
)

if(WINDOWS)
    list(APPEND GAME_HEADER
        proj.win32/main.h
        proj.win32/resource.h
    )
    list(APPEND GAME_SOURCE
        proj.win32/main.cpp
        proj.win32/game.rc
        ${common_res_files}
    )
elseif(APPLE)
    if(IOS)
        list(APPEND GAME_HEADER
            proj.ios_mac/ios/AppController.h
            proj.ios_mac/ios/RootViewController.h
        )
        set(APP_UI_RES
            proj.ios_mac/ios/LaunchScreen.storyboard
            proj.ios_mac/ios/LaunchScreenBackground.png
            proj.ios_mac/ios/Images.xcassets
        )
        list(APPEND GAME_SOURCE
            proj.ios_mac/ios/main.m
            proj.ios_mac/ios/AppController.mm
            proj.ios_mac/ios/RootViewController.mm
            proj.ios_mac/ios/Prefix.pch
            Classes/MKDelegateCPP.mm
            Classes/MKStoreManager.mm
            Classes/MKStoreManager_mm.mm
            Classes/MKStoreObserver.mm
            Classes/NativeShare_apple.mm
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
elseif(ANDROID)
    list(APPEND GAME_SOURCE
        Classes/AndroidBilling.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/javaactivity-android.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxHelper.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxAccelerometer.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxBitmap.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/jni/Java_org_cocos2dx_lib_Cocos2dxRenderer.cpp
        ${COCOS2DX_ROOT_PATH}/cocos/platform/android/jni/TouchesJni.cpp
    )
endif()

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)

if(ANDROID)
    add_library(${APP_NAME} SHARED ${all_code_files})
else()
    add_executable(${APP_NAME} ${all_code_files})
endif()

target_link_libraries(${APP_NAME} cocos2d)

# 인앱결제(MKStoreKit)가 StoreKit 을 쓰는데 cocos2d 는 이 프레임워크를 링크하지 않는다.
# 빠지면 _OBJC_CLASS_$_SKPaymentQueue 등이 undefined 로 링크 실패한다.
if(IOS)
    target_link_libraries(${APP_NAME} "-framework StoreKit")
endif()
target_include_directories(${APP_NAME}
    PRIVATE Classes
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

setup_cocos_app_config(${APP_NAME})

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(IOS)
        # cocos2d-x 기본 plist 템플릿은 가로 전용이고 번들 ID 도 org.cocos2dx.kw4_x 가 된다.
        # 번들 ID 가 다르면 기존 App Store 앱에 업로드할 수 없고 인앱결제 상품도 조회되지 않는다.
        # 값 출처: Resources/Info_lite.plist (라이트판, LITE_VER 빌드가 이것에 해당)
        cocos_pak_xcode(${APP_NAME}
            INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/kw4xBundleInfo.plist.in"
            GUI_IDENTIFIER "com.ozzywow.kw4iphonelite"
            BUNDLE_NAME "한글떼기1단계"
        )
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        # cocos2d-x 는 배포 타깃을 8.0 으로 두는데 최신 Xcode 는 12.0 미만을 받지 않는다
        set_xcode_property(${APP_NAME} IPHONEOS_DEPLOYMENT_TARGET "12.0")
        # 서명은 Xcode 에서 본인 팀으로 지정할 것 (자동 서명 권장)
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(WINDOWS)
    # 리소스 파일 변경 시에만 복사 (스탬프 파일로 증분 추적)
    # $<TARGET_FILE_DIR:...> 대신 경로를 직접 계산하여 순환 의존성 방지
    set(RES_STAMP "${CMAKE_CURRENT_BINARY_DIR}/res_copy.stamp")
    set(APP_RES_DIR "${CMAKE_BINARY_DIR}/bin/${APP_NAME}/$<CONFIG>/Resources")
    add_custom_command(
        OUTPUT "${RES_STAMP}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${GAME_RES_FOLDER}" "${APP_RES_DIR}"
        COMMAND ${CMAKE_COMMAND} -E touch "${RES_STAMP}"
        DEPENDS ${common_res_files}
        COMMENT "Copying resources..."
        VERBATIM
    )
    add_custom_target(copy_resources DEPENDS "${RES_STAMP}")
    add_dependencies(${APP_NAME} copy_resources)
endif()
