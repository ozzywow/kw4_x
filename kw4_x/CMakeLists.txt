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
            Classes/MKStoreManager_mm.mm
            Classes/MKStoreObserver.mm
            ${APP_UI_RES}
        )
    endif()
    list(APPEND GAME_SOURCE ${common_res_files})
endif()

set(all_code_files
    ${GAME_HEADER}
    ${GAME_SOURCE}
)

add_executable(${APP_NAME} ${all_code_files})

target_link_libraries(${APP_NAME} cocos2d)
target_include_directories(${APP_NAME}
    PRIVATE Classes
    PRIVATE ${COCOS2DX_ROOT_PATH}/cocos/audio/include/
)

setup_cocos_app_config(${APP_NAME})

if(APPLE)
    set_target_properties(${APP_NAME} PROPERTIES RESOURCE "${APP_UI_RES}")
    if(IOS)
        cocos_pak_xcode(${APP_NAME} INFO_PLIST "iOSBundleInfo.plist.in")
        set_xcode_property(${APP_NAME} ASSETCATALOG_COMPILER_APPICON_NAME "AppIcon")
        set_xcode_property(${APP_NAME} DEVELOPMENT_TEAM "")
        set_xcode_property(${APP_NAME} CODE_SIGN_IDENTITY "iPhone Developer")
    endif()
elseif(WINDOWS)
    cocos_copy_target_dll(${APP_NAME})
endif()

if(WINDOWS)
    set(APP_RES_DIR "$<TARGET_FILE_DIR:${APP_NAME}>/Resources")
    cocos_copy_target_res(${APP_NAME} COPY_TO ${APP_RES_DIR} FOLDERS ${GAME_RES_FOLDER})
endif()
