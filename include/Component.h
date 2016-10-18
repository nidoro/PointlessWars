#ifndef COMPONENT_H
#define COMPONENT_H
#include <cstring>
#include <cmath>
#include <list>
#include <vector>
#include <map>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <SFML/Network.hpp>
#include "Messages.h"
#include "Standard.h"
class Entity;

struct Component{
    public:
        enum Type{
            NO_COMPONENT,
            BUTTON_STATE,
            BUTTON_HITBOX,
            HOVER_TEXTURE,
            BUTTON_SOUNDS,
            PERSON_ID,
            FAMILY,
            CHILDREN,
            ACTIVITIES,
            SKILLS,
            PARENTS,
            POSITION,
            ANCHOR,
            TEXTURE,
            DEFAULT_TEXTURE,
            PORTRAIT,
            DIMENSIONS,
            ACTIVE_TEXTURE,
            CIRCLE_LIST,
            OPTION_BOX,
            OPTION_CELL,
            BUTTON_TRIGGER,
            ENTITY_CONTAINER,
            ARMY,
            SKILL_OPTIONS,
            FACTION,
            UNIT,
            STRATEGY,
            ANGULAR_VELOCITY,
            VELOCITY,
            ROTATION,
            TARGET_POSITION,
            TARGET_ANGLE,
            TEXT_BOX,
            BAR_HUD,
            TARGET_VALUE,
            DRAW,
            PATH,
            PROJECTILE,
            ACCELERATION,
            TARGET_VELOCITY,
            GRAVITY,
            ACTION,
            ANIMATION,
            CURSOR,
            TIMER,
            BATTLE_PHASE,
            TEMPORARY,
            PLAYER,
            TOOLTIP,
            TEXT_BOX_2,
            CIRCLE_SHAPE,
            RECT_SHAPE,
            FADE_TRANSITION,
            FADE,
            UNIT_OPTIONS,
            SPIN_BUTTON,
            SPIN_BUTTON_VALUE,
            PANEL,
            UNIT_TOOLTIP,
            ARMY_HUD,
            DISPLAYER,
            UNIT_HIGHLIGHT,
            UNIT_HIGHLIGHT_2,
            ARMY_ACTION,
            ARMY_POLYGON,
            SIMPLE_BUTTON,
            ARMY_VIEW,
            SECTOR,
            BATTLE,
            ACTION_BUTTON,
            COMMAND_OPTION,
            CAPTAIN,
            FORMATION,
            BATTLE_CLOSURE,
            ACTION_OUTCOME,
            ACTOR,
            ARMY_COMPOSITION,
            SCENARIO_OBJECT,
            SOUND,
            SCENARIO,
            SCROLL_LIST,
            SCROLL_BUTTON,
            SCENARIO_PREVIEW,
            INPUT_TEXT_BOX,
            MOUSE_INPUT,
            TEXT_INPUT,
            KEYBOARD_INPUT,
            DRAG_DROP,
            PARENT_PANEL,
            SCENE,
            OWNER,
            HIGHLIGHT_TRIGGER,
            CALLOUT,
            BAR_DISPLAYER,
            AVERAGE_UNIT,
            SCREEN,
            SYSTEM,
            PARTICLE_EMMITER,
            PARTICLE_EFFECT,
            AUTO_PILOT,
            TYPING_EFFECT,
            TOGGLE_BUTTON,
            DROP_LIST,
            DROP_LIST_CELL,
            CHECK_BOX,
            MAIN_MENU_PAGE,
            PROFILE,
            ORIGINAL_HERO,
            AI,
            ZOOM,
            BUTTON_TEXTURES,
            CHAR,
            PROFILE_EDITOR,
            CAMERA_SHAKE,
            IN_GAME_MENU_PAGE,
            SLIDER,
            REMOTE_PLAYER,
            REMOTE_CONTROLER,
            REMOTELY_CONTROLED,
            PACKET,
            MUSIC,
            NINE_PATCH,
            COMMAND_LINE,
            UI_LAYER,
            RECT_BUTTON,
            CIRCLE_BUTTON,
            ELIPSOIDAL_MOVEMENT,
            CAMERA_MAN,
            STRING_TOGGLE_BUTTON,
            NUM_COMPONENTS
        };

        Component(){
            //componentType = NO_COMPONENT;
        }
        virtual ~Component(){}

        Component(Type type){
            //componentType = type;
        }

};

enum PlayerColor{
    GREY,
    BLACK,
    WHITE,
    BLUE,
    RED,
    GREEN,
    YELLOW,
    N_COLORS
};

struct Player{
    enum ID{A, B, NONE, BOTH};
};

struct CProfile : public Component{
    static Type getType(){return PROFILE;}
    CProfile(){}
    string name;
    string charName;
};

struct CChar : public Component{
    static Type getType(){return CHAR;}
    static map<string, CChar> Map;
    string name;
    string aIdle;
    string aWalk;
};

struct CProfileEditor : public Component{
    static Type getType(){return PROFILE_EDITOR;}
    CProfileEditor(Entity* eName, Entity* eChar){
        this->eName = eName;
        this->eChar = eChar;
    }
    Entity* eName;
    Entity* eChar;
};

struct CPosition : public Component{
    static Type getType(){return POSITION;};
    CPosition():Component(POSITION){
        x = y = 0;
    }
    CPosition(double x, double y):Component(POSITION){
        this->x = x;
        this->y = y;
    }
    double x;
    double y;
};

struct CTexture : public Component{
    static Type getType(){return TEXTURE;};
    CTexture(){
        hFlip = false;
        vFlip = false;
    }
    CTexture(std::string file, bool hFlip = false, bool vFlip = false){
        this->file = file;
        this->hFlip = hFlip;
        this->vFlip = vFlip;
    }
    string file;
    bool hFlip;
    bool vFlip;
};

struct CNinePatch : public Component{
    static Type getType(){return NINE_PATCH;}
    CNinePatch(string file){
        this->file = file;
    }

    string file;
};

struct CButtonHitbox : public Component{
    static Type getType(){return BUTTON_HITBOX;};
    enum Shape{
        CIRCLE,
        RECT
    };

    CButtonHitbox():Component(BUTTON_HITBOX){
        shape = RECT;
        width = 0;
        height = 0;
        radius = 0;
    }
    CButtonHitbox(double w, double h):Component(BUTTON_HITBOX){
        shape = RECT;
        width = w;
        height = h;
        xOff = yOff = 0;
    }
    CButtonHitbox(double r):Component(BUTTON_HITBOX){
        shape = CIRCLE;
        radius = r;
        xOff = yOff = r;
    }
    double xOff, yOff;
    double width;
    double height;
    double radius;
    Shape shape;
};

struct CUILayer : public Component{
    static Type getType(){return UI_LAYER;};
    enum Layer{NONE, L1, L2, L3, L4, L5, N_LAYERS};

    CUILayer(){
        layer = NONE;
    }
    CUILayer(Layer l){
        layer = l;
    }

    Layer layer;
};

struct CSimpleButton : public Component{
    static Type getType(){return SIMPLE_BUTTON;}

    CSimpleButton(double w, double h, sf::Color def, sf::Color hov, sf::Color act){
        shape.setSize(sf::Vector2f(w, h));
        shape.setOutlineColor(sf::Color::Black);
        shape.setOutlineThickness(3);
        defaultColor = def;
        hoverColor = hov;
        activeColor = act;
    }

    sf::RectangleShape shape;
    sf::Color defaultColor;
    sf::Color hoverColor;
    sf::Color activeColor;

};

struct CRectButton : public Component{
    static Type getType(){return RECT_BUTTON;}

