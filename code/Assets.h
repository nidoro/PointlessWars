#ifndef ASSETS_H
#define ASSETS_H

#include <map>
#include <cstring>
#include <string>
#include <cstdio>
#include <fstream>
#include <list>
#include <sstream>
#include <iostream>
#include <experimental/filesystem>

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "Standard.h"
#include "Messages.h"
#include "Component.h"
#include "tinyxml2.h"
#include "json.hpp"
#include "OSAppDataDir.h"

namespace fs = std::experimental::filesystem;

struct Animation {
    Animation() {
        loop = true;
    }

    Animation(string id, double w, double h, int n, double rate) {
        this->id = id;
        this->wSprite = w;
        this->hSprite = h;
        this->nFrames = n;
        this->rate = rate;
        this->duration = rate*nFrames;
    }

    typedef string ID;
    ID id;
    int wSprite, hSprite;
    int nFrames;
    double rate;
    vector<sf::Sprite> frames;
    bool loop;
    double duration;
    Message endMessage;
};

struct NinePatch {
    NinePatch() {
        parts = vector< vector<sf::Sprite> >(3, vector<sf::Sprite>(3));
        wMin = 0;
        hMin = 0;
    }

    vector< vector<sf::Sprite> > parts;
    vector< vector<sf::Texture*> > textureParts;
    double wMin, hMin;
};

char* mallocFile(string filename, int& length);

class Assets {
    public:
        Assets();
        ~Assets();

        static void load();
        static void shutdown();
        static sf::Texture* getTexture(std::string file, bool returnNullIfMissing = false);
        static void createColors();

        static void readObjects();
        static void readScenarios();

        static void readActions();
        static void readAnimations();
        static void readStrings(string lan = "en");
        static void readSounds();
        static void readMusics();
        static void readChars();

        static void createUnits();
        static void createCaptains();
        static void createActions();
        static void createSprites();
        static void packResources(string file);
        static void unpackResources(string file);

        static list<string> getBackgrounds();

        static sf::Font& getFont(string name);
        static string getPrimaryFont();

        static void readTextures();
        static void loadTexturesAt(fs::path p);
        static void loadSoundsAt(fs::path p);
        static void loadMusicsAt(fs::path p);
        static void loadFontsAt(fs::path p);
        static void loadScenariosAt(fs::path p);
        static void createNinePatches();

        static const NinePatch& getNinePatch(string id);

        static Animation& getAnimation(Animation::ID id);
        static CChar& getChar(Animation::ID id);
        static const string getString(string id);
        static sf::SoundBuffer* getSound(string id);
        static sf::Music* getMusic(string id);
        static int getUnitsCount();

        static vector<sf::Color> colors;
        static vector< vector<sf::Color> > scale;
        static string primaryFont;
        static string secondaryFont;
        static string rscRoot;

    private:
        static int getFilesCount(fs::path path, list<string> extensions);
        static void writeFilesAt(ofstream& file, fs::path p, list<string> extensions);
        static void writeTexturesAt(ofstream& file, fs::path p);
        static void writeSoundsAt(ofstream& file, fs::path p);
        static void writeMusicAt(ofstream& file, fs::path p);
        static void writeFontsAt(ofstream& file, fs::path p);

        static void readTextureMap(ifstream& file);
        static void readSoundsMap(ifstream& file);
        static void readMusicMap(ifstream& file);
        static void readFontsMap(ifstream& file);

        static map<string, NinePatch> ninePatches;
        static map<string, sf::Texture*> textures;
        static map<string, sf::Font> fonts;
        static map<Animation::ID, Animation> animations;
        static map<string, string> stringMap;
        static map<string, sf::Music*> musics;
        static map<string, sf::SoundBuffer*> sounds;

        static map<string, char*> musicBuffers;
        static map<string, char*> fontBuffers;

        static std::string appDataDir;
};

#endif // ASSETS_H
