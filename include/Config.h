#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include "tinyxml2.h"
#include "Standard.h"

class Config{
    public:
        Config();
        ~Config();

        sf::Vector2u getResolution();
        bool getFullscreen();
        double getMusMaxVolume();
        double getSfxMaxVolume();
        bool getDeadBodies();
        bool getSkipIntro();

        void setResolution(unsigned w, unsigned h);
        void setFullscreen(bool value);
        void setMusMaxVolume(double value);
        void setSfxMaxVolume(double value);
        void setDeadBodies(bool value);
        void setSkipIntro(bool value);

        bool loadConfigFile(std::string file);
        bool saveConfigFile(std::string file);
        void validateValues();
        void setWindowIcon(sf::Texture* texture);
        sf::Image& getWindowIcon();

    private:

        sf::Vector2u resolution;
        bool fullscreen;
        double musMaxVolume;
        double sfxMaxVolume;
        bool deadBodies;
        sf::Image windowIcon;

        bool skipIntro;
        list<string> secretOptions;

};

#endif // CONFIG_H