    CRectButton(){}

    CRectButton(sf::RectangleShape defRect, sf::RectangleShape hovRect, sf::RectangleShape actRect){
        this->defRect = defRect;
        this->hovRect = hovRect;
        this->actRect = actRect;
    }

    sf::RectangleShape defRect;
    sf::RectangleShape hovRect;
    sf::RectangleShape actRect;
};

struct CCircleButton : public Component{
    static Type getType(){return CIRCLE_BUTTON;}

    CCircleButton(){}

    CCircleButton(sf::CircleShape defCircle, sf::CircleShape hovCircle, sf::CircleShape actCircle){
        this->defCircle = defCircle;
        this->hovCircle = hovCircle;
        this->actCircle = actCircle;
    }

    sf::CircleShape defCircle;
    sf::CircleShape hovCircle;
    sf::CircleShape actCircle;
};

struct CSlider : public Component{
    static Type getType(){return SLIDER;}
    enum Direction{HORIZONTAL, VERTICAL};

    CSlider(double x, double y, double length, double value, Direction direction = HORIZONTAL, string var = ""){
        this->x = x;
        this->y = y;
        this->length = length;
        this->value = value;
        this->direction = direction;
        sliding = false;
        variable = var;
    }

    double x;
    double y;
    double length;
    double value;
    bool sliding;
    Direction direction;
    string variable;
};

struct COptionBox : public Component{
    static Type getType(){return OPTION_BOX;}
    enum State{
        CHOOSING,
        CHOSEN
    };
    enum Direction{
        UP,
        DOWN,
        LEFT,
        RIGHT
    };

    enum Data{CAPTAIN, FORMATION, ACTION};

    COptionBox(){
        state = CHOSEN;
        selected = 0;
        owner = Player::NONE;
    }
    COptionBox(double d, Direction dir = DOWN){
        state = CHOSEN;
        distance = d;
        direction = dir;
        selected = 0;
        offset.x = 0;
        offset.y = 0;
        switch (dir){
            case DOWN: offset.y = d; break;
            case UP: offset.y = -d; break;
            case LEFT: offset.x = -d; break;
            case RIGHT: offset.x = d; break;
            default: break;
        }
        owner = Player::NONE;
        data = ACTION;
    }
    void addOption(int op){
        options.push_back(op);
    }

    State state;
    std::list<int> options;
    int selected;
    std::list<Entity*> cells;
    Direction direction;
    double distance;
    sf::Vector2f offset;
    Player::ID owner;
    Data data;
};
struct CAnchor : public Component{
    static Type getType(){return ANCHOR;};
    CAnchor(){
        xOff = 0.0;
        yOff = 0.0;
    }
    CAnchor(double xOff, double yOff){
        this->xOff = xOff;
        this->yOff = yOff;
    }

    double xOff;
    double yOff;

};

struct CButtonTextures : public Component{
    static Type getType(){return BUTTON_TEXTURES;}

    CButtonTextures(string def, string hov = string(), string act = string()){
        this->def = def;
        this->hov = hov;
        this->act = act;
        if (hov.empty()) this->hov = this->def;
        if (act.empty()) this->act = this->hov;
    }

    CButtonTextures(){}

    string def;
    string hov;
    string act;
};

struct CButtonState : public Component{
    static Type getType(){return BUTTON_STATE;};
    enum State{
        NON_ACTIVE,
        HOVERED,
        ACTIVE,
        LOCKED
    };

    CButtonState():Component(BUTTON_STATE){
        state = NON_ACTIVE;
        gainedFocusMessage = EMPTY_MESSAGE;
        lostFocusMessage = EMPTY_MESSAGE;
    }
    CButtonState(State s){
        state = s;
        gainedFocusMessage = EMPTY_MESSAGE;
        lostFocusMessage = EMPTY_MESSAGE;
    }

    State state;
    Message gainedFocusMessage;
    Message lostFocusMessage;
};

struct CButtonTrigger : public Component{
    static Type getType(){return BUTTON_TRIGGER;}

    enum Action{ON_PRESS, ON_RELEASE};

    CButtonTrigger():Component(BUTTON_TRIGGER){
        message = EMPTY_MESSAGE;
        action = ON_RELEASE;
    }
    CButtonTrigger(Message trigger, Action a = ON_RELEASE, sf::Keyboard::Key hk = sf::Keyboard::Unknown){
        message = trigger;
        action = a;
        hotkey = hk;
        mouseButton = sf::Mouse::Left;
    }

    Message message;
    Action action;
    sf::Keyboard::Key hotkey;
    sf::Mouse::Button mouseButton;
};

struct CHoverTexture : public Component{
    static Type getType(){return HOVER_TEXTURE;};
    CHoverTexture():Component(HOVER_TEXTURE){}

    CHoverTexture(std::string tex):Component(HOVER_TEXTURE){
        this->file = tex;
    }

    string file;
};

struct CDefaultTexture : public Component{
    static Type getType(){return DEFAULT_TEXTURE;};
    CDefaultTexture():Component(DEFAULT_TEXTURE){}

    CDefaultTexture(std::string tex):Component(DEFAULT_TEXTURE){
        this->file = tex;
    }
    string file;
};

struct CActiveTexture : public Component{
    static Type getType(){return ACTIVE_TEXTURE;};
    CActiveTexture():Component(ACTIVE_TEXTURE){}

    CActiveTexture(std::string tex):Component(ACTIVE_TEXTURE){
        this->file = tex;
    }

    string file;
};

struct CButtonSounds : public Component{
    static Type getType(){return BUTTON_SOUNDS;}

    CButtonSounds(){}

    CButtonSounds(string focused, string pressed = "", string released = ""){
        this->focused = focused;
        this->pressed = pressed;
        this->released = released;
    }

    string focused;
    string pressed;
    string released;
};

struct CDimensions : public Component{
    static Type getType(){return DIMENSIONS;};
    CDimensions():Component(DIMENSIONS){}
    CDimensions(double w, double h):Component(DIMENSIONS){
        width = w;
        height = h;
    }

    double width;
    double height;
};

struct CAnimation : public Component{
    static Type getType(){return ANIMATION;}

    CAnimation(bool hFlip, string aID, bool vFlip = false){
        this->hFlip = hFlip;
        this->vFlip = vFlip;
        current = aID;
        frame = 0;
        update = true;
        freeze = false;
    }

    sf::Clock clock;
    string current;
    sf::Sprite sprite;
    int frame;
    bool hFlip, vFlip;
    bool update;
    bool freeze;
};

struct CAction : public Component{
    static Type getType(){return ACTION;}

    enum EffectType{NONE, BUFF, DEBUFF};

    typedef int ID;

    static map<CAction::ID, CAction> Map;

    CAction(){}
    CAction(ID i, EffectType eff = NONE, int dur = 1, bool oneBattle = false, bool oneWar = false){
        id = i;
        btDefTexture = "button-action-" + int2str(0, 2) + "-default.png";
        btHovTexture = "button-action-" + int2str(0, 2) + "-hovered.png";
        btActTexture = "button-action-" + int2str(0, 2) + "-active.png";

        strName = "NAME-ACTION-" + int2str(i);
        strDescription = "DESCRIPTION-ACTION-" + int2str(i);
        targets = 10;
        effectValue = 0;
        effectType = eff;
        duration = dur;
        oneTimeBattle = oneBattle;
        oneTimeWar = oneWar;
    }

    ID id;

    string btDefTexture;
    string btHovTexture;
    string btActTexture;

    string icon;
    string name;
    string description;
    string damage;
    string strName;
    string strDescription;
    int damageType;
    int targets;

