#ifndef AUDIOMANAGER_H
#define AUDIOMANAGER_H
#include <SDL.h>
#include <SDL_mixer.h>
#include <vector>
#include <iostream>

class C_AudioManager{

	private:
			std::vector<Mix_Music*> v_MusicLibrary;//music
			std::vector<Mix_Chunk*> v_SFXLibrary;//sfx

			int m_currentTrack;

	public:
			C_AudioManager();
			~C_AudioManager();

			int LoadMusic();//TODO finish this, but will return -1 on error  ( code as false on other end)
			int LoadSFX();			

			//MUSIC
			int PlayMusic(int);
			int FadeOutMusic(int);//fade out time in milliseconds
			int Get_CurrentTrack();//returns what track is playing
			bool Get_PlayingStatus();//true if playing

			//SFX
			int PlaySFX(int, int);
};

#endif