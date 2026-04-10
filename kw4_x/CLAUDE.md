# Project Overview

This project is a 2D game built using cocos2d-x (v3.17.x) in C++.
The project uses a Scene-based architecture and supports multiple platforms including Windows and iOS.

# Tech Stack

* Language: C++
* Engine: cocos2d-x 3.17.x
* IDE: Visual Studio Code
* Build:

  * Windows: Visual Studio / CMake
  * iOS: Xcode
* Platforms:

  * Windows (Win32)
  * iOS

# Folder Structure

* /Classes: Core game logic and scene implementations
* /Resources: Images, sounds, and assets
* /proj.win32: Windows build project files
* /proj.ios_mac: iOS and macOS Xcode project files

# Key Files

* AppDelegate.cpp: Application entry point and initialization
* main.cpp: Program entry
* MainMenuScene.cpp: Main menu logic
* StudyScene.cpp: Core gameplay scene
* InfoScene.cpp: Information UI
* TouchedHandleLayer.cpp: Input handling

# Architecture

* The project follows cocos2d-x Scene structure
* Each scene is responsible for its own UI and logic
* Scene transitions should use Director

# Platform Notes

## Windows

* Built using Visual Studio or CMake
* Output: .exe file

## iOS

* Built using Xcode
* Located in /proj.ios_mac
* Requires macOS environment
* Deployment via iOS Simulator or real device

# Coding Rules

* Follow existing code style in the project
* Do NOT rename existing classes or files
* Keep class structure consistent
* Use clear and minimal modifications
* Avoid unnecessary abstraction

# Build & Run

## Windows

* cmake --build .
* or use Visual Studio build

## iOS

* Open /proj.ios_mac in Xcode
* Build and run using Xcode

# Debugging

* Use VSCode launch.json for Windows debugging
* For iOS, use Xcode debugger
* Ensure correct executable or scheme is selected

# Instructions for AI (VERY IMPORTANT)

## Do

* Modify existing files instead of creating new ones
* Keep changes minimal and localized
* Follow existing patterns in the codebase
* Ensure code compiles on both Windows and iOS if applicable
* Respect current architecture (Scene-based)

## Don't

* Do NOT refactor entire project structure
* Do NOT introduce new frameworks or libraries
* Do NOT rename files or folders
* Do NOT change unrelated code
* Do NOT over-engineer solutions

# Preferred Workflow

1. Analyze existing code first
2. Identify minimal change needed
3. Modify only relevant parts
4. Ensure build success (Windows and/or iOS)
5. Keep solution simple

# Notes

* This is a cocos2d-x cross-platform project
* Stability is more important than new features
* Changes should be safe and incremental
* iOS builds must be compatible with Xcode project settings
