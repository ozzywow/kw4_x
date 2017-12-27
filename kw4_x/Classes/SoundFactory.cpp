#include "stdafx.h"
#include "SoundFactory.h"

SoundFactory::SoundFactory(float initVol)
{
	m_mastVolume = initVol;

	PRELOAD_SOUND_FILE(SOUND_FILE_bgm);
	PRELOAD_SOUND_FILE(SOUND_FILE_apple_tree_bgm);
	PRELOAD_SOUND_FILE(SOUND_FILE_click_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_tick_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_dingling_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_bulle_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_pass_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_turnpage_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_skip_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_puck_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_wind_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_levelup_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_giggle_effect);
	PRELOAD_SOUND_FILE(SOUND_FILE_intro_voice);
	PRELOAD_SOUND_FILE(SOUND_FILE_goodjob_voice);
	PRELOAD_SOUND_FILE(SOUND_FILE_excellent_voice);
	PRELOAD_SOUND_FILE(SOUND_FILE_fighting_voice);
}

SoundFactory::~SoundFactory()
{
	UNLOAD_SOUND_FILE(SOUND_FILE_bgm);
	UNLOAD_SOUND_FILE(SOUND_FILE_apple_tree_bgm);
	UNLOAD_SOUND_FILE(SOUND_FILE_click_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_tick_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_dingling_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_bulle_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_pass_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_turnpage_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_skip_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_puck_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_wind_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_levelup_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_giggle_effect);
	UNLOAD_SOUND_FILE(SOUND_FILE_intro_voice);
	UNLOAD_SOUND_FILE(SOUND_FILE_goodjob_voice);
	UNLOAD_SOUND_FILE(SOUND_FILE_excellent_voice);
	UNLOAD_SOUND_FILE(SOUND_FILE_fighting_voice);
}

void SoundFactory::play(char* soundFile)
{	
	CocosDenshion::SimpleAudioEngine::getInstance()->playEffect(soundFile);
}