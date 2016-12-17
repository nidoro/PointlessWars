#ifndef CONFIG_H
#define CONFIG_H

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include "tinyxml2.h"
#include "Standard.h"
#include "OSAppDataDir.h"

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
        string getQuickStart();
        string getLanguage();
        std::list<std::string> getSupportedLanguages();
        std::string isLanguageSupported(std::string lan);
        
        void setResolution(unsigned w, unsigned h);
        void setFullscreen(bool value);
        void setMusMaxVolume(double value);
        void setSfxMaxVolume(double value);
        void setDeadBodies(bool value);
        void setSkipIntro(bool value);
        void setQuickStart(string value);
        void setLanguage(string lan);
        
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

        string quickStart;
        bool skipIntro;
        list<string> secretOptions;
        string language;
        std::list<std::string> supportedLanguages;
};

#endif // CONFIG_H
