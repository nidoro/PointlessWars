#include "SoundSystem.h"

SoundSystem::SoundSystem(){
    addRequirement(Component::SOUND);
    addSubscription(PLAY_SOUND);
    addSubscription(PLAY_MUSIC);
    addSubscription(NEW_COMMAND_LINE);
    addSubscription(WINDOW_LOST_FOCUS);
    addSubscription(WINDOW_GAINED_FOCUS);
    addSubscription(STOP_MUSIC);

    eNowPlaying = nullptr;
    eNextMusic = nullptr;

    musFadeInLength = 0.1;
    musFadeOutLength = 1.0;
    musMaxVolume = 10;
    fadingOut = false;
    mute = false;
}

SoundSystem::~SoundSystem(){
    //dtor
}

void SoundSystem::update(){
    for (EntityListIt i = entities.begin(); i != entities.end(); i++){
        Entity* e = *i;
        updateVolumn(e);
        //evenVolume(e);
        if (e->get<CSound>()->sound.getStatus() == sf::Sound::Stopped){
            if (e->get<CSound>()->endTrigger == CSound::REMOVE_ENTITY){
                eManager->removeEntity(e);
            }else if (e->get<CSound>()->endTrigger == CSound::REMOVE_COMPONENT){
                e->remove(Component::SOUND);
                eManager->addModified(e);
            }else if (e->get<CSound>()->endTrigger == CSound::LOOP){
                e->get<CSound>()->sound.setLoop(true);
            }else if (e->get<CSound>()->endTrigger == CSound::LOOP_THEN_REMOVE){
                e->get<CSound>()->sound.setLoop(true);
                if (e->get<CSound>()->clock.getElapsedTime().asSeconds() >= e->get<CSound>()->duration){
                    eManager->removeEntity(e);
                }
            }
        }
    }
    
    if (window->hasFocus()) mute = false;
    else                    mute = true;
    updateMusic();
}

void SoundSystem::onPlaySound(Entity* e){
    if (mute) return;
    if (e->get<CSound>()->name.empty()) return;
    if (Assets::getSound(e->get<CSound>()->name) == NULL){
        e->get<CSound>()->sound.play();
    }else{
        e->get<CSound>()->sound.setBuffer(*Assets::getSound(e->get<CSound>()->name));
        e->get<CSound>()->clock.restart();
        if (e->get<CSound>()->endTrigger == CSound::REMOVE_ENTITY || e->get<CSound>()->endTrigger == CSound::REMOVE_COMPONENT){
            e->get<CSound>()->duration = e->get<CSound>()->sound.getBuffer()->getDuration().asSeconds();
        }

        if (e->get<CSound>()->fadeInLength > 0.f){
            e->get<CSound>()->volumn = 0;
        }else{
            e->get<CSound>()->volumn = e->get<CSound>()->maxVolumn;
        }

        updateVolumn(e);
        e->get<CSound>()->sound.play();
    }
}

void SoundSystem::updateVolumn(Entity* e){
    //double bufferDuration = e->get<CSound>()->sound.getBuffer()->getDuration().asSeconds();
    double duration = e->get<CSound>()->duration;
    double timeElapsed = e->get<CSound>()->clock.getElapsedTime().asSeconds();
    double fadeInLength = e->get<CSound>()->fadeInLength;
    double fadeOutLength = e->get<CSound>()->fadeOutLength;
    double maxVolumn = e->get<CSound>()->maxVolumn;
    double fadeSpeed = 0;
    if (timeElapsed < fadeInLength){
        fadeSpeed = maxVolumn/fadeInLength;
    }else if (duration - timeElapsed < fadeOutLength){
        fadeSpeed = -maxVolumn/fadeOutLength;
    }

    e->get<CSound>()->volumn += fadeSpeed*delay;
    e->get<CSound>()->volumn = min(e->get<CSound>()->volumn, maxVolumn);
    e->get<CSound>()->volumn = max(e->get<CSound>()->volumn, 0.0);

    e->get<CSound>()->sound.setVolume(e->get<CSound>()->volumn);
}

void SoundSystem::evenVolume(Entity* e){
    int nInstances = 1;
    for (auto& i : entities){
        if (e->get<CSound>()->name == i->get<CSound>()->name && i != e){
            nInstances++;
        }
    }
    e->get<CSound>()->volumn = 1.f/(double)nInstances;
}

void SoundSystem::onPlayMusic(Entity* e){
    //if (mute) return;
    if (eNextMusic) eManager->removeEntity(eNextMusic);
    if (toUpper(e->get<CMusic>()->name) == "NONE") return;
    eNextMusic = e;
    eNextMusic->add(new CSystem());

    if (e->get<CMusic>()->name.empty() || !Assets::getMusic(e->get<CMusic>()->name)){
        printf("Music not found: %s\n", e->get<CMusic>()->name.c_str());
        eManager->removeEntity(eNextMusic);
        eNextMusic = nullptr;
        return;
    }
}

