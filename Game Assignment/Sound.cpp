#include "Sound.h"

// Music
Music::Music(std::string _filepath) {
	music = nullptr;
	filepath = _filepath;

	music = Mix_LoadMUS(filepath.c_str());
	if(!music) {
		printf("[MUSIC] Couldn't load music on %s : %s\n", filepath.c_str(), Mix_GetError());
	} else {
		printf("[MUSIC] %s was loaded\n", filepath.c_str());
	}
}
Music::~Music() {
	if(music) {
		Mix_FreeMusic(music);
	}
}
void Music::play(int times, int fadeInMs) {
	if(!music) return;
	if(Mix_FadeInMusic(music, times, fadeInMs)) {
		printf("[MUSIC] Couldn't play music on %s\n", filepath.c_str());
		return;
	}
}
void Music::fadeOut(int ms) {

}
void Music::stop() {

}
void Music::pause() {

}
void Music::unpause() {

}
void Music::restart() {

}
void Music::rewind() {

}
void Music::setPosition(double seconds) {

}
bool Music::isPlaying() {
	
	return true;
}
bool Music::isPaused() {
	
	return true;
}
bool Music::isFadingIn() {
	return true;
}
bool Music::isFadingOut() {
	
	return true;
}
int Music::setVolume(int volume) {
	
	return 1;
}
int Music::getVolume() {
	
	return 1;
}
int Music::getMaxVolume() {
	
	return 1;
}

// Sound effects
SFX::SFX(std::string _filepath) {
	sfx = nullptr;
	filepath = _filepath;

	sfx = Mix_LoadWAV(_filepath.c_str());
	if(!sfx) {
		printf("[SOUND] Couldn't load sound on %s : %s\n", filepath.c_str(), Mix_GetError());
	} else {
		printf("[SOUND] %s was loaded\n", filepath.c_str());
	}
	Mix_VolumeChunk(sfx, 128);
}

SFX::~SFX() {
	if(sfx) {
		Mix_FreeChunk(sfx);
	}
}

void SFX::play(int times) {
	if(!sfx) return;
	if(times > 0) times--;

	int chan = Mix_PlayChannel(-1, sfx, times);
	if(chan == -1) {	
		printf("Couldn't play SFX");
	}
	channel = chan;
}

bool SFX::isPlaying() {
	return Mix_Playing(channel) && !isPaused();
}

bool SFX::isPaused() {
	return Mix_Paused(channel);
}

void SFX::pause() {
	Mix_Pause(channel);
}

void SFX::stop() {
	if(Mix_GetChunk(channel) == sfx) {
		Mix_HaltChannel(channel);
	}
}

int SFX::getVolume() {
	return Mix_VolumeChunk(sfx, -1);
}

void SFX::setVolume(int _volume) {
	Mix_VolumeChunk(sfx, volume);
}

Mix_Chunk* SFX::getSFX() {
	return sfx;
}

// SoundManager
std::map<std::string, SFX*> SoundManager::sounds;
std::map<std::string, Music*> SoundManager::mus;
std::string SoundManager::soundDir;
std::string SoundManager::MUSdir;

SoundManager::SoundManager(){
	sounds.clear();
	masterVolume = MIX_MAX_VOLUME * 0.5;
}

void SoundManager::setSoundDir(std::string _soundDir) { soundDir = _soundDir; }
void SoundManager::setMusicDir(std::string _MUSdir) { MUSdir = _MUSdir; }

void SoundManager::addSound(std::string name, std::string filename) {
	SFX* sound = new SFX(std::string(soundDir + filename));
	sounds.insert(std::pair<std::string, SFX*>(name, sound));
}

void SoundManager::addMusic(std::string name, std::string filename) {
	Music* _mus = new Music(std::string(MUSdir + filename));
	mus.insert(std::pair<std::string, Music*>(name, _mus));
}

SFX* SoundManager::get(std::string name) {
	auto it = sounds.find(name);
	if(it != sounds.end()) {
		return it->second;
	}
}

Music* SoundManager::music(std::string name) {
	auto it = mus.find(name);
	if(it != mus.end()) {
		return it->second;
	}
}