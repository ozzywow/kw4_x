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
    # Resources/Info.plist·Info_lite.plist 는 번들 ID 참조용 plist 일 뿐 런타임 리소스가 아니다.
    # 그대로 두면 앱 번들 루트로 복사되어 생성되는 Info.plist 와 충돌한다
    # (error: Multiple commands produce .../kw4_x.app/Info.plist).
    list(FILTER common_res_files EXCLUDE REGEX "Resources/Info(_lite)?\\.plist$")
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
    Classes/InfoScene.cpp
    Classes/MainMenuScene.cpp
    Classes/MKStoreManagerDelegate.cpp
    Classes/MKStoreManager_cpp.cpp
    Classes/NativeShare.cpp
    Classes/ParentalGate.cpp
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
    Classes/InfoScene.h
    Classes/KoreanUTF8.h
    Classes/MainMenuScene.h
    Classes/MKStoreManagerDelegate.h
    Classes/MKStoreManager_cpp.h
    Classes/MKStoreManager_mm.h
    Classes/NativeShare.h
    Classes/ParentalGate.h
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
        # xcassets/storyboard 는 MACOSX_PACKAGE_LOCATION 을 지정해야
        # CMake Xcode 제너레이터가 CopyFiles 빌드 페이즈에 포함시켜 actool/ibtool 이 실행된다.
        foreach(_ui_res ${APP_UI_RES})
            set_source_files_properties(
                "${CMAKE_CURRENT_SOURCE_DIR}/${_ui_res}"
                PROPERTIES MACOSX_PACKAGE_LOCATION ""
            )
        endforeach()
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
    elseif(MACOSX)
        list(APPEND GAME_SOURCE
            proj.ios_mac/mac/main.cpp
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

# ---------------------------------------------------------------------------
# 앱 타깃 구성
#   iOS 는 유료(kw4)·무료(kw4_lite) 두 실행 타깃을 만들어 한 프로젝트에서
#   두 버전을 함께 관리한다(Xcode 상단에서 스킴만 바꿔 전환).
#   Windows/macOS/Android 는 기존처럼 단일 kw4_x 타깃을 유지한다.
#   두 버전의 코드 차이는 LITE_VER 매크로 하나뿐이다. 기본은 라이트이며
#   (common_define.h), 유료 타깃만 KW4_PAID 를 정의해 라이트 기능을 끈다.
# ---------------------------------------------------------------------------
if(IOS)
    add_executable(kw4      ${all_code_files})   # 유료(정식)
    add_executable(kw4_lite ${all_code_files})   # 무료(라이트)
    set(APP_TARGETS kw4 kw4_lite)
elseif(ANDROID)
    add_library(${APP_NAME} SHARED ${all_code_files})
    set(APP_TARGETS ${APP_NAME})
else()
    add_executable(${APP_NAME} ${all_code_files})
    set(APP_TARGETS ${APP_NAME})
endif()

# 모든 앱 타깃 공통 설정
foreach(app_target ${APP_TARGETS})
    target_link_libraries(${app_target} cocos2d)
    # 인앱결제(MKStoreKit)가 StoreKit 을 쓰는데 cocos2d 는 이 프레임워크를 링크하지 않는다.
    # 빠지면 _OBJC_CLASS_$_SKPaymentQueue 등이 undefined 로 링크 실패한다.
    if(IOS)
        target_link_libraries(${app_target} "-framework StoreKit")
    endif()
    target_include_directories(${app_target}
        PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/Classes
        PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
    )
    setup_cocos_app_config(${app_target})
endforeach()

if(IOS)
    # 유료(정식) 타깃만 KW4_PAID 정의 → common_define.h 의 LITE_VER 를 끈다.
    # (무료 kw4_lite 는 정의하지 않으므로 기본값인 라이트로 빌드된다)
    target_compile_definitions(kw4 PRIVATE KW4_PAID)

    # 버전별 번들 메타데이터는 각자 plist 로 분리한다(유료 Info.plist.in / 무료 Info_lite.plist.in).
    # 같은 디렉터리에서 cocos_pak_xcode 를 두 번 호출하면 MACOSX_BUNDLE_* 변수가 마지막 값으로
    # 덮여 두 타깃이 같은 번들ID/이름을 갖는 CMake 함정이 있다. 그래서 번들ID·실행파일명은 plist
    # 안에서 Xcode 빌드 설정($(PRODUCT_BUNDLE_IDENTIFIER)/$(EXECUTABLE_NAME))으로 주입하고,
    # 번들ID 는 아래에서 타깃별 Xcode 속성으로 지정한다. 표시이름은 각 plist 에 하드코딩돼 있다.
    cocos_pak_xcode(kw4      INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info.plist.in")
    cocos_pak_xcode(kw4_lite INFO_PLIST "${CMAKE_CURRENT_SOURCE_DIR}/proj.ios_mac/ios/Info_lite.plist.in")
    set_xcode_property(kw4      PRODUCT_BUNDLE_IDENTIFIER "com.ozzywow.kw4iphone")
    set_xcode_property(kw4_lite PRODUCT_BUNDLE_IDENTIFIER "com.ozzywow.kw4iphonelite")

    # 유료/무료 아이콘을 별도 appiconset 으로 분리
    set_xcode_property(kw4      ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
    set_xcode_property(kw4_lite ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon_lite")

    foreach(app_target kw4 kw4_lite)
        # cocos2d-x 는 배포 타깃을 8.0 으로 두는데 최신 Xcode 는 12.0 미만을 받지 않는다
        set_xcode_property(${app_target} IPHONEOS_DEPLOYMENT_TARGET "12.0")
        # 서명은 Xcode 에서 본인 팀으로 지정할 것 (자동 서명 권장)
        set_xcode_property(${app_target} DEVELOPMENT_TEAM "GFX32EJL53")
        set_xcode_property(${app_target} CODE_SIGN_IDENTITY "iPhone Developer")
        # 아카이브가 App Store 배포로 인식되려면 앱이 .xcarchive 의 Applications/ 에 설치돼야 한다.
        # CMake 기본값은 INSTALL_PATH="" 라 Organizer 가 "일반 아카이브"로 분류하고
        # 배포 화면에 App Store Connect 옵션이 사라진 채 Custom 만 뜬다.
        set_xcode_property(${app_target} INSTALL_PATH "$(LOCAL_APPS_DIR)")
        set_xcode_property(${app_target} SKIP_INSTALL "NO")
    endforeach()
elseif(MACOSX)
    set_target_properties(${APP_NAME} PROPERTIES
        XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY ""
        XCODE_ATTRIBUTE_CODE_SIGNING_REQUIRED "NO"
        XCODE_ATTRIBUTE_CODE_SIGNING_ALLOWED "NO"
    )
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/../Resources")
    add_custom_command(TARGET ${APP_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${APP_RES_DIR}"
        COMMAND rsync -a --update "${GAME_RES_FOLDER}/" "${APP_RES_DIR}/"
    )
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