    int effectValue;
    int duration;
    bool oneTimeBattle;
    bool oneTimeWar;
    EffectType effectType;
};

struct COwner : public Component{
    static Type getType(){return OWNER;}
    COwner(Entity* e){
        this->e = e;
    }
    Entity* e;
};

struct CUnit : public Component{
    static Type getType(){return UNIT;}

    typedef int ID;
    static map<CUnit::ID, CUnit> Map;
    static int maxResistance;

    enum Role{COMMANDER, AGGRESSIVE, DEFENSIVE_SUPPORT, OFFENSIVE_SUPPORT, N_ROLES};
    enum DamageType{FIRE, WATER, EARTH, AIR, N_DAMAGE_TYPES};

    ID id;
    Role role;
    string actionDescription;

    CUnit(){
        resistance.resize(N_DAMAGE_TYPES, 0);
        realResist.resize(N_DAMAGE_TYPES, 0);
        displayer = "unit-displayer.png";

        char name[255];
        sprintf(name, "NAME-UNIT-%02d", id);
        strName = name;

        aIdle = "alpha.png";
        aWalk = "alpha.png";
        aDeath = "alpha.png";
        aDead = "alpha.png";
        aAction01 = "alpha.png";
        aAction02 = "alpha.png";

        dead = false;
        armyID = 0;
    }

    CUnit(ID i, DamageType d){
        id = i;
        damage = d;
        resistance.resize(N_DAMAGE_TYPES, 0);
        realResist.resize(N_DAMAGE_TYPES, 0);
        displayer = "unit-displayer.png";

        action = id;
        char name[255];
        sprintf(name, "NAME-UNIT-%02d", id);
        strName = name;

        aIdle = "egg-unit.png";
        aWalk = "egg-unit.png";
        aDeath = "egg-unit.png";
        aDead = "egg-unit-dead.png";
        aAction01 = "egg-unit.png";
        aAction02 = "egg-unit.png";

        dead = false;
        armyID = 0;
    }

    string displayer;
    vector<int> resistance;
    vector<int> realResist;
    DamageType damage;

    string strName;
    string texture;
    string icon;
    string portrait;
    bool hasAction;

    sf::Color color;
    string aIdle;
    string aWalk;
    string aDeath;
    string aDead;
    string aHurt;
    string aAction01;
    string aAction02;
    string aAction03;
    string aAction04;

    string btActionDef;
    string btActionHov;
    string btActionAct;

    CAction::ID action;
    map<CAction::ID, CAction> effects;

    bool dead;
    int armyID;
};

struct CCaptain : public Component{
    static Type getType(){return CAPTAIN;}
    typedef int ID;
    static map<CCaptain::ID, CCaptain> Map;

    CCaptain(){
        isConfined = false;
    }
    CCaptain(ID i){
        id = i;
        btDefTexture = "button-action-00-active.png";
        btHovTexture = "button-action-00-active.png";
        btActTexture = "button-action-00-active.png";

        aIdle = "square-unit.png";
        aWalk = "square-unit.png";
        isConfined = false;
        morale = 1;
    }

    ID id;

    string strName;
    string texture;
    string icon;
    string portrait;

    string aIntimidation;
    string aIdle;
    string aWalk;
    string aDeath;
    string aDead;
    string aAction;

    string btDefTexture;
    string btHovTexture;
    string btActTexture;

    list<CAction::ID> actions;

    bool isConfined;
    int morale;
};

struct CPlayer : public Component{
    static Type getType(){return PLAYER;}

    enum Side{LEFT, RIGHT};
    typedef int ID;

    Side side;

    Entity* enemy;
    CPlayer(){
    }
    CPlayer(Entity* enemy){
        this->enemy = enemy;
    }

    list<CCaptain::ID> heroDeck;
    list<CUnit::ID> unitDeck;
    PlayerColor color;
    string name;
    int maxPicks;
    int maxRepicks;
    double turnDuration;
    double maxTurnDuration;
    ID id;
};

struct CArmyView : public Component{
    static Type getType(){return ARMY_VIEW;}

    CArmyView(){
        sectors.resize(CUnit::N_ROLES);
        eArmy = nullptr;
    }

    CArmyView(Entity* a){
        sectors.resize(CUnit::N_ROLES);
        eArmy = a;
    }

    vector< vector<Entity*> > sectors;

    Entity* eArmy;
};

struct CSector : public Component{
    static Type getType(){return SECTOR;}

    CSector(){
        strength = 10;
    }

    CSector(CUnit unit, int stg = 10){
        this->unit = unit;
        strength = stg;
    }

    CUnit unit;
    int strength;
};

struct CArmy : public Component{
    static Type getType(){return ARMY;};

    enum Side{LEFT, RIGHT};

    enum Formation{F1, F2, F3, F4, N_FORMATIONS};

    CArmy(Side side){
        this->side = side;
        x = 0;
        captain = nullptr;
        ready = false;
        score = 0;
        nAlive = 0;
        nextUnitID = 0;
        hasCoin = false;
    }

    vector<int> resistance;

    ///HUD
    Formation formation;
    CAction::ID nextAction;

    map<CUnit::ID, int> unitCount;
    Entity* captain;

    map<CAction::ID, Entity*> effects;
    map<CAction::ID, CAction> armyEffects;

    map<CCaptain::ID, Entity*> captains;
    list<Entity*> allUnits;

    map<CUnit::ID, int> lastArmy;

    bool hasCoin;
    int nAlive;
    Side side;
    double x;
    bool ready;
    int repicks;
    int score;
    int nextUnitID;
};

struct CFaction : public Component{
    static Type getType(){return FACTION;};
    enum Faction{A, B};
    CFaction(){

    }
    CFaction(Faction f){
        faction = f;
    }
    Faction faction;
};

struct COptionCell : public Component{
    static Type getType(){return OPTION_CELL;};
    COptionCell():Component(OPTION_CELL){}

    COptionCell(Entity* box, int v):Component(OPTION_CELL){
        optionBox = box;
        value = v;
    }

    Entity* optionBox;
    int value;
};

struct CRotation : public Component{
    static Type getType(){return ROTATION;}
    CRotation(double angle){
        this->angle = angle;
        followVelocity = false;
        this->velocity = 0;
    }
    CRotation(bool vel){
        this->angle = 0;
        this->followVelocity = vel;
        this->velocity = 0;
    }
    double angle;
    double velocity;
    double maxVel;
    bool followVelocity;
};

struct CVelocity : public Component{
    static Type getType(){return VELOCITY;}
    CVelocity():Component(VELOCITY){}

    CVelocity(double sp){
        speed = sp;
    }

    CVelocity(double xMax, double yMax, double x = 0, double y = 0):Component(VELOCITY){
        this->xMax = xMax;
        this->yMax = yMax;
        this->x = x;
        this->y = y;

        if (this->xMax == 0.f) this->xMax = 1000;
        if (this->yMax == 0.f) this->yMax = 1000;
    }
    double x, y;
    double xMax, yMax;
    double speed;
};

struct CAutoPilot : public Component{
    static Type getType(){return AUTO_PILOT;}
    CAutoPilot(){}
};

struct CAcceleration : public Component{
    static Type getType(){return ACCELERATION;}
    CAcceleration():Component(ACCELERATION){}
    CAcceleration(double xMax, double yMax, double x = 0, double y = 0):Component(ACCELERATION){
        this->xMax = xMax;
        this->yMax = yMax;
        this->x = x;
        this->y = y;
        if (this->xMax == 0.f) this->xMax = 1000;
        if (this->yMax == 0.f) this->yMax = 1000;
    }
    double x, y;
    double xMax, yMax;
};

