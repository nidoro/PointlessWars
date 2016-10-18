#ifndef ASSETS_H
#define ASSETS_H
#include <map>
#include <cstring>
#include <string>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <cstdio>
#include <fstream>
#include <list>
#include <sstream>
#include <iostream>
#include "Standard.h"
#include "Messages.h"
#include "Component.h"
#include "tinyxml2.h"
#include <boost/filesystem.hpp>

using namespace tinyxml2;
namespace fs = boost::filesystem;

const int wWorld = 1280;
const int hWorld = 720;

struct Animation{
    Animation(){
        loop = true;
    }

    Animation(string id, double w, double h, int n, double rate){
        this->id = id;
        this->wSprite = w;
        this->hSprite = h;
        this->nFrames = n;
        this->rate = rate;
        this->duration = rate*nFrames;
    }

    typedef string ID;
    ID id;
    double wSprite, hSprite;
    int nFrames;
    double rate;
    vector<sf::Sprite> frames;
    bool loop;
    double duration;
    Message endMessage;
};

struct Buff{
    enum ID{
        NONE,
        FORMATION_SQUARE,
        FORMATION_OVAL,
        FORMATION_SPEAR,
        FORMATION_OPEN
    };
    enum Type{FORMATION = 1, POSITIVE, NEGATIVE};
    Type type;
    ID id;
    double rangedBonus;
    double meleeBonus;
    double siegeBonus;
    double defenseBonus;
    string icon;
    string description;
};

struct Action{
    enum ID{
        NONE,
        RANGED_ATK,
        MELEE_DEF,
        SHOOT_ARROW,
        RAISE_SHIELD,
        THROW_STONE,
        N_ACTIONS
    };
    ID id;
    string btDefTexture;
    string btHovTexture;
    string btActTexture;
    string strText, strTitle;
    Buff::ID buff;
};

struct Item{
    enum ID{BOW, SWORD, SHIELD, LIGHT_ARMOR, HEAVY_ARMOR, N_ITEMS};
    Item(double v, Action::ID a = Action::NONE){
        value = v;
        action = a;
    }
    double value;
    Action::ID action;
};

struct Unit{
    typedef int ID;

    enum Type{MELEE = 1, RANGED, MAGIC, N_UTYPES};

    Unit(){
        animations.resize(N_ANIMATIONS);
        items.resize(Item::N_ITEMS, false);
    }

    enum Animation{IDLE, WALK, DEATH, DEAD, ACTION, RAISE_SHIELD, SHIELD,  N_ANIMATIONS};

    ID id;
    string strName;
    Type type;
    Action::ID action;
    string texture;
    string projTexture;
    int attack;
    int defense;
    double speed;
    double width, height;
    string icon;
    bool hasAction;

    int melee;
    int armor;
    int intimidation;
    int rMinDamage;
    int rMaxDamage;
    int mDamage;

    int damage;
    int cResist;
    int rResist;
    int mResist;

    vector<string> animations;
    vector<bool> items;
};

struct GridCoord{
    GridCoord(){
        x = y = 0;
    };
    GridCoord(int x, int y){
        this->x = x;
        this->y = y;
    }
    int x;
    int y;
};

struct Formation{
    enum ID{
        SQUARE,
        OPEN,
        SPEAR,
        OVAL,
        N_FORMATIONS
    };
    vector<GridCoord> spots;
    double meleeBonus;
    double rangedBonus;
    double siegeBonus;
    double defenseBonus;
    string btDefTexture;
    string btHovTexture;
    string btActTexture;
    string strText, strTitle;
    Buff::ID buff;
};

struct Projectile{
    enum ID{
        ARROW=1
    };
    ID id;
    string texture;
    double speed;
    double width, height;
};

struct BattleSolving{
    enum ID{MELEE_BATTLE, MAN_VS_MAN, CHAMPIONS_DUEL, DRAW, N_SOLVINGS};
    ID id;
    string btDefTexture;
    string btHovTexture;
    string btActTexture;
    string strText, strTitle;
};

struct NinePatch{
    NinePatch(){
        parts = vector< vector<sf::Sprite> >(3, vector<sf::Sprite>(3));
        wMin = 0;
        hMin = 0;
    }

    vector< vector<sf::Sprite> > parts;
    double wMin, hMin;
};

class Assets{
    public:
        Assets();
        ~Assets();

        static void load();
        static void shutdown();
        static sf::Texture* getTexture(std::string file);
        static Formation getFormation(Formation::ID f);
        static void createBattleSolvings();
        static void createColors();

        static void readUnits();
        static void readObjects();
        static void readScenarios();

        static void readActions();
        static void readFonts();
        static void readBuffs();
        static void readProjectiles();
        static void readAnimations();
        static void readItems();
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

        static const Buff& getBuff(Buff::ID id);
        static const Unit& getUnit(Unit::ID id);
        static const Action& getAction(Action::ID id);
        static const Projectile& getProjectile(Projectile::ID id);
        static Animation& getAnimation(Animation::ID id);
        static CChar& getChar(Animation::ID id);
        static const Item& getItem(Item::ID id);
        static const BattleSolving& getBattleSolving(BattleSolving::ID id);
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
        static void writeTextureMap(ofstream& file);
        static void writeTexturesAt(ofstream& file, fs::path p);
        static void readTextureMap(ifstream& file);

        static map<string, NinePatch> ninePatches;
        static map<string, sf::Texture*> textures;
        static map<string, sf::Font> fonts;
        static vector<Formation> formations;
        static map<Unit::ID, Unit> units;
        static map<Action::ID, Action> actions;
        static map<Buff::ID, Buff> buffs;
        static map<Projectile::ID, Projectile> projectiles;
        static map<Animation::ID, Animation> animations;
        static map<Item::ID, Item> items;
        static map<BattleSolving::ID, BattleSolving> solvings;
        static map<string, string> stringMap;
        static map<string, sf::Music*> musics;
        static map<string, sf::SoundBuffer*> sounds;

};

char* mallocFile(string filename, int& length);

#endif // ASSETS_H
