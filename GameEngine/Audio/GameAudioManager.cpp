#include "GameAudioManager.h"
#include "AudioController.h"
#include "SoundEffect.h"
#include "Song.h"

GameAudioManager::GameAudioManager()
{
}

GameAudioManager::~GameAudioManager()
{
    Shutdown();
}

void GameAudioManager::Initialize()
{
    m_audioController = &AudioController::Instance();
    
    // Load all audio assets
    m_playerJumpSound = m_audioController->LoadEffect("../Assets/Audio/playerJump.wav");
    m_enemyStompSound = m_audioController->LoadEffect("../Assets/Audio/jump.wav");
    m_clickSound = m_audioController->LoadEffect("../Assets/Audio/collect.wav");
    m_punchSound = m_audioController->LoadEffect("../Assets/Audio/punch.wav");
    m_dashSound = m_audioController->LoadEffect("../Assets/Audio/dash.wav");
    m_hurtSound = m_audioController->LoadEffect("../Assets/Audio/hurt.wav");
    m_dieSound = m_audioController->LoadEffect("../Assets/Audio/die.wav");
    m_menuMusic = m_audioController->LoadSong("../Assets/Audio/menu.wav");
    m_backgroundMusic = m_audioController->LoadSong("../Assets/Audio/background.wav");
}

void GameAudioManager::Shutdown()
{
    m_playerJumpSound = nullptr;
    m_enemyStompSound = nullptr;
    m_clickSound = nullptr;
    m_punchSound = nullptr;
    m_dashSound = nullptr;
    m_hurtSound = nullptr;
    m_dieSound = nullptr;
    m_menuMusic = nullptr;
    m_backgroundMusic = nullptr;
    m_audioController = nullptr;
}

void GameAudioManager::PlayPlayerJumpSound()
{
    if (m_playerJumpSound && m_audioController)
        m_audioController->Play(m_playerJumpSound);
}

void GameAudioManager::PlayEnemyStompSound()
{
    if (m_enemyStompSound && m_audioController)
        m_audioController->Play(m_enemyStompSound);
}

void GameAudioManager::PlayClickSound()
{
    if (m_clickSound && m_audioController)
        m_audioController->Play(m_clickSound);
}

void GameAudioManager::PlayPunchSound()
{
    if (m_punchSound && m_audioController)
        m_audioController->Play(m_punchSound);
}

void GameAudioManager::PlayDashSound()
{
    if (m_dashSound && m_audioController)
        m_audioController->Play(m_dashSound);
}

void GameAudioManager::PlayHurtSound()
{
    if (m_hurtSound && m_audioController)
        m_audioController->Play(m_hurtSound);
}

void GameAudioManager::PlayDieSound()
{
    if (m_dieSound && m_audioController)
        m_audioController->Play(m_dieSound);
}

void GameAudioManager::PlayMenuMusic()
{
    if (m_menuMusic && m_audioController)
        m_audioController->Play(m_menuMusic);
}

void GameAudioManager::PlayBackgroundMusic()
{
    if (m_backgroundMusic && m_audioController)
        m_audioController->Play(m_backgroundMusic);
}

void GameAudioManager::StopMusic()
{
    if (m_audioController)
        m_audioController->StopMusic();
}

void GameAudioManager::PauseMusic()
{
    if (m_audioController)
        m_audioController->PauseMusic();
}

void GameAudioManager::ResumeMusic()
{
    if (m_audioController)
        m_audioController->ResumeMusic();
}

void GameAudioManager::StopAllSounds()
{
    if (m_audioController)
    {
        m_audioController->StopMusic();
    }
}