struct CGravity : public Component{
    static Type getType(){return GRAVITY;}
};

struct CTargetVelocity : public Component{
    static Type getType(){return TARGET_VELOCITY;}
    CTargetVelocity(double x, double y){
        this->x = x;
        this->y = y;
        xOK = false;
        yOK = false;
    }
    double x, y;
    bool xOK, yOK;
};

struct CTargetPosition : public Component{
    static Type getType(){return TARGET_POSITION;};
    CTargetPosition():Component(TARGET_POSITION){}
    CTargetPosition(double x, double y):Component(TARGET_POSITION){
        this->x = x;
        this->y = y;
        xOK = false;
        yOK = false;
    }
    double x;
    double y;

    bool xOK, yOK;
};

struct CTextBox : public Component{
    static Type getType(){return TEXT_BOX;};
    CTextBox():Component(TEXT_BOX){}
    CTextBox(string text, string font, sf::Color color, int size, double wMax = INFINITY, double hMax = INFINITY, int spacing = 0):Component(TEXT_BOX){
        this->spacing = spacing;
        this->hMax = hMax;
        this->wMax = wMax;
        this->size = size;
        this->font = font;
        this->color = color;
    }
    void updateDimensions(){
        width = 0; height = 0;
        for (unsigned int i = 0; i < lines.size(); i++){
            height += lines[i].getLocalBounds().height;
            if (lines[i].getLocalBounds().width > width){
                width = lines[i].getLocalBounds().width;
            }
        }
    }

    vector<sf::Text> lines;
    string font;
    sf::Color color;
    int size;
    double width, height;
    double wMax;
    double hMax;
    int spacing;
};

struct CBarHUD : public Component{
    static Type getType(){return BAR_HUD;};
    enum GrowDirection{LEFT, RIGHT, UP, DOWN};
    CBarHUD():Component(BAR_HUD){}
    CBarHUD(GrowDirection grow, double w0, double h0, double speed = 40,
            sf::Color fillColor = sf::Color::Black, sf::Color outlineColor = sf::Color::White,
            double outlineThickness = 0, double sizeUnit = 1, Entity* label = nullptr):Component(BAR_HUD){
        this->grow = grow;
        this->w0 = w0;
        this->h0 = h0;
        this->width = w0;
        this->height = h0;
        this->growSpeed = speed;
        this->fillColor = fillColor;
        this->outlineColor = outlineColor;
        this->outlineThickness = outlineThickness;
        this->sizeUnit = sizeUnit;
        this->label = label;
        this->value = 0;
    }
    GrowDirection grow;
    double width, height;
    double w0, h0;
    double growSpeed;
    double sizeUnit;
    sf::Color fillColor;
    sf::Color outlineColor;
    double outlineThickness;
    double value;
    Entity* label;
};

struct CCursor : public Component{
    static Type getType(){return CURSOR;}
};

struct CTargetValue : public Component{
    static Type getType(){return TARGET_VALUE;};
    CTargetValue(double value):Component(TARGET_VALUE){
        this->target = value;
    }
    double target;
};

struct CDraw : public Component{
    static Type getType(){return DRAW;};
    enum Tag{BACKGROUND, NONE, WORLD, WORLD_0, WORLD_1, WORLD_2, WORLD_3, SKY, HUD1, HUD2, HUD3, HUD4, HUD5,
            GUI, GUI1, GUI2, GUI3, GUI4, GUI5,
            GUI_00, GUI_01, GUI_02, GUI_03, GUI_04, GUI_05,
            PARTICLES, CLOUDS, BLINDFOLD, SCREEN_TRANSITION, CURSOR, N_TAGS};
    CDraw(){
        tag = NONE;
        isHidden = false;
        alpha = 255;
    }
    CDraw(Tag tag, double a = 255){
        this->tag = tag;
        isHidden = false;
        alpha = a;
    }
    CDraw(Tag tag, bool hidden, double a){
        this->tag = tag;
        isHidden = hidden;
        alpha = a;
    }
    Tag tag;
    bool isHidden;
    double alpha;
};

struct CPath : public Component{
    static Type getType(){return PATH;};

    CPath(){
        firstInsertion = true;
    }

    void addNode(double x, double y){
        if (firstInsertion){
            target = sf::Vector2f(x,y);
            firstInsertion = false;
        }else{
            nodes.push_back(sf::Vector2f(x, y));
        }
    }

    bool firstInsertion;

    sf::Vector2f target;
    list<sf::Vector2f> nodes;
};

struct CTimer : public Component{
    static Type getType(){return TIMER;}
    CTimer(double t, Message m, bool rem = false){
        message = m;
        target = t;
        removeEntity = rem;
    }
    sf::Clock clock;
    double target;
    Message message;
    bool removeEntity;
};

struct BattlePhase{
    enum ID{NONE, PRE_BATTLE, FORMATION_PLANNING, ACTION_PLANNING, BATTLE_SOLVING_PLANNING, FORMATION_EXECUTION,
            ACTION_EXECUTION, BATTLE_SOLVING_EXECUTION, ARMY_ASSEMBLE, RESTART};
    BattlePhase(){
        id = NONE;
    }
    BattlePhase(Player::ID p, ID i, double x = 0){
        player = p;
        id = i;
    }
    Player::ID player;
    ID id;
};

struct CBattlePhase : public Component{
    static Type getType(){return BATTLE_PHASE;}
    CBattlePhase(){}
    CBattlePhase(BattlePhase phase){
        this->phase = phase;
    }
    BattlePhase phase;
};

struct CTemporary : public Component{
    static Type getType(){return TEMPORARY;}
};

struct CTextbox2 : public Component{
    static Type getType(){return TEXT_BOX_2;}

    enum Align{NORMAL, CENTRALIZED};

    CTextbox2(){
        xOff = 0;
        yOff = 0;
        align = NORMAL;
        wMax = INFINITY;
    }

    CTextbox2(sf::Text t, double xOff = 0, double yOff = 0, Align a = NORMAL, double w = INFINITY){
        content = t;
        this->xOff = xOff;
        this->yOff = yOff;
        this->align = a;
        this->wMax = w;
    }

    CTextbox2(string str, sf::Font& font, int size, sf::Color color = sf::Color::Black, double xOff = 0, double yOff = 0, Align a = CENTRALIZED, double w = INFINITY){
        content.setString(str);
        content.setFont(font);
        content.setCharacterSize(size);
        content.setFillColor(color);
        this->xOff = xOff;
        this->yOff = yOff;
        this->align = a;
        this->wMax = w;
    }

    Align align;
    sf::Text content;
    double xOff;
    double yOff;
    double wMax;
};

struct CTooltip : public Component{
    static Type getType(){return TOOLTIP;}
    enum Style{STYLE_1, STYLE_2, N_STYLES};
    enum Position{TL, TC, TR, ML, MC, MR, BL, BC, BR};
    enum Info{SECTOR, UNIT_GROUP, UNIT, ACTION, EFFECT, CAPTAIN, TEXT_1};

    CTooltip(Info i){
        info = i;
        showAfter = 0;
        xOff = 0;
        yOff = 0;
        isShowing = false;
    }

    CTooltip(string text, string title = string(), double xOff = 0, double yOff = 0,
             double showAfter = 0.f, Style style = STYLE_1, Position position = MR){
        this->text = text;
        this->title = title;
        this->showAfter = showAfter;
        this->xOff = xOff;
        this->yOff = yOff;
        this->isShowing = false;
        this->style = style;
        this->position = position;
    }

