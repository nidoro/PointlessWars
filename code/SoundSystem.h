#ifndef SOUNDSYSTEM_H
#define SOUNDSYSTEM_H

#include "System.h"
#include "Standard.h"
#include "Assets.h"
#include <SFML/Audio.hpp>

class SoundSystem : public System{
    public:
        SoundSystem();
        ~SoundSystem();

        void update();
        void updateMusic();

    private:
        void onPlaySound(Entity* e);
        void onPlaySound(std::string filename);
        void onPlayMusic(Entity* e);
        void onStopMusic(Entity* e);
        void onWindowClosed(Entity* e);
        void onWindowGainedFocus(Entity* e);
        void onWindowLostFocus(Entity* e);

        void onNewCommandLine(Entity* e);
        void updateVolumn(Entity* e);
        void evenVolume(Entity* e);
        void updateMusicVolume();
        void startPlayingMusic();
        void startMusicFadeOut();

        Entity* eNowPlaying;
        Entity* eNextMusic;
        bool fadingOut;

        double musMaxVolume;
        double musFadeInLength;
        double musFadeOutLength;

        bool mute;

};

#endif // SOUNDSYSTEM_H