void SoundSystem::updateMusic(){
    if (eNextMusic && eNowPlaying && !fadingOut){
        startMusicFadeOut();
    }else if (eNextMusic && !eNowPlaying){
        eNowPlaying = eNextMusic;
        eNextMusic = nullptr;
        startPlayingMusic();
    }

    if (eNowPlaying) updateMusicVolume();

    if (eNowPlaying){
        sf::Music* music = Assets::getMusic(eNowPlaying->get<CMusic>()->name);
        if (!music){
            eManager->removeEntity(eNowPlaying);
            eNowPlaying = nullptr;
        }
        if (music->getStatus() == sf::Music::Stopped && eNowPlaying->get<CMusic>()->endTrigger == CMusic::STOP){
            //printf("C\n");
            eManager->removeEntity(eNowPlaying);
            eNowPlaying = nullptr;
        }else if (music->getStatus() == sf::Music::Stopped && eNowPlaying->get<CMusic>()->endTrigger == CMusic::NEXT){
            //eManager->removeEntity(eNowPlaying);
            //eNowPlaying = nullptr;
        }else if (music->getStatus() == sf::Music::Stopped && eNowPlaying->get<CMusic>()->endTrigger == CMusic::NEXT_RANDOM){

        }else if (music->getStatus() == sf::Music::Stopped && eNowPlaying->get<CMusic>()->endTrigger == CMusic::LOOP){
            //eManager->removeEntity(eNowPlaying);
            //eNowPlaying = nullptr;
        }
        if (eNowPlaying->get<CMusic>()->endTrigger == CMusic::LOOP){

        }else if (eNowPlaying->get<CMusic>()->volume == 0.0 && eNextMusic){
            Assets::getMusic(eNowPlaying->get<CMusic>()->name)->stop();
            eManager->removeEntity(eNowPlaying);
            eNowPlaying = nullptr;
        }
    }
    if (!eNowPlaying) fadingOut = false;
}

void SoundSystem::startMusicFadeOut(){
    eNowPlaying->get<CMusic>()->duration = Assets::getMusic(eNowPlaying->get<CMusic>()->name)->getPlayingOffset().asSeconds();
    fadingOut = true;
}

void SoundSystem::onStopMusic(Entity* e){
    if (!eNowPlaying) return;
    startMusicFadeOut();
}

void SoundSystem::updateMusicVolume(){
    Entity* e = eNowPlaying;
    if (!eNowPlaying) return;
    if (!Assets::getMusic(eNowPlaying->get<CMusic>()->name)) return;

    double duration = e->get<CMusic>()->duration;
    double timeElapsed = e->get<CMusic>()->clock.getElapsedTime().asSeconds();
    double fadeInLength = musFadeInLength;
    double fadeOutLength = musFadeOutLength;
    double maxVolume = mute ? 0.0 : config.getMusMaxVolume();
    double fadeSpeed = 0.f;
    if (timeElapsed < fadeInLength){
        fadeSpeed = maxVolume/fadeInLength;
    }else if (duration - timeElapsed < fadeOutLength){
        fadeSpeed = -maxVolume/fadeOutLength;
    }else{
        e->get<CMusic>()->volume = maxVolume;
    }

    e->get<CMusic>()->volume += fadeSpeed*delay;
    e->get<CMusic>()->volume = min(e->get<CMusic>()->volume, maxVolume);
    e->get<CMusic>()->volume = max(e->get<CMusic>()->volume, 0.0);

    Assets::getMusic(e->get<CMusic>()->name)->setVolume(e->get<CMusic>()->volume);

    if (fadeSpeed < 0.0 && e->get<CMusic>()->volume == 0.0){
        eManager->removeEntity(e);
        eNowPlaying = nullptr;
        fadingOut = false;
    }
}

void SoundSystem::startPlayingMusic(){
    if (eNowPlaying->get<CMusic>()->endTrigger == CMusic::LOOP){
        Assets::getMusic(eNowPlaying->get<CMusic>()->name)->setLoop(true);
        //printf("OK\n");
    }else{
        Assets::getMusic(eNowPlaying->get<CMusic>()->name)->setLoop(false);
    }
    eNowPlaying->get<CMusic>()->volume = 0;
    if (eNowPlaying->get<CMusic>()->duration <= 0){
        eNowPlaying->get<CMusic>()->duration = INFINITY;
    }
    eNowPlaying->get<CMusic>()->clock.restart();
    Assets::getMusic(eNowPlaying->get<CMusic>()->name)->play();
}

void SoundSystem::onNewCommandLine(Entity* e){
    if (e->get<CCommandLine>()->command == "play-music"){
        if (e->get<CCommandLine>()->hasParam("name")){
            string musicName = e->get<CCommandLine>()->params["name"];
            Entity* eMusic = eManager->createEntity();
            eMusic->add(new CMusic(musicName));
            notify(PLAY_MUSIC, eMusic);
        }else{
            printf("Command play-music help:\n");
            printf("play-music -name <NAME>\n");
        }
    }
}

void SoundSystem::onWindowClosed(Entity* e){
    if (eNowPlaying){
        Assets::getMusic(eNowPlaying->get<CMusic>()->name)->stop();
        eManager->removeEntity(eNowPlaying);
        eNowPlaying = nullptr;
    }
}

void SoundSystem::onWindowLostFocus(Entity* e){
    for (Entity* e : entities){
        eManager->removeEntity(e);
    }

    if (eNowPlaying){
        eNowPlaying->get<CMusic>()->volume = 0.0;
        mute = true;
    }
}

void SoundSystem::onWindowGainedFocus(Entity* e){
    if (eNowPlaying){
        eNowPlaying->get<CMusic>()->volume = config.getMusMaxVolume();
        mute = false;
    }
}
