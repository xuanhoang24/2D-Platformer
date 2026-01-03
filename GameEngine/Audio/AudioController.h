#ifndef AUDIOCONTROLLER_H
#define AUDIOCONTROLLER_H

#include "../Core/StandardIncludes.h"

constexpr int MaxEffectChannels = 16;

class SoundEffect;
class Song;

class AudioController : public Singleton<AudioController>
{
public:
	// Contructors/Destructors
	AudioController();
	virtual ~AudioController();

	// Accessors
	string* GetCurrentEffect() { return m_currentEffects; }
	string GetMusicTitle() { return m_musicTitle; }
	string GetMusicLength() { return m_musicLength; }

	// Methods
	SoundEffect* LoadEffect(string _guid);
	Song* LoadSong(string _guid);
	void Play(SoundEffect* _effect);
	void Play(Song* _song);
	void StopMusic();
	void PauseMusic();
	void ResumeMusic();
	double MusicPosition();
	void Shutdown();
	
	// Volume control (0-100)
	void SetMasterVolume(int _volume);
	void SetMusicVolume(int _volume);
	void SetEffectsVolume(int _volume);
	int GetMasterVolume() const { return m_masterVolume; }
	int GetMusicVolume() const { return m_musicVolume; }
	int GetEffectsVolume() const { return m_effectsVolume; }

	static void CatchChannelDone(int _channel);

private:
	// Methods
	Mix_Chunk* GetSDLSFX(SoundEffect* _effect);
	Mix_Music* GetSDLMusic(Song* _song);

	// Members
	map<string, Mix_Chunk*> m_effects;
	map<string, Mix_Music*> m_songs;
	string m_currentEffects[MaxEffectChannels];
	string m_musicLength;
	string m_musicTitle;
	Mix_Music* m_currentSong;
	
	// Volume settings (0-100)
	int m_masterVolume;
	int m_musicVolume;
	int m_effectsVolume;
};

#endif // AUDIOCONTROLLER_H