    Info info;
    Style style;
    sf::Clock timeOnHover;
    double showAfter;
    string title;
    string text;
    double xOff;
    double yOff;
    bool isShowing;
    Position position;
};

struct CCircleShape : public Component{
    static Type getType(){return CIRCLE_SHAPE;}
    CCircleShape(sf::CircleShape circle){
        shape = circle;
    }
    CCircleShape(double r, sf::Color fillColor = sf::Color::White, double outThick = 0, sf::Color outColor = sf::Color::Black){
        shape.setRadius(r);
        shape.setFillColor(fillColor);
        shape.setOutlineThickness(outThick);
        shape.setOutlineColor(outColor);
    }
    sf::CircleShape shape;
};

struct CRectShape : public Component{
    static Type getType(){return RECT_SHAPE;}
    CRectShape(sf::RectangleShape rect){
        shape = rect;
    }
    CRectShape(double w, double h, sf::Color fillColor = sf::Color::White, double outThick = 0, sf::Color outColor = sf::Color::Black){
        shape.setSize(sf::Vector2f(w, h));
        shape.setFillColor(fillColor);
        shape.setOutlineThickness(outThick);
        shape.setOutlineColor(outColor);
    }
    sf::RectangleShape shape;
};

struct CFadeTransition : public Component{
    static Type getType(){return FADE_TRANSITION;}
    CFadeTransition(){
        color = sf::Color::White;
        holdTime = 0;
        endMessage = EMPTY_MESSAGE;
    }
    sf::Color color;
    Message endMessage;
    double holdTime;
};

struct CFade : public Component{
    static Type getType(){return FADE;}
    CFade(){
        alphaSpeed = 255;
        endMessage = EMPTY_MESSAGE;
    }
    CFade(double speed, double target, Message m = EMPTY_MESSAGE){
        active = false;
        alphaSpeed = speed;
        endMessage = m;
        alphaTarget = target;
    }

    double alphaSpeed;
    Message endMessage;
    bool active;
    double alphaTarget;
};

struct CSpinButton : public Component{
    static Type getType(){return SPIN_BUTTON;}

    enum Style{STYLE_1, STYLE_2, N_STYLES};

    CSpinButton(){
        step = 1;
        min = 0;
        max = 10;
    }

    CSpinButton(int value, int step, int min, int max){
        this->value = value;
        this->step = step;
        this->min = min;
        this->max = max;
        this->style = STYLE_1;
    }
    ///OBSERVED ENTITY: SpinButtonValue
    int value;
    int step;
    int min;
    int max;
    Style style;
    ///OBSERVED ENTITIES:
    /*
    Entity* eSpinMore;
    Entity* eSpinLess;
    Entity* eIcon;
    Entity* eDisplayer;
    */
};

struct CPanel : public Component{
    static Type getType(){return PANEL;}

    enum PanelType{TOOLTIP, WINDOW};

    CPanel(){
    }

    void remove(Entity* e){
        objects.remove(e);
    }
    void add(Entity* e){
        objects.push_back(e);
    }

    PanelType panelType;

    list<Entity*> objects;

    Entity* windowButton;
    Entity* toolButton;
};

struct CUnitTooltip : public Component{
    static Type getType(){return UNIT_TOOLTIP;}

/*
    CUnitTooltip(){
        showAfter = 0;
        isShowing = false;
        eTooltip = nullptr;
    }
    CUnitTooltip(Unit::ID id, double delay){
        unit = id;
        showAfter = delay;
        isShowing = false;
        eTooltip = nullptr;
    }

    Unit::ID unit;
    sf::Clock timeOnHover;
    double showAfter;
    bool isShowing;
    Entity* eTooltip;
*/
};

struct IconDisplayerPair{
    IconDisplayerPair(){
        eIcon = nullptr;
        eDisplayer = nullptr;
    }
    IconDisplayerPair(Entity* eIcon, Entity* eDisplayer){
        this->eIcon = eIcon;
        this->eDisplayer = eDisplayer;
    }
    Entity* eIcon;
    Entity* eDisplayer;
};

struct CArmyHUD : public Component{
    static Type getType(){return ARMY_HUD;}

    CArmyHUD(){
        captain = nullptr;
        eNAlive = nullptr;
        eCoin = nullptr;
    }

    map<CUnit::ID, IconDisplayerPair> units2;
    map<CUnit::ID, Entity*> units;
    map<CAction::ID, Entity*> effects;
    Entity* captain;
    vector<Entity*> resistBars;
    vector<Entity*> medals;
    Entity* eNAlive;
    Entity* eCoin;
};

struct CDisplayer : public Component{
    static Type getType(){return DISPLAYER;}

    enum ValueType{INPUT_TEXT, SECTOR_STRENGTH, ARMY_MORAL, ARMY_MELEE_POWER, ARMY_ARMOR, SPIN_BUTTON, UNIT_COUNT,
                    RESISTANCE_NEUTRAL, RESISTANCE_FIRE, RESISTANCE_WATER, RESISTANCE_ELECTRIC, BATTLE_STATE_DESCRIPTION,
                    DROP_LIST_VALUE, ALIVE_COUNT, STR_TOGGLE_BUTTON, GAME_STATE};

    CDisplayer(ValueType vType, Entity* e){
        valueType = vType;
        eValue = e;
    }

    ValueType valueType;
    Entity* eValue;
};

struct CUnitHighlight : public Component{
    static Type getType(){return UNIT_HIGHLIGHT;}

    enum State{DEFAULT, ACTOR, TARGET, N_STATES};

    CUnitHighlight(){
        colors.resize(N_STATES);
        colors[DEFAULT] = sf::Color::Transparent;
        colors[ACTOR] = sf::Color::White;
        colors[TARGET] = sf::Color::Black;
        xOff = 0;
        yOff = 16;
        state = DEFAULT;
    }

    CUnitHighlight(sf::Color color){
        colors.resize(N_STATES);
        colors[DEFAULT] = color;
        colors[ACTOR] = sf::Color::White;
        colors[TARGET] = sf::Color::Black;
        xOff = 0;
        yOff = 16;
        state = DEFAULT;
    }

    State state;
    vector<sf::Color> colors;
    double xOff, yOff;
};

struct CUnitHighlight2 : public Component{
    static Type getType(){return UNIT_HIGHLIGHT_2;}

    enum State{ON, OFF};
    enum Info{NONE, EFFECT, RESISTANCE, UNIT, ATTACK, ACTING};

    CUnitHighlight2(){
        this->color = sf::Color::White;
        xOff = 0;
        yOff = 16;
        state = OFF;
    }

    CUnitHighlight2(sf::Color color){
        this->color = color;
        xOff = 0;
        yOff = 16;
        state = OFF;
    }

    Info info;
    int value;

    State state;
    sf::Color color;
    double xOff, yOff;
};

struct CArmyPolygon : public Component{
    static Type getType(){return ARMY_POLYGON;}

    sf::ConvexShape shape;
};

struct BattleState{
    enum ID{NONE, COIN, ASK_FORMATION, PLAY_FORMATION, ADVANCE_ARMY,
                ASK_CAPTAIN_ACTION, PLAY_ACTION, BETWEEN_ROUNDS,
                ASK_ARMY_ACTION, ASK_BATTLE_CLOSURE, ASK_ARMY_ASSEMBLE,
                ASK_CAPTAIN_SELECTION, PLAY_MELEE_BATTLE, PRESENT_ARMY,
                FIELD_CLEANUP, BETWEEN_TURNS, ENDING, BEGINING, UPDATE_SCORE};
    BattleState(){}
    BattleState(ID id, Entity* a1 = nullptr, Entity* a2 = nullptr, Message m = EMPTY_MESSAGE){
        this->id = id;
        if (a1 != nullptr) this->actors.push_back(a1);
        if (a2 != nullptr) this->actors.push_back(a2);
        endSignal = m;
    }
    ID id;
    Message endSignal;
    list<Entity*> actors;
};

