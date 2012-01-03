#ifndef AUDIOMANAGER_CPP
#define AUDIOMANAGER_CPP
#include "AudioManager.h"

C_AudioManager::C_AudioManager()
{
	m_currentTrack = -1;
}
C_AudioManager::~C_AudioManager()
{
	//Clean up music
	for(int i = 0; i < v_MusicLibrary.size(); i ++)
		Mix_FreeMusic(v_MusicLibrary[i]);
	//Clean up sfx
	for(int i = 0; i < v_SFXLibrary.size(); i ++)
		Mix_FreeChunk(v_SFXLibrary[i]);


}
int C_AudioManager::LoadMusic()
{
	Mix_Music *MusicToLoad = NULL;
	MusicToLoad = Mix_LoadMUS("Audio\\Faron_Woods_REMIX.mp3");
	v_MusicLibrary.push_back(MusicToLoad);

	MusicToLoad = NULL;
	MusicToLoad = Mix_LoadMUS("Audio\\Dark_World_REMIX.mp3");
	v_MusicLibrary.push_back(MusicToLoad);

	MusicToLoad = NULL;
	MusicToLoad = Mix_LoadMUS("Audio\\Light_World_Dungeon_REMIX.mp3");
	v_MusicLibrary.push_back(MusicToLoad);



/*
	char* geterr;
	if(MusicToLoad == NULL)
		geterr = Mix_GetError();	
*/	

	return 1;


}
int C_AudioManager::LoadSFX()
{
	char *errorMsg;
	Mix_Chunk *SfxToLoad = NULL;
	SfxToLoad = Mix_LoadWAV("Audio\\LTTP_Sword1.wav");
	if(SfxToLoad == NULL)
		errorMsg = Mix_GetError();
	v_SFXLibrary.push_back(SfxToLoad);

	SfxToLoad = Mix_LoadWAV("Audio\\LTTP_Boomerang.wav");
	v_SFXLibrary.push_back(SfxToLoad);

	SfxToLoad = Mix_LoadWAV("Audio\\SFX\\LTTP_Link_Hurt.wav");
	v_SFXLibrary.push_back(SfxToLoad);

	SfxToLoad = Mix_LoadWAV("Audio\\SFX\\LTTP_Link_Dying.wav");
	v_SFXLibrary.push_back(SfxToLoad);

	return 1;//TODO FIx these. They're not returning anything right now 
}
//MUSIC
int C_AudioManager::PlayMusic(int songToPlay)//TODO: make so i can select track and pass options as variables //fade in/out etc
{	
	m_currentTrack = songToPlay;
	return Mix_PlayMusic( v_MusicLibrary[m_currentTrack], -1);
}
int C_AudioManager::FadeOutMusic(int milliseconds)
{
	return Mix_FadeOutMusic(milliseconds);
	//returns 0 on error
}
int C_AudioManager::Get_CurrentTrack()
{
	return m_currentTrack;
}
bool C_AudioManager::Get_PlayingStatus()
{
	return Mix_PlayingMusic();
}
//SFX
int C_AudioManager::PlaySFX(int SfxType, int loops)//same
{
	return Mix_PlayChannel(-1, v_SFXLibrary[SfxType], loops);
}
#endif