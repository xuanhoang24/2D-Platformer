#ifndef GAME_CONTROLLER_H
#define GAME_CONTROLLER_H

#include "StandardIncludes.h"
#include "AudioController.h"

class Renderer;
class TTFont;
class InputController;
class AudioController;
class SoundEffect;
class Song;

class GameController : public Singleton<GameController>
{
public:
    // Constructors/ Destructors
    GameController();
    virtual ~GameController();

    //Methods
    void RunGame();
    void Initialize();
    void HandleInput(SDL_Event _event);
    void ShutDown();

private:
    //Members
    SDL_Event m_sdlEvent;
    Renderer* m_renderer;
    TTFont* m_fArial20;
    bool m_quit;
    InputController* m_input;
    AudioController* m_audio;
    SoundEffect* m_effects[MaxEffectChannels];
    Song* m_song;
};

#endif // GAME_CONTROLLER_H