struct CBattle : public Component{
    static Type getType(){return BATTLE;}

    CBattle(Entity* P1, Entity* P2, Entity* eFirst){
        this->P1 = P1;
        this->P2 = P2;
        P1ready = false;
        P2ready = false;
        P1streak = 0;
        P2streak = 0;
        this->eFirst = eFirst;
        eWinner = nullptr;
        endBattle = false;
    }

    Entity* P1;
    Entity* P2;
    Entity* eFirst;

    Entity* eWinner;
    bool endBattle;

    BattleState state;

    bool P1ready;
    bool P2ready;

    int P1streak;
    int P2streak;

    string stateDescription;
};

struct CActionButton : public Component{
    static Type getType(){return ACTION_BUTTON;}
    CActionButton(Entity* e){
        army = e;
    }
    Entity* army;
};

struct CCommandOption : public Component{
    static Type getType(){return COMMAND_OPTION;}

    CCommandOption(Entity* e){
        eArmy = e;
    }

    Entity* eArmy;
};

struct CArmyComposition : public Component{
    static Type getType(){return ARMY_COMPOSITION;}
    CArmyComposition(list<Entity*> eOptions){
        this->eOptions = eOptions;
        composed = false;
    }
    list<Entity*> eOptions;
    bool composed;
    Entity* eArmy;
};

struct TargetOutcome{
    enum ID{BLOCKED, DIED};
    TargetOutcome(){}
    TargetOutcome(Entity* eCauser, ID id){
        this->eCauser = eCauser;
        this->id = id;
    }
    int idCauser;
    int idTarget;
    Entity* eCauser;
    Entity* eTarget;
    ID id;
};

struct CActionOutcome : public Component{
    static Type getType(){return ACTION_OUTCOME;}

    CAction::ID action;

    CActionOutcome(CAction::ID id, Entity* actor = nullptr, Entity* enemy = nullptr){
        action = id;
        actorArmy = actor;
        enemyArmy =  enemy;
        animDuration = 0;
    }

    Entity* actorArmy;
    Entity* enemyArmy;
    int idActor;
    int idEnemy;

    Entity* eWinner;
    Entity* eLoser;
    int idWinner;
    int idLoser;

    list<TargetOutcome> outcomes;

    double dValue;
    int iValue;
    string sValue;
    bool bValue;

    list<Entity*> actors;
    list<Entity*> targets;
    list<int> idTargets;
    list<int> idActors;
    int nTargets;
    int nActors;

    double animDuration;
};

struct CScene : public Component{
    static Type getType(){return SCENE;}

    void addActor(Entity* e){
        actors.push_back(e);
    }

    list<Entity*> actors;
};

struct CZoom : public Component{
    static Type getType(){return ZOOM;}

    CZoom(double dx, double dy = 0, double wt = -1, double ht = -1){
        this->dx = dx;
        this->dy = dy;
        this->wTarget = wt;
        this->hTarget = ht;
    }

    double dx;
    double dy;
    double wTarget;
    double hTarget;
};

struct CSound : public Component{
    static Type getType(){return SOUND;}
    enum EndTrigger{REMOVE_COMPONENT, REMOVE_ENTITY, LOOP, LOOP_THEN_REMOVE, STOP};
    CSound(){
        endTrigger = STOP;
    }
    CSound(string n, EndTrigger endTrigger = STOP, double fadeInLength = 0.f, double fadeOutLength = 0.f, double duration = 1000){
        name = n;
        this->endTrigger = endTrigger;
        if (endTrigger == LOOP || endTrigger == LOOP_THEN_REMOVE){
            sound.setLoop(true);
        }
        maxVolumn = 100;
        volumn = 100;
        this->duration = duration;
        this->fadeInLength = fadeInLength;
        this->fadeOutLength = fadeOutLength;
    }
    string name;
    sf::Sound sound;
    EndTrigger endTrigger;
    double fadeInLength;
    double fadeOutLength;
    double duration;
    double maxVolumn;
    double volumn;
    sf::Clock clock;
};


struct AnimationNode{
    enum Type{A_SPRITE_ANIMATION, A_FADE, A_DESTROY, A_MOVE, A_TELEPORT,
                A_VARIABLE, A_SPEAK, A_SHOOT, A_SOUND, A_ZOOM, A_NOTHING,
                A_ADD_COMPONENT, A_REMOVE_COMPONENT};
    AnimationNode(){}
    virtual ~AnimationNode(){}
    double triggerTime;
    Type type;
};

struct ASpriteAnimation : public AnimationNode{
    ASpriteAnimation(double trigger, string a, string s = ""){
        type = A_SPRITE_ANIMATION;
        triggerTime = trigger;
        animation = a;
        sound = s;
    }

    string animation;
    string sound;
};

struct ASpeak : public AnimationNode{
    ASpeak(double trigger, string what, double duration){
        type = A_SPEAK;
        triggerTime = trigger;
        text = what;
        this->duration = duration;
    }
    string text;
    double duration;
};

struct AMove : public AnimationNode{
    AMove(double trigger, double x, double y, double s = 0){
        type = A_MOVE;
        triggerTime = trigger;
        this->x = x;
        this->y = y;
        this->speed = s;
    }

    double x, y;
    double speed;
};

struct AAddComponent : public AnimationNode{
    AAddComponent(double trigger, Component* c, Component::Type cType){
        type = A_ADD_COMPONENT;
        triggerTime = trigger;
        this->c = c;
        this->cType = cType;
    }
    Component* c;
    Component::Type cType;
};

struct ARemoveComponent : public AnimationNode{
    ARemoveComponent(double trigger, Component::Type c){
        type = A_REMOVE_COMPONENT;
        triggerTime = trigger;
        this->c = c;

    }
    Component::Type c;
};

struct AFade : public AnimationNode{
    AFade(double trigger, double s, double alphaTarget){
        type = A_FADE;
        triggerTime = trigger;
        speed = s;
        this->alphaTarget = alphaTarget;
    }

    double speed;
    double alphaTarget;
};

struct ATeleport : public AnimationNode{
    ATeleport(double trigger, double x, double y){
        type = A_TELEPORT;
        triggerTime = trigger;
        this->x = x;
        this->y = y;
    }

    double x, y;
};

struct ADestroy : public AnimationNode{
    ADestroy(double trigger){
        type = A_DESTROY;
        triggerTime = trigger;
    }
};

struct AFlip : public AnimationNode{
    AFlip(double trigger, int hFlip, int vFlip){
        this->hFlip = hFlip;
        this->vFlip = vFlip;
    }

    int hFlip, vFlip;
};

struct AVariable : public AnimationNode{
    enum Var{DEAD, ALPHA, H_FLIP, V_FLIP, ADD_EFFECT, REMOVE_FROM_ARMY, REMOVE_EFFECT, WIN, PARTICLE_EFFECT,
            BUT_LOCKED, HIDDEN, AUTO_P, FREEZE, X_VEL, Y_VEL, X_ACC, Y_ACC, ANGLE, ROTATION_VEL,
            DRAW_TAG, REPLACE_HERO, COIN, HERO_CONFINED};

