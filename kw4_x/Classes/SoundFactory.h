#include <SimpleAudioEngine.h>
#include "Singleton.h"

#pragma once

//bgm
#define SOUND_FILE_bgm					"Sound/click.mp3"
#define SOUND_FILE_apple_tree_bgm		"Sound/appleTree.aiff"

//effect
#define SOUND_FILE_click_effect			"Sound/click.mp3"
#define SOUND_FILE_tick_effect			"Sound/tick.wav"
#define SOUND_FILE_dingling_effect		"Sound/DingLing.wav"
#define SOUND_FILE_bulle_effect			"Sound/Bulle.wav"
#define SOUND_FILE_pass_effect			"Sound/pass.wav"
#define SOUND_FILE_turnpage_effect		"Sound/turnpage.wav"
#define SOUND_FILE_skip_effect			"Sound/skip.wav"
#define SOUND_FILE_puck_effect			"Sound/puck.wav"
#define SOUND_FILE_wind_effect			"Sound/Chimes.wav"
#define SOUND_FILE_levelup_effect		"Sound/levelup.wav"
#define SOUND_FILE_giggle_effect		"Sound/giggle.m4a"


//voice
#define SOUND_FILE_intro_voice			"Sound/intro.mp3"		//우리아이 한글떼기
#define SOUND_FILE_goodjob_voice		"Sound/goodjob.m4a"
#define SOUND_FILE_excellent_voice		"Sound/excellent.m4a"
#define SOUND_FILE_fighting_voice		"Sound/fighting.mp3"	// 시작

#define PRELOAD_SOUND_FILE(fileName)		CocosDenshion::SimpleAudioEngine::getInstance()->preloadEffect(fileName)
#define UNLOAD_SOUND_FILE(fileName)			CocosDenshion::SimpleAudioEngine::getInstance()->unloadEffect(fileName)
#define PLAY_EFFECT_SOUND_FILE(fineName)	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(fileName)

class SoundFactory : public Singleton<SoundFactory>
{
public:
	
	float m_mastVolume;


	SoundFactory( float initVol = 1 );
	~SoundFactory();

	void play(char* soundFile);


};
