#pragma once

#include <string>
#include <map>
#include "SDL_mixer.h"

class Music {
public:
	Music(std::string _filepath);
	~Music();

	void play(int times = -1, int fadeInMs = 0);
	void fadeOut(int ms);
	void stop();
	void pause();
	void unpause();
	void restart();
	void rewind();
	void setPosition(double seconds);
	bool isPlaying();
	bool isPaused();
	bool isFadingIn();
    bool isFadingOut();
    int setVolume(int volume);
    int getVolume();
    int getMaxVolume();

private:
	Mix_Music* music;
	std::string filepath;
};

class SFX {
public:
	SFX(std::string _filepath);
	~SFX();
	
	void play(int times = 1);
	void pause();
	void unpause();
	void stop();
	
	bool isPlaying();
	bool isPaused();

	int getVolume();
	void setVolume(int _volume);

	Mix_Chunk* getSFX();

private:
	Mix_Chunk* sfx;
	std::string filepath;
	int channel;
	int volume;
};

class SoundManager {
public:
	SoundManager();

	static void setSoundDir(std::string _soundDir);
	static void setMusicDir(std::string _MUSdir);
	static void addSound(std::string name, std::string filename);
	static void addMusic(std::string name, std::string filename);
	static int getMasterVolume();
	static void setMasterVolume(int _masterVolume);
	static void stop();
	static void pause();
	static SFX* get(std::string name);
	static Music* music(std::string name);
private:
	static std::string soundDir;
	static std::string MUSdir;
	int masterVolume;
	static std::map<std::string, SFX*> sounds;
	static std::map<std::string, Music*> mus;
};