    AVariable(double trigger, Var var, bool value){triggerTime = trigger; type = A_VARIABLE; this->var = var; bValue = value;}
    AVariable(double trigger, Var var, double value){triggerTime = trigger; type = A_VARIABLE; this->var = var; dValue = value;}
    AVariable(double trigger, Var var, int value){triggerTime = trigger; type = A_VARIABLE; this->var = var; iValue = value;}
    AVariable(double trigger, Var var){triggerTime = trigger; type = A_VARIABLE; this->var = var;}

    Var var;
    bool bValue;
    double dValue;
    int iValue;
};

struct AShoot : public AnimationNode{
    enum Object{ARROW, BOMB, SCIMITAR, ROCK, ARROW_2, DART, SPEAR, ERIDU_BLADE};
    AShoot(double trigger, double x, double y, Object obj = ARROW){
        triggerTime = trigger;
        type = A_SHOOT;
        xTarget = x;
        yTarget = y;
        object = obj;
    }
    Object object;
    double xTarget;
    double yTarget;
};

struct ASound : public AnimationNode{
    ASound(double trigger, string snd, CSound::EndTrigger endTrigger = CSound::REMOVE_ENTITY){
        type = A_SOUND;
        triggerTime = trigger;
        name = snd;
        trigger = endTrigger;
    }
    string name;
    CSound::EndTrigger trigger;
};

struct AZoom : public AnimationNode{
    AZoom(double trigger, double dx, double dy, double wTarget, double hTarget){
        type = A_ZOOM;
        triggerTime = trigger;
        this->dx = dx;
        this->dy = dy;
        this->wTarget = wTarget;
        this->hTarget = hTarget;
    }

    double dx, dy;
    double wTarget;
    double hTarget;
};

struct CActor : public Component{
    static Type getType(){return ACTOR;}

    CActor(){
        acting = false;
    }

    void addNode(AnimationNode* node){
        timeline.push_back(node);
    }

    list<AnimationNode*> timeline;
    sf::Clock timer;
    bool acting;
};
struct CScrollList : public Component{
    static Type getType(){return SCROLL_LIST;}

    enum Orientation{HORIZONTAL, VERTICAL};

    CScrollList(){
        size = 100;
        front = 0;
        spacing = 0;
        orientation = HORIZONTAL;
        step = 1;
    }

    CScrollList(list<Entity*> l, int sz = 5, Orientation ori = HORIZONTAL, int spc = 0, int stp = 1){
        for(list<Entity*>::iterator i = l.begin(); i != l.end(); i++){
            cells.push_back(*i);
        }
        front = 0;
        size = sz;
        orientation = ori;
        step = stp;
        spacing = spc;
    }

    vector<Entity*> cells;
    int size;
    int front;
    int selected;
    double spacing;
    double wCell;
    double hCell;
    int step;
    Orientation orientation;
};

struct CScrollButton : public Component{
    static Type getType(){return SCROLL_BUTTON;}

    enum Move{BACKWARD, FORWARD};

    CScrollButton(Entity* e, Move m){
        eList = e;
        move = m;
    }

    Move move;
    Entity* eList;

};

struct CScenarioObject : public Component{
    static Type getType(){return SCENARIO_OBJECT;}

    typedef string ID;
    ID id;
    static map<ID, CScenarioObject> Map;
    CScenarioObject(){}

    CScenarioObject(string def, string click, string sound){
        aDefault = def;
        aClick = click;
        clickSound = sound;
        xScale = 1;
        yScale = 1;
        hFlip = false;
        vFlip = false;
    }

    string aDefault;
    string aClick;
    string clickSound;

    double xRelative, yRelative;
    double width, height;
    double xScale, yScale;
    bool hFlip, vFlip;

};

struct CScenario : public Component{
    static Type getType(){return SCENARIO;}

    static map<string, CScenario> Map;

    CScenario(){
    }
    CScenario(string n){
        name = n;
        xOff = 0;
        yOff = 0;
    }
    CScenario(string n, double xOff, double yOff){
        name = n;
        this->xOff = xOff;
        this->yOff = yOff;
    }

    string name;
    string background;
    string bgMusic;
    list<CScenarioObject> objects;
    double xOff, yOff;
};

struct CScenarioPreview : public Component{
    static Type getType(){return SCENARIO_PREVIEW;}

    list<Entity*> objects;
};

struct CInputTextBox : public Component{
    static Type getType(){return INPUT_TEXT_BOX;}

    enum State{
        ACTIVE, INACTIVE
    };

    CInputTextBox(){
        state = INACTIVE;
    }
    CInputTextBox(bool ignore, State st = INACTIVE, int max = 500, bool numOnly = false){
        ignoreEnter = ignore;
        state = st;
        charCount = 0;
        charMax = max;
        numbersOnly = numOnly;
        ignoreTab = true;
        blinkRate = 0.5;
        blinkOn = false;
    }

    State state;
    string content;
    bool ignoreEnter;
    int charCount;
    int charMax;

    bool numbersOnly;
    bool ignoreTab;

    sf::Clock blinkTimer;
    double blinkRate;
    bool blinkOn;
};

struct CTextInput : public Component{
    static Type getType(){return TEXT_INPUT;}

    char value;
};

struct CMouseInput : public Component{
    static Type getType(){return MOUSE_INPUT;}

    sf::Mouse::Button buttonPressed;
    sf::Mouse::Button buttonReleased;
};

struct CKeyboardInput : public Component{
    static Type getType(){return KEYBOARD_INPUT;}
    void setInput(const sf::Event& ev){
        code = ev.key.code;
        alt = ev.key.alt;
        control = ev.key.control;
        shift = ev.key.shift;
        system = ev.key.system;
    }

    sf::Keyboard::Key code;
    bool alt, control, shift, system;
};

struct CDragDrop : public Component{
    static Type getType(){return DRAG_DROP;}
    CDragDrop(){
        active = false;
    }
    CDragDrop(bool ac){
        active = ac;
    }

    bool active;
};

struct CParentPanel : public Component{
    static Type getType(){return PARENT_PANEL;}
    CParentPanel(Entity* e){
        this->e = e;
    }
    Entity* e;
};

struct CHighlightTrigger : public Component{
    static Type getType(){return HIGHLIGHT_TRIGGER;}

    CHighlightTrigger(CUnitHighlight2::Info i, int v){
        info = i;
        value = v;
    }

    CUnitHighlight2::Info info;
    int value;
};

struct CCallout : public Component{
    static Type getType(){return CALLOUT;}
};

struct CBarDisplayer : public Component{
    static Type getType(){return BAR_DISPLAYER;}

    enum ValueType{FIRE_RESISTANCE, WATER_RESISTANCE, EARTH_RESISTANCE, AIR_RESISTANCE};
    enum GrowDirection{LEFT, RIGHT, TOP, DOWN};

    CBarDisplayer(ValueType vType, Entity* e, GrowDirection grow = RIGHT, double unit = 1.f, double minSize = 0){
        valueType = vType;
        this->growDirection = grow;
        this->unit = unit;
        eValue = e;
        this->minSize = minSize;
    }

    ValueType valueType;
    GrowDirection growDirection;
    Entity* eValue;
    double unit;
    double minSize;
};

struct CAverageUnit : public Component{
    static Type getType(){return AVERAGE_UNIT;}
    vector<int> resistance;
    CAverageUnit(){
        resistance.resize(CUnit::N_DAMAGE_TYPES, 0);
    }
};

struct CScreen : public Component{
    static Type getType(){return SCREEN;}
    enum ID{MAIN_MENU, SPLASH_1, SPLASH_2, SPLASH_3, MATCH, CUSTOM_MATCH, FORMATION_EDITOR};
    enum Transition{FADE_BLACK};

