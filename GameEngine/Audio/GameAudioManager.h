#ifndef GAME_AUDIO_MANAGER_H
#define GAME_AUDIO_MANAGER_H

#include "../Core/StandardIncludes.h"

class SoundEffect;
class Song;
class AudioController;

class GameAudioManager : public Singleton<GameAudioManager>
{
public:
    GameAudioManager();
    ~GameAudioManager();

    void Initialize();
    void Shutdown();

    // Game sound effects
    void PlayPlayerJumpSound();
    void PlayEnemyStompSound();
    void PlayClickSound();
    void PlayPunchSound();
    void PlayDashSound();
    void PlayHurtSound();
    void PlayDieSound();
    
    // Background music
    void PlayMenuMusic();
    void PlayBackgroundMusic();
    void StopMusic();
    void PauseMusic();
    void ResumeMusic();
    
    // Stop all sounds
    void StopAllSounds();

private:
    AudioController* m_audioController = nullptr;
    
    SoundEffect* m_playerJumpSound = nullptr;
    SoundEffect* m_enemyStompSound = nullptr;
    SoundEffect* m_clickSound = nullptr;
    SoundEffect* m_punchSound = nullptr;
    SoundEffect* m_dashSound = nullptr;
    SoundEffect* m_hurtSound = nullptr;
    SoundEffect* m_dieSound = nullptr;
    Song* m_menuMusic = nullptr;
    Song* m_backgroundMusic = nullptr;
};

#endif // GAME_AUDIO_MANAGER_H