    CScreen(ID id, Transition t){
        this->id = id;
        this->transition = t;
    }

    ID id;
    Transition transition;
};

struct CSystem : public Component{
    static Type getType(){return SYSTEM;}
    CSystem(){}
};

struct CParticleEffect : public Component{
    static Type getType(){return PARTICLE_EFFECT;}
    CParticleEffect(){}
    list<Entity*> emmiters;
};

struct CParticleEmmiter : public Component{
    static Type getType(){return PARTICLE_EMMITER;}

    enum Drawable{TEXTURE, RECTANGLE, CIRCLE, ANIMATION};

    CParticleEmmiter(){}
    CParticleEmmiter(double rate, sf::RectangleShape shape, double lifeSpan, double minSpeed, double maxSpeed,
                     double angle, double angleGap, int nParticles = 1){
        this->rate = rate;
        this->rectShape = shape;
        this->lifeSpan = lifeSpan;
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        this->angle = angle;
        this->angleGap = angleGap;
        this->nParticles = nParticles;
        this->xAcc = 0;
        this->yAcc = 0;
        on = false;
        drawable = RECTANGLE;
    }
    CParticleEmmiter(double rate, sf::CircleShape shape, double lifeSpan, double minSpeed, double maxSpeed,
                     double angle, double angleGap, int nParticles = 1){
        this->rate = rate;
        this->circleShape = shape;
        this->lifeSpan = lifeSpan;
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        this->angle = angle;
        this->angleGap = angleGap;
        this->nParticles = nParticles;
        this->xAcc = 0;
        this->yAcc = 0;
        on = false;
        drawable = CIRCLE;
    }
    CParticleEmmiter(double rate, string texture, double lifeSpan, double minSpeed, double maxSpeed,
                     double angle, double angleGap, int nParticles = 1){
        this->rate = rate;
        this->texture = texture;
        this->lifeSpan = lifeSpan;
        this->minSpeed = minSpeed;
        this->maxSpeed = maxSpeed;
        this->angle = angle;
        this->angleGap = angleGap;
        this->nParticles = nParticles;
        this->xAcc = 0;
        this->yAcc = 0;
        on = false;
        drawable = TEXTURE;
    }

    sf::RectangleShape rectShape;
    sf::CircleShape circleShape;
    string texture;

    double lifeSpan;
    double minSpeed;
    double maxSpeed;
    double xAcc, yAcc;
    double angle;
    double angleGap;
    int nParticles;

    Drawable drawable;
    sf::Clock clock;
    double rate;
    bool on;
};

struct CTypingEffect : public Component{
    static Type getType(){return TYPING_EFFECT;}
    CTypingEffect(string t, double r = 5){
        fullText = t;
        rate = r;
        iNextChar = 0;
    }
    int iNextChar;
    string fullText;
    string text;
    double rate;
    sf::Clock clock;
};

struct CToggleButton : public Component{
    static Type getType(){return TOGGLE_BUTTON;}
    CToggleButton(bool on){
        this->on = on;
    }
    bool on;
    string onTexture;
    string offTexture;
};

struct CStringToggleButton : public Component{
    static Type getType(){return STRING_TOGGLE_BUTTON;}
    CStringToggleButton(string v1, string v2, int value = 0){
        valueIndex = value;
        values.push_back(v1);
        values.push_back(v2);
    }
    void toggle(){
        valueIndex = (valueIndex + 1) % values.size();
    }
    int valueIndex;
    vector<string> values;
};

struct CDropList : public Component{
    static Type getType(){return DROP_LIST;}
    enum Direction{UP, DOWN, LEFT, RIGHT};

    CDropList(list<string> l, string init = "", Direction d = DOWN){
        values = l;
        value = init;
        direction = d;
    }
    void setStyle(sf::RectangleShape def, sf::RectangleShape hov, sf::RectangleShape act){
        defRect = def;
        hovRect = hov;
        actRect = act;
    }

    Direction direction;
    string value;
    list<string> values;
    map<Entity*, string> cells;

    sf::RectangleShape defRect;
    sf::RectangleShape hovRect;
    sf::RectangleShape actRect;
};

struct CCheckBox : public Component{
    static Type getType(){return CHECK_BOX;}

    CCheckBox(string onTexture, string offTexture, bool on = true){
        this->on = on;
        this->onTexture = onTexture;
        this->offTexture = offTexture;
    }

    bool on;
    string onTexture;
    string offTexture;
};

struct CMainMenuPage : public Component{
    static Type getType(){return MAIN_MENU_PAGE;}
    enum ID{SELECT_PLAYER, MAIN_SCREEN, OPTIONS, FIGHT, QUICK_GAME, LAN, ONLINE, ACHIEVEMENTS, FORMATIONS, N_PAGES};
    ID id;
    CMainMenuPage(ID id){
        this->id = id;
    }
};

struct CInGameMenuPage : public Component{
    static Type getType(){return IN_GAME_MENU_PAGE;}
    enum ID{FIRST, OPTIONS, N_PAGES};
    ID id;
    CInGameMenuPage(ID id){
        this->id = id;
    }
};

struct COriginalHero : public Component{
    static Type getType(){return ORIGINAL_HERO;}

    COriginalHero(CCaptain hero){
        this->hero = hero;
    }

    CCaptain hero;
};
struct CAInt : public Component{
    static Type getType(){return AI;}

    CAInt(){
        actionSelected = false;
    }

    bool actionSelected;
};
struct CFormation : public Component{
    static Type getType(){return FORMATION;}

    int positions[23][15];
    string name;
};

struct CCameraShake : public Component{
    static Type getType(){return CAMERA_SHAKE;}
    CCameraShake(double dur, double intens = 8){
        duration = dur;
        intensity = intens;
    }
    double intensity;
    double duration;
};

struct CRemoteControler : public Component{
    static Type getType(){return REMOTE_CONTROLER;}
};

struct CRemotelyControled : public Component{
    static Type getType(){return REMOTELY_CONTROLED;}
};

struct CPacket : public Component{
    static Type getType(){return PACKET;}
    CPacket(){}
    CPacket(sf::Packet pkt){
        packet = pkt;
    }
    sf::Packet packet;
};

struct CMusic : public Component{
    static Type getType(){return MUSIC;}
    enum EndTrigger{LOOP, STOP, NEXT, NEXT_RANDOM};
    CMusic(){}
    CMusic(string name, EndTrigger trigger = LOOP){
        this->name = name;
        this->endTrigger = trigger;
        duration = 0.f;
    }

    string name;
    EndTrigger endTrigger;
    double duration;
    double volume;
    sf::Clock clock;
};

struct CCommandLine : public Component{
    static Type getType(){return COMMAND_LINE;}
    string command;
    map<string, string> params;
    bool hasParam(string param){
        return params.find(param) != params.end();
    }
};

struct CElipsoidalMovement : public Component{
    static Type getType(){return ELIPSOIDAL_MOVEMENT;}
    CElipsoidalMovement(double cx, double cy, double xAmp, double yAmp, double angularSpeed,
                        double startingAngle = 0, bool setX = true, bool setY = true){
        this->cx = cx;
        this->cy = cy;
        this->xAmp = xAmp;
        this->yAmp = yAmp;
        this->angularSpeed = angularSpeed;
        this->angle = startingAngle;
        this->setX = setX;
        this->setY = setY;
    }
    double cx, cy;
    double xAmp, yAmp;
    double angularSpeed;
    bool setX, setY;
    double angle;
};

struct CCameraMan : public Component{
    static Type getType(){return CAMERA_MAN;}
};

#endif // COMPONENT_H
