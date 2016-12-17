#include "Assets.h"

map<std::string, sf::Texture*> Assets::textures;
vector<Formation> Assets::formations(Formation::N_FORMATIONS);
map<Unit::ID, Unit> Assets::units;
map<Action::ID, Action> Assets::actions;
map<Buff::ID, Buff> Assets::buffs;
map<string, sf::Font> Assets::fonts;
map<Projectile::ID, Projectile> Assets::projectiles;
map<Animation::ID, Animation> Assets::animations;
map<Item::ID, Item> Assets::items;
map<BattleSolving::ID, BattleSolving> Assets::solvings;
map<string, string> Assets::stringMap;
map<string, sf::SoundBuffer*> Assets::sounds;
map<string, sf::Music*> Assets::musics;
map<string, NinePatch> Assets::ninePatches;
map<string, char*> Assets::musicBuffers;
map<string, char*> Assets::fontBuffers;
vector<sf::Color> Assets::colors;
vector< vector<sf::Color> > Assets::scale;
string Assets::primaryFont = "8bitOperatorPlusSC-Bold.ttf";
string Assets::secondaryFont = "8bitOperatorPlusSC-Regular.ttf";
string Assets::rscRoot = "../../rsc/";
std::string Assets::appDataDir = helper::getAppDataDir();

Assets::Assets(){
    //ctor
}

void Assets::load(){
    //loadTexturesAt(rscRoot + "images");
    //loadSoundsAt(rscRoot + "sounds");
    //loadMusicsAt(rscRoot + "music");
    //loadFontsAt(rscRoot + "fonts");

    packResources(rscRoot + "resources");
    unpackResources(rscRoot + "resources");

    loadScenariosAt(rscRoot + "sceneries");
    readAnimations();
    readObjects();
    //readFonts();
    readStrings("en");
    readChars();
    //readScenarios();
    //readMusics();
    //readSounds();

    createBattleSolvings();
    createColors();
    createNinePatches();
    createUnits();
    createCaptains();
    createActions();
    createSprites();
}

void Assets::createNinePatches(){
    for (auto& p : textures){
        string filename = p.first;
        string nPatchBegining = "9p-";
        if (hasBegining(filename, nPatchBegining)){
            //printf("===== %s\n", filename.c_str());
            sf::Image img = getTexture(filename)->copyToImage();
            vector< vector<sf::IntRect> > rects(3, vector<sf::IntRect>(3, sf::IntRect(0,0,0,0)));
            NinePatch ninePatch;

            for (int iPart = 0; iPart < 3; iPart++){
                for (int jPart = 0; jPart < 3; jPart++){

                    //printf("%d %d\n", iPart, jPart);
                    rects[iPart][jPart].top = iPart-1 >= 0 ? rects[iPart-1][jPart].top + rects[iPart-1][jPart].height + 1 : 0;
                    rects[iPart][jPart].left = jPart-1 >= 0 ? rects[iPart][jPart-1].left + rects[iPart][jPart-1].width + 1 : 0;
                    rects[iPart][jPart].width = 0;
                    rects[iPart][jPart].height = 0;
                    int iPix, jPix;
                    sf::Color pixel;

                    iPix = rects[iPart][jPart].top;
                    jPix = rects[iPart][jPart].left;
                    pixel = img.getPixel(jPix, iPix);
                    while (pixel != sf::Color(255,0,255,255) && jPix < img.getSize().x){
                        jPix++;
                        rects[iPart][jPart].width++;
                        pixel = img.getPixel(jPix, iPix);
                    }

                    iPix = rects[iPart][jPart].top;
                    jPix = rects[iPart][jPart].left;
                    pixel = img.getPixel(jPix, iPix);
                    while (pixel != sf::Color(255,0,255,255) && iPix < img.getSize().y){
                        iPix++;
                        rects[iPart][jPart].height++;
                        pixel = img.getPixel(jPix, iPix);
                    }

                    sf::Texture* pieceTexture = new sf::Texture;
                    pieceTexture->loadFromImage(img, rects[iPart][jPart]);

                    //printf("left %d top %d\n", rects[iPart][jPart].left, rects[iPart][jPart].top);
                    //printf("width %d height %d\n", rects[iPart][jPart].width, rects[iPart][jPart].height);

                    ninePatch.parts[iPart][jPart] = sf::Sprite(*pieceTexture);
                }
            }

            sf::Sprite spt(*p.second);
            ninePatch.wMin = spt.getLocalBounds().width-2;
            ninePatch.hMin = spt.getLocalBounds().height-2;
            ninePatches.insert(make_pair(filename, ninePatch));

        }
    }
}

void Assets::readChars(){
    tinyxml2::XMLDocument doc;
    string path = rscRoot + "chars/chars.xml";
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Could not open chars.xml!\n");
    }
    tinyxml2::XMLElement* element = doc.FirstChildElement();

    for (element; element != nullptr; element = element->NextSiblingElement()){
        CChar cchar;
        cchar.name = element->Value();

        tinyxml2::XMLElement* elAnimations = element->FirstChildElement("Animations");
        cchar.aIdle = elAnimations->FirstChildElement("Idle")->Attribute("File");
        cchar.aWalk = elAnimations->FirstChildElement("Walk")->Attribute("File");

        CChar::Map.insert(make_pair(cchar.name, cchar));

        tinyxml2::XMLElement* elAnim;
        sf::Texture* texture;
        Animation animation;

        elAnim = elAnimations->FirstChildElement("Idle");
        animation = Animation(elAnim->Attribute("File"), elAnim->DoubleAttribute("FrameWidth"), elAnim->DoubleAttribute("FrameHeight"), elAnim->IntAttribute("NumFrames"), 1.f/elAnim->DoubleAttribute("FPS"));
        animations.insert(make_pair(animation.id, animation));
        texture = new sf::Texture();
        if (!texture->loadFromFile(rscRoot + "chars/" + animation.id)){
            printf("Texture not found: %s\n", animation.id.c_str());
        }
        textures.insert(make_pair(animation.id, texture));

        elAnim = elAnimations->FirstChildElement("Walk");
        animation = Animation(elAnim->Attribute("File"), elAnim->DoubleAttribute("FrameWidth"), elAnim->DoubleAttribute("FrameHeight"), elAnim->IntAttribute("NumFrames"), 1.f/elAnim->DoubleAttribute("FPS"));
        animations.insert(make_pair(animation.id, animation));
        texture = new sf::Texture();
        if (!texture->loadFromFile(rscRoot + "chars/" + animation.id)){
            printf("Texture not found: %s\n", animation.id.c_str());
        }
        textures.insert(make_pair(animation.id, texture));

    }
}

void Assets::createActions(){
    map<CAction::ID, CAction>& actions = CAction::Map;
    int K;
    ///UNIT ACTIONS
    int nUnits = 19;
    K = 0;
    for(int i = 0; i < nUnits; i++){
        actions.insert(make_pair(i+K, CAction(i+K)));
        actions[i].damage = getString("NAME-DAMAGE-TYPE-" + int2str(CUnit::Map[i].damage+1));
        actions[i].damageType = CUnit::Map[i].damage+1;
    }

    ///CAPTAIN SELECTION
	K = 100;
	int nCaptains = 14;
    for(int i = K; i < K+nCaptains; i++){
        actions.insert(make_pair(i, CAction(i)));
    }

    ///CAPTAIN ACTIONS
	K = 200;
    actions.insert(make_pair(K+0, CAction(K+0)));                                       //DEPRECATED (nothing)
    actions.insert(make_pair(K+1, CAction(K+1)));                                       //DEPRECATED (focus fire)
    actions.insert(make_pair(K+2, CAction(K+2)));                                       //INTIMIDATION
    actions.insert(make_pair(K+3, CAction(K+3)));                                       //RESSURECTION
    actions.insert(make_pair(K+4, CAction(K+4, CAction::BUFF, 2)));                     //FIRE BUFF
    actions.insert(make_pair(K+5, CAction(K+5, CAction::BUFF, 2)));                     //WATER BUFF
    actions.insert(make_pair(K+6, CAction(K+6, CAction::BUFF, 2)));                     //EARTH BUFF
    actions.insert(make_pair(K+7, CAction(K+7, CAction::DEBUFF)));                      //FIRE DEBUFF
    actions.insert(make_pair(K+8, CAction(K+8, CAction::DEBUFF)));                      //WATER DEBUFF
    actions.insert(make_pair(K+9, CAction(K+9, CAction::DEBUFF)));                      //EARTH DEBUFF
    actions.insert(make_pair(K+10, CAction(K+10, CAction::DEBUFF, 2)));                 //FIRE FOCUS
    actions.insert(make_pair(K+11, CAction(K+11, CAction::DEBUFF, 2)));                 //WATER FOCUS
    actions.insert(make_pair(K+12, CAction(K+12, CAction::DEBUFF, 2)));                 //EARTH FOCUS
    actions.insert(make_pair(K+13, CAction(K+13)));                                     //FIRE ATTACK
    actions.insert(make_pair(K+14, CAction(K+14)));                                     //WATER ATTACK
    actions.insert(make_pair(K+15, CAction(K+15)));                                     //EARTH ATTACK
    actions.insert(make_pair(K+16, CAction(K+16)));                                     //PURIFY
    actions.insert(make_pair(K+17, CAction(K+17)));                                     //CONVERT
    actions.insert(make_pair(K+18, CAction(K+18)));                                     //SUMMON
    actions.insert(make_pair(K+19, CAction(K+19)));                                     //TRUE FORM
    actions.insert(make_pair(K+20, CAction(K+20)));                                     //SWAP
    actions.insert(make_pair(K+21, CAction(K+21, CAction::DEBUFF, -1, true, true)));    ///SOMNIFEROUS
    actions.insert(make_pair(K+22, CAction(K+22)));                                     ///CURSE
    actions.insert(make_pair(K+23, CAction(K+23)));                                     //HEX
    actions.insert(make_pair(K+24, CAction(K+24)));                                     ///DOOM
    actions.insert(make_pair(K+25, CAction(K+25)));                                     ///HERO VS HERO
    actions.insert(make_pair(K+26, CAction(K+26)));                                     //LOVE FORMATION
    actions.insert(make_pair(K+27, CAction(K+27)));                                     ///TAROT
    actions.insert(make_pair(K+28, CAction(K+28)));                                     //TELEPORT
    actions.insert(make_pair(K+29, CAction(K+29)));                                     ///ACTIVATE OBJECT
    actions.insert(make_pair(K+30, CAction(K+30, CAction::DEBUFF, 2)));                 //CONFUSION
    actions.insert(make_pair(K+31, CAction(K+31)));                                     ///MIND CONTROL
    actions.insert(make_pair(K+32, CAction(K+32)));                                     //AIR BUFF
    actions.insert(make_pair(K+33, CAction(K+33)));                                     //AIR DEBUFF
    actions.insert(make_pair(K+34, CAction(K+34)));                                     //AIR FOCUS
    actions.insert(make_pair(K+35, CAction(K+35, CAction::DEBUFF, 2)));                 //SILENCE
    actions.insert(make_pair(K+36, CAction(K+36)));                                     ///DEAD ATTACK
    actions.insert(make_pair(K+37, CAction(K+37)));                                     //AIR ATTACK
    actions.insert(make_pair(K+38, CAction(K+38)));                                     //IMMORTALITY
    actions.insert(make_pair(K+39, CAction(K+39)));                                     ///CALL SLAVES
    actions.insert(make_pair(K+40, CAction(K+40)));                                     //CROAK
    actions.insert(make_pair(K+41, CAction(K+41)));                                     ///TRUE FORM 1
    actions.insert(make_pair(K+42, CAction(K+42)));                                     ///TRUE FORM 2
    actions.insert(make_pair(K+43, CAction(K+43)));                                     ///TRUE FORM 3
    actions.insert(make_pair(K+44, CAction(K+44)));                                     ///RETURN

    actions[202].btDefTexture = "intimidate-1.png";
    actions[202].btHovTexture = "intimidate-highlit.png";
    actions[202].btActTexture = "intimidate-highlit.png";

    actions[203].btDefTexture = "ressurection-1.png";
    actions[203].btHovTexture = "ressurection-highlit.png";
    actions[203].btActTexture = "ressurection-highlit.png";

    actions[207].btDefTexture = "fire-decrease-1.png";
    actions[207].btHovTexture = "fire-decrease-highlit.png";
    actions[207].btActTexture = "fire-decrease-highlit.png";

    actions[208].btDefTexture = "water-decrease-1.png";
    actions[208].btHovTexture = "water-decrease-highlit.png";
    actions[208].btActTexture = "water-decrease-highlit.png";

    actions[209].btDefTexture = "earth-decrease-1.png";
    actions[209].btHovTexture = "earth-decrease-highlit.png";
    actions[209].btActTexture = "earth-decrease-highlit.png";

    actions[212].btDefTexture = "change-formation-1.png";
    actions[212].btHovTexture = "change-formation-highlit.png";
    actions[212].btActTexture = "change-formation-highlit.png";

    actions[216].btDefTexture = "purification-1.png";
    actions[216].btHovTexture = "purification-highlit.png";
    actions[216].btActTexture = "purification-highlit.png";

    actions[219].btDefTexture = "true-form-1.png";
    actions[219].btHovTexture = "true-form-highlit.png";
    actions[219].btActTexture = "true-form-highlit.png";

    actions[222].btDefTexture = "doom-1.png";
    actions[222].btHovTexture = "doom-highlit.png";
    actions[222].btActTexture = "doom-highlit.png";

    actions[223].btDefTexture = "hex-1.png";
    actions[223].btHovTexture = "hex-highlit.png";
    actions[223].btActTexture = "hex-highlit.png";

    actions[227].btDefTexture = "tarot-1.png";
    actions[227].btHovTexture = "tarot-highlit.png";
    actions[227].btActTexture = "tarot-highlit.png";

    actions[233].btDefTexture = "air-decrease-1.png";
    actions[233].btHovTexture = "air-decrease-highlit.png";
    actions[233].btActTexture = "air-decrease-highlit.png";

    actions[239].btDefTexture = "call-slaves-1.png";
    actions[239].btHovTexture = "call-slaves-highlit.png";
    actions[239].btActTexture = "call-slaves-highlit.png";

    actions[240].btDefTexture = "croak-1.png";
    actions[240].btHovTexture = "croak-highlit.png";
    actions[240].btActTexture = "croak-highlit.png";

    actions[241].btDefTexture = "stampede-1.png";
    actions[241].btHovTexture = "stampede-highlit.png";
    actions[241].btActTexture = "stampede-highlit.png";

    actions[242].btDefTexture = "become-human-1.png";
    actions[242].btHovTexture = "become-human-highlit.png";
    actions[242].btActTexture = "become-human-highlit.png";

    map<CAction::ID, CAction>::iterator it;
    for(it = actions.begin(); it != actions.end(); it++){
        CAction::ID i = it->first;
        if (i >= 200 && i <= 299){

        }
    }

    ///FORMATIONS
	K = 300;
    actions.insert(make_pair(0+K, CAction(0+K)));   //F1
    actions.insert(make_pair(1+K, CAction(1+K)));   //F2
    actions.insert(make_pair(2+K, CAction(2+K)));   //F3
    actions.insert(make_pair(3+K, CAction(3+K)));   //F3
    for(int i = 300; i < 304; i++){
        actions[i].duration = -1;
        actions[i].description = getString("DESCRIPTION-ACTION-" + int2str(i));
        actions[i].name = getString("NAME-ACTION-" + int2str(i));
        actions[i].btDefTexture = "button-formation-" + int2str(i-300, 2) + ".png";
        actions[i].btHovTexture = "button-formation-" + int2str(i-300, 2) + "-highlit.png";
        actions[i].btActTexture = "button-formation-" + int2str(i-300, 2) + "-highlit.png";
    }

    ///BATTLE CLOSURES
	K = 400;
    actions.insert(make_pair(0+K, CAction(0+K)));   //MELEE BATTLE
    actions.insert(make_pair(1+K, CAction(1+K)));
    actions.insert(make_pair(2+K, CAction(2+K)));
    actions.insert(make_pair(3+K, CAction(3+K)));
    for(int i = 400; i < 404; i++){
        actions[i].btDefTexture = "button-closure-" + int2str(i-400, 2) + ".png";
        actions[i].btHovTexture = "button-closure-" + int2str(i-400, 2) + "-highlit.png";
        actions[i].btActTexture = "button-closure-" + int2str(i-400, 2) + "-highlit.png";
    }
    actions[400].name = "Grand Battle";
    actions[401].name = "Duel";
    actions[402].name = "Confine";
    actions[403].name = "Mercy";

    ///OTHERS
    K = 500;
    actions.insert(make_pair(0+K, CAction(0+K)));   //PRESENT ARMY
    actions.insert(make_pair(1+K, CAction(1+K)));   //PRESENT CAPTAIN
    actions.insert(make_pair(2+K, CAction(2+K)));   //ADVANCE ARMY
    actions.insert(make_pair(3+K, CAction(3+K)));   //RETURN WITH ARMY
    actions.insert(make_pair(4+K, CAction(4+K)));   //REMOVE DEAD
}

void Assets::createCaptains(){
    map<CCaptain::ID, CCaptain>& captains = CCaptain::Map;
    int nCaptains = 15;
    for(int i = 0; i < nCaptains; i++){
        captains.insert(make_pair(i, CCaptain(i)));
        captains[i].actions.push_back(202);

        captains[i].aIntimidation = "hero-" + int2str(i, 2) + "-intimidation.png";
        captains[i].aIdle = "hero-" + int2str(i, 2) + "-idle.png";
        captains[i].aWalk = "hero-" + int2str(i, 2) + "-walk.png";
        captains[i].strName = "NAME-HERO-" + int2str(i, 2);
        captains[i].portrait = "hero-" + int2str(i, 2) + "-portrait.png";

        captains[i].btDefTexture = "hero-" + int2str(i, 2) + "-portrait.png";
        captains[i].btHovTexture = "hero-" + int2str(i, 2) + "-portrait-highlit.png";
        captains[i].btActTexture = "hero-" + int2str(i, 2) + "-portrait-highlit.png";
    }
    //HERO-00	Chimalmat
    captains[0].actions.push_back(205);
    captains[0].actions.push_back(231);
    captains[0].actions.push_back(226);
    //captains[0].portrait = "chimalmat-s.png";
    //HERO-01	Abraham
    captains[1].actions.push_back(230);
    captains[1].actions.push_back(218);
    //captains[1].portrait = "abraham-s.png";
    //HERO-02	Etelka
    captains[2].actions.push_back(212);
    captains[2].actions.push_back(223);
    //HERO-03	Nagendra
    captains[3].actions.push_back(208);
    captains[3].actions.push_back(227);
    //HERO-04	Goraidh
    captains[4].actions.push_back(209);
    captains[4].actions.push_back(216);
    //HERO-05	Mog-Ur
    captains[5].actions.push_back(203);
    captains[5].actions.push_back(222);
    //HERO-06	Caeser
    captains[6].actions.push_back(232);
    captains[6].actions.push_back(221);
    //HERO-07	T�rrega
    captains[7].actions.push_back(213);
    captains[7].actions.push_back(235);
    //HERO-08	Catherine
    captains[8].actions.push_back(233);
    captains[8].actions.push_back(239);
    //HERO-09	Jamyang
    captains[9].actions.push_back(206);
    captains[9].actions.push_back(217);
    //HERO-10	Imamu
    captains[10].actions.push_back(207);
    captains[10].actions.push_back(219);
    //HERO-11	Alfgeir
    captains[11].actions.push_back(204);
    captains[11].actions.push_back(238);
    //HERO-12	Lily
    captains[12].actions.push_back(228);
    captains[12].actions.push_back(224);
    captains[12].actions.push_back(220);
    //HERO-13	True Form
    captains[13].actions.push_back(241);
    captains[13].actions.push_back(242);
    captains[13].aIdle = "hero-" + int2str(13, 2) + "-idle.png";
    captains[13].aWalk = "hero-" + int2str(13, 2) + "-walk.png";
    //HERO-13	Frog
    captains[14].actions.push_back(240);

    captains[13].actions.remove(202);
    captains[14].actions.remove(202);
}

void Assets::createUnits(){
    CUnit unit;
    map<CUnit::ID, CUnit>& units = CUnit::Map;

    int nUnits = 21;
    int unitAtt[21][5] = {
//      0   1   2   3   4
//      F   W   E   A   D
        1,	0,	3,  2,	0,      //0
        2,	1,	0,  3,	0,      //1
        1,	0,	2,  3,	0,      //2
        2,	1,	1,  2,	0,      //3
        3,	2,	1,  0,	2,      //4
        0,	3,	2,  1,	2,      //5
        2,	3,	1,  0,	2,      //6
        1,	2,	2,  1,	2,      //7
        2,	1,	0,  3,	1,      //8
        3,	2,	1,  0,	1,      //9
        3,	1,	0,  2,	1,      //10
        2,	2,	1,  1,	1,      //11
        0,	3,	2,  1,	3,      //12
        1,	0,	3,  2,	3,      //13
        0,	2,	3,  1,	3,      //14
        1,	1,	2,  2,	3,      //15
        3,	3,	3,  3,	4,      //16
        0,	0,	0,  0,	4,      //17
        0,	0,	0,  0,	4,      //18
        0,	0,	0,  0,	4,      //19
        0,	0,	0,  0,	4       //20
    };

    //1(0) 13(3) 6(2) 9(1)
    //2(0) 13(3) 6(2) 9(1)
    //8(1) 13(3) 6(2) 9(1)

    for(int i = 0; i < nUnits; i++){
        CUnit unit;
        unit.id = i;
        unit.action = (CAction::ID) i;
        unit.resistance[CUnit::FIRE] = unitAtt[i][0];
        unit.resistance[CUnit::WATER] = unitAtt[i][1];
        unit.resistance[CUnit::EARTH] = unitAtt[i][2];
        unit.resistance[CUnit::AIR] = unitAtt[i][3];
        unit.damage = (CUnit::DamageType)unitAtt[i][4];
        unit.color = Assets::colors[unit.id];
        unit.strName = "NAME-UNIT-" + int2str(i, 2);
        unit.displayer = "unit-" + int2str(i, 2) + "-counter.png";

        unit.aIdle = "unit-" + int2str(i, 2) + "-idle.png";
        unit.aWalk = "unit-" + int2str(i, 2) + "-walk.png";
        unit.aDeath = "unit-" + int2str(i, 2) + "-death.png";
        unit.aDead = "unit-" + int2str(i, 2) + "-dead.png";
        unit.aHurt = "unit-" + int2str(i, 2) + "-hurt.png";
        unit.aAction01 = "unit-" + int2str(i, 2) + "-action-01.png";
        unit.aAction02 = "unit-" + int2str(i, 2) + "-action-02.png";
        unit.aAction03 = "unit-" + int2str(i, 2) + "-action-03.png";
        unit.aAction04 = "unit-" + int2str(i, 2) + "-action-04.png";

        unit.btActionDef = "bt-unit-" + int2str(i, 2) + "-attack-def.png";
        unit.btActionHov = "bt-unit-" + int2str(i, 2) + "-attack-highlit.png";
        unit.btActionAct = "bt-unit-" + int2str(i, 2) + "-attack-highlit.png";

        unit.portrait = "unit-" + int2str(i, 2) + "-counter.png";
        unit.portraitHighlit = "unit-" + int2str(i, 2) + "-counter-highlit.png";

        units.insert(make_pair(unit.id, unit));
    }

    units[19].action = -1;
    units[19].damage = CUnit::FIRE;
    units[20].action = -1;
    units[20].damage = CUnit::FIRE;

/*
    units[8].portrait = "blow-s.png";
    units[11].portrait = "dagger-s.png";
    units[13].portrait = "halberdier-s.png";
    units[12].portrait = "longbow-s.png";
    units[14].portrait = "ninja-s.png";
    units[3].portrait = "wizard-s.png";
*/
}

void Assets::createColors(){

    srand(0);

    colors.push_back(sf::Color::Red);
    colors.push_back(sf::Color::Blue);
    colors.push_back(sf::Color::Green);
    colors.push_back(sf::Color::Yellow);
    colors.push_back(sf::Color::Cyan);
    colors.push_back(sf::Color::Magenta);
    colors.push_back(sf::Color(128,	0,	128));            ///purple
    colors.push_back(sf::Color(255, 106, 0));             ///orange
    colors.push_back(sf::Color(128, 128, 128));           ///grey
    colors.push_back(sf::Color(0, 148, 255));             ///light blue
    colors.push_back(sf::Color(255, 0, 110));             ///pink
    colors.push_back(sf::Color(0, 127, 14));              ///dark green
    colors.push_back(sf::Color::Black);

    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));
    colors.push_back(sf::Color(randomInt(0, 255), randomInt(0, 255), randomInt(0, 255)));

    scale.resize(N_COLORS, vector<sf::Color>(6, sf::Color::White));
    scale[BLUE][0] = sf::Color(255,255,255);
    scale[BLUE][1] = sf::Color(185,255,255);
    scale[BLUE][2] = sf::Color(153,242,248);
    scale[BLUE][3] = sf::Color(102,216,242);
    scale[BLUE][4] = sf::Color(23,127,191);
    scale[BLUE][5] = sf::Color(2,51,115);

    scale[RED][0] = sf::Color(255,255,255);
    scale[RED][1] = sf::Color(252,146,114);
    scale[RED][2] = sf::Color(251,106,74);
    scale[RED][3] = sf::Color(239,59,44);
    scale[RED][4] = sf::Color(203,24,29);
    scale[RED][5] = sf::Color(165,15,21);

    srand(time(nullptr));
}
void Assets::readStrings(string language){
    std::string filename = appDataDir + "/strings-" + language + ".json";
    std::ifstream file(filename.c_str());
    if (!file.is_open()){
        printf("strings-%s.dat not found!\n", language.c_str());
        return;
    }
    std::istream& fileStream = file;
    nlohmann::json j;
    fileStream >> j;
    stringMap.clear();
    for (nlohmann::json::iterator entry = j.begin(); entry != j.end(); entry++){
        std::string value = entry.value();
        for(int c = 0; c < value.size(); c++){
            if (value[c] == '/'){
                value[c] = '\n';
            }
        }
        stringMap.insert(make_pair(entry.key(), value));
    }
}

void Assets::readTextures(){
    /*
	FILE* pipe = popen("ls ../rsc-0.2/images", "r");
	char buf[1024];
	while(fgets(buf, 1024, pipe)){
		string name = buf;
		if (name[name.size()-1] == '\n') name.resize(name.size()-1);

		if (hasEnding(name, ".png") || hasEnding(name, ".jpg") || hasEnding(name, ".bg")){
            sf::Texture* texture = new sf::Texture();
            if (!texture->loadFromFile("../rsc-0.2/images/" + name)){
                printf("Texture not found: %s\n", name.c_str());
            }
            textures.insert(make_pair(name, texture));
		}
	}
    */

    fs::path p(rscRoot + "images");

    loadTexturesAt(p);
}

void Assets::loadTexturesAt(fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".png") || hasEnding(name, ".jpg") || hasEnding(name, ".bg")){
                sf::Texture* texture = new sf::Texture();
                if (!texture->loadFromFile(p.string())){
                    printf("Texture not found: %s\n", name.c_str());
                }
                textures.insert(make_pair(name, texture));
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            loadTexturesAt(subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}
void Assets::loadSoundsAt(fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                sf::SoundBuffer* sb = new sf::SoundBuffer();
                if (!sb->loadFromFile(p.string())){
                    printf("Sound not found: %s\n", name.c_str());
                }
                sounds.insert(make_pair(name, sb));
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            loadSoundsAt(subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}
void Assets::loadMusicsAt(fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                sf::Music* music = new sf::Music();
                if (!music->openFromFile(p.string())){
                    printf("Music not found: %s\n", name.c_str());
                }
                musics.insert(make_pair(name, music));
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            loadMusicsAt(subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}
void Assets::loadFontsAt(fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ttf") || hasEnding(name, ".TTF")){
                sf::Font font;
                if (!font.loadFromFile(p.string())){
                    printf("Fonts not found: %s\n", name.c_str());
                }
                fonts.insert(make_pair(name, font));
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            loadFontsAt(subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}
void Assets::loadScenariosAt(fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".sce")){
                CScenario scenario;
                scenario.name = name;
                ifstream file(string(string(p.string()).c_str()));
                if (!file.is_open()){
                    printf("%s not found!\n", name.c_str());
                    return;
                }
                string line;
                while(!file.eof()){
                    if (line.size() != 0 && line[0] != '%'){
                        istringstream ss(line);
                        if (scenario.background.empty()){
                            ss >> scenario.background;
                        }else if (scenario.bgMusic.empty()){
                            ss >> scenario.bgMusic;
                        }else{
                            CScenarioObject object;
                            int column = 0;
                            while(++column <= 7){
                                switch(column){
                                    case 1: ss >> object.id; break;
                                    case 2: ss >> object.xRelative; break;
                                    case 3: ss >> object.yRelative; break;
                                    case 4: ss >> object.xScale; break;
                                    case 5: ss >> object.yScale; break;
                                    case 6: ss >> object.hFlip; break;
                                    case 7: ss >> object.vFlip; break;
                                    default: break;
                                }
                            }
                            scenario.objects.push_back(object);
                        }
                    }
                    getline(file, line);
                }
                CScenario::Map.insert(make_pair(scenario.name, scenario));
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            loadScenariosAt(subPath);
          }
        }else{
          //cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::readSounds(){
    /*
	FILE* pipe = popen("ls ../rsc-0.2/sounds", "r");
	char buf[1024];
	while(fgets(buf, 1024, pipe)){
		string name = buf;
		if (name[name.size()-1] == '\n') name.resize(name.size()-1);

		if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
            sf::SoundBuffer* sb = new sf::SoundBuffer();
            if (!sb->loadFromFile("../rsc-0.2/sounds/" + name)){
                printf("Sound not found: %s\n", name.c_str());
            }
            sounds.insert(make_pair(name, sb));
		}
	}
    */
    fs::path p(rscRoot + "sounds");

    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
        }else if (fs::is_directory(p)){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            //cout << "    " << x.path() << '\n';
            string name = x.path().filename().string();

            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                sf::SoundBuffer* sb = new sf::SoundBuffer();
                if (!sb->loadFromFile(rscRoot + "sounds/" + name)){
                    printf("Sound not found: %s\n", name.c_str());
                }
                sounds.insert(make_pair(name, sb));
            }
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::readMusics(){
    /*
	FILE* pipe = popen("ls ../rsc-0.2/musics", "r");
	char buf[1024];
	while(fgets(buf, 1024, pipe)){
		string name = buf;
		if (name[name.size()-1] == '\n') name.resize(name.size()-1);

		if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
            sf::Music* music = new sf::Music();
            if (!music->openFromFile("../rsc-0.2/musics/" + name)){
                printf("Music not found: %s\n", name.c_str());
            }
            musics.insert(make_pair(name, music));
		}
	}
	*/

    fs::path p(rscRoot + "music");

    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
        }else if (fs::is_directory(p)){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            //cout << "    " << x.path() << '\n';
            string name = x.path().filename().string();

            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                sf::Music* music = new sf::Music();
                if (!music->openFromFile(rscRoot + "music/" + name)){
                    printf("Music not found: %s\n", name.c_str());
                }
                musics.insert(make_pair(name, music));
            }
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::readScenarios(){
    CScenario::Map.clear();
    fs::path p(rscRoot + "sceneries");

    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
        }else if (fs::is_directory(p)){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            //cout << "    " << x.path() << '\n';
            string name = x.path().filename().string();

            if (hasEnding(name, ".sce")){
                CScenario scenario;
                scenario.name = name;
                ifstream file(string(string(rscRoot + "sceneries/") + name).c_str());
                if (!file.is_open()){
                    printf("%s not found!\n", name.c_str());
                    return;
                }
                string line;
                while(!file.eof()){
                    if (line.size() != 0 && line[0] != '%'){
                        istringstream ss(line);
                        if (scenario.background.empty()){
                            ss >> scenario.background;
                        }else if (scenario.bgMusic.empty()){
                            ss >> scenario.bgMusic;
                        }else{
                            CScenarioObject object;
                            int column = 0;
                            while(++column <= 7){
                                switch(column){
                                    case 1: ss >> object.id; break;
                                    case 2: ss >> object.xRelative; break;
                                    case 3: ss >> object.yRelative; break;
                                    case 4: ss >> object.xScale; break;
                                    case 5: ss >> object.yScale; break;
                                    case 6: ss >> object.hFlip; break;
                                    case 7: ss >> object.vFlip; break;
                                    default: break;
                                }
                            }
                            scenario.objects.push_back(object);
                        }
                    }
                    getline(file, line);
                }
                CScenario::Map.insert(make_pair(scenario.name, scenario));
            }
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::readUnits(){
    ifstream file("../rsc-0.2/units.dat");
    if (!file.is_open()){
        printf("unit.dat not found!\n");
        return;
    }
    int iValue;
    string items;
	string line;
	units.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            Unit unit;
            istringstream ss(line);
            int column = 0;
            while(++column <= 17){
                switch(column){
                    case 1: ss >> iValue; unit.id = (Unit::ID) iValue; break;
                    case 2: ss >> unit.strName; break;
                    case 3: ss >> unit.texture; break;
                    case 4: ss >> unit.width; break;
                    case 5: ss >> unit.height; break;
                    case 6: ss >> iValue; unit.type = (Unit::Type) iValue; break;
                    case 7: ss >> unit.speed; break;
                    case 8: ss >> items; break;
                    case 9: ss >> unit.intimidation; break;
                    case 10: ss >> unit.armor; break;
                    case 11: ss >> unit.melee; break;
                    case 12: ss >> unit.rMinDamage; break;
                    case 13: ss >> unit.rMaxDamage; break;
                    case 14: ss >> unit.damage; break;
                    case 15: ss >> unit.cResist; break;
                    case 16: ss >> unit.rResist; break;
                    case 17: ss >> unit.mResist; break;
                    default: break;
                }
            }
            for(unsigned int i = 0; i < items.size(); i+=2){
                string item;
                item.push_back(items[i]);
                item.push_back(items[i+1]);
                unit.items[str2int(item)] = true;
            }
            units.insert(make_pair(unit.id, unit));
        }
        getline(file, line);
	}

	for(map<Unit::ID, Unit>::iterator i = units.begin(); i != units.end(); i++){
        Unit& unit = i->second;
        unit.animations[Unit::IDLE]         = unit.texture + "-idle.png";
        unit.animations[Unit::WALK]         = unit.texture + "-walk.png";
        unit.animations[Unit::DEATH]        = unit.texture + "-death.png";
        unit.animations[Unit::DEAD]         = unit.texture + "-dead.png";
        unit.animations[Unit::ACTION]       = unit.texture + "-action.png";
        unit.animations[Unit::RAISE_SHIELD] = unit.texture + "-action.png";
        unit.animations[Unit::SHIELD]       = unit.texture + "-action.png";
        //unit.icon = unit.texture + "-icon.png";
        unit.icon = "unit-displayer.png";
        getAnimation(unit.animations[Unit::DEATH]).endMessage = END_ANIMATION_DEATH;
        getAnimation(unit.animations[Unit::ACTION]).endMessage = END_ANIMATION_ACTION;
        getAnimation(unit.animations[Unit::RAISE_SHIELD]).endMessage = END_ANIMATION_RAISE_SHIELD;
	}
}
void Assets::readActions(){
    ifstream file("../rsc-0.2/actions.dat");
    if (!file.is_open()){
        printf("actions.dat not found!\n");
        return;
    }
    int iValue;
	string line;
	actions.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            Action action;
            istringstream ss(line);
            int column = 0;
            while(++column <= 6){
                switch(column){
                    case 1: ss >> iValue; action.id = (Action::ID) iValue; break;
                    case 2: ss >> action.btDefTexture; break;
                    case 3: ss >> action.btHovTexture; break;
                    case 4: ss >> action.btActTexture; break;
                    case 5: ss >> action.strTitle; break;
                    case 6: ss >> action.strText; break;
                    default: break;
                }
            }
            actions.insert(make_pair(action.id, action));
        }
        getline(file, line);
	}
}

void Assets::readProjectiles(){
    ifstream file("../rsc-0.2/projectiles.dat");
    if (!file.is_open()){
        printf("projectiles.dat not found!\n");
        return;
    }
    int iValue;
	string line;
	projectiles.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            Projectile proj;
            istringstream ss(line);
            int column = 0;
            while(++column <= 5){
                switch(column){
                    case 1: ss >> iValue; proj.id = (Projectile::ID) iValue; break;
                    case 2: ss >> proj.texture; break;
                    case 3: ss >> proj.width; break;
                    case 4: ss >> proj.height; break;
                    case 5: ss >> proj.speed; break;
                    default: break;
                }
            }
            projectiles.insert(make_pair(proj.id, proj));
        }
        getline(file, line);
	}
}

void Assets::readBuffs(){
    ifstream file("../rsc-0.2/buffs.dat");
    if (!file.is_open()){
        printf("buffs.dat not found!\n");
        return;
    }
    int iValue;
	string line;
	buffs.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            Buff buff;
            istringstream ss(line);
            int column = 0;
            while(++column <= 7){
                switch(column){
                    case 1: ss >> iValue; buff.id = (Buff::ID) iValue; break;
                    case 2: ss >> buff.icon; break;
                    case 3: ss >> iValue; buff.type = (Buff::Type) iValue; break;
                    case 4: ss >> buff.rangedBonus; break;
                    case 5: ss >> buff.meleeBonus; break;
                    case 6: ss >> buff.siegeBonus; break;
                    case 7: ss >> buff.defenseBonus; break;
                    default: break;
                }
            }
            buffs.insert(std::make_pair(buff.id, buff));
        }
        getline(file, line);
	}
}

void Assets::readAnimations(){
    ifstream file(rscRoot + "animations.dat");
    if (!file.is_open()){
        printf("animations.dat not found!\n");
        return;
    }

	string line;
	animations.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            Animation animation;
            istringstream ss(line);
            int column = 0;
            while(++column <= 5){
                switch(column){
                    case 1: ss >> animation.id; break;
                    case 2: ss >> animation.wSprite; break;
                    case 3: ss >> animation.hSprite; break;
                    case 4: ss >> animation.nFrames; break;
                    case 5: ss >> animation.rate; animation.rate = 1/animation.rate; break;
                    case 6: ss >> animation.loop; break;
                    default: break;
                }
            }
            animation.duration = animation.nFrames * animation.rate;
            animations.insert(make_pair(animation.id, animation));
        }
        getline(file, line);
	}
}

void Assets::createSprites(){
	for(map<Animation::ID, Animation>::iterator i = animations.begin(); i != animations.end(); i++){
        Animation& animation = i->second;
        if (getTexture(animation.id, true) == nullptr) continue;
        sf::Image image = getTexture(animation.id)->copyToImage();
        sf::IntRect rect(0, 0, animation.wSprite, animation.hSprite);
        for (int i = 0; i < animation.nFrames; i++){
            sf::Texture* sptTexture = new sf::Texture;
            sptTexture->loadFromImage(image, rect);
            sf::Sprite s(*sptTexture);
            animation.frames.push_back(s);
            rect.left += animation.wSprite;
        }
        //printf("%s %d %d\n", i->first.c_str(), animation.wSprite, animation.hSprite);
	}
}

void Assets::readObjects(){
    ifstream file(rscRoot + "objects.dat");
    if (!file.is_open()){
        printf("objects.dat not found!\n");
        return;
    }
    //int iValue;
	string line;
	CScenarioObject::Map.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            CScenarioObject object;
            istringstream ss(line);
            int column = 0;
            while(++column <= 4){
                switch(column){
                    case 1: ss >> object.id; break;
                    case 2: ss >> object.aDefault; break;
                    case 3: ss >> object.aClick; break;
                    case 4: ss >> object.clickSound; break;
                    default: break;
                }
            }
            object.width = getAnimation(object.aDefault).wSprite;
            object.height = getAnimation(object.aDefault).hSprite;
            CScenarioObject::Map.insert(make_pair(object.id, object));
        }
        getline(file, line);
	}
}


void Assets::readItems(){
    items.clear();
    items.insert(make_pair(Item::BOW, Item(1, Action::RANGED_ATK)));
    items.insert(make_pair(Item::SWORD, Item(1)));
    items.insert(make_pair(Item::SHIELD, Item(1, Action::MELEE_DEF)));
    items.insert(make_pair(Item::LIGHT_ARMOR, Item(1)));
    items.insert(make_pair(Item::HEAVY_ARMOR, Item(2)));
}

sf::Texture* Assets::getTexture(std::string file, bool returnNullIfMissing){
    std::map<std::string, sf::Texture*>::iterator it;
    it = textures.find(file);

    if (it == textures.end()){
        printf("Texture not mapped: %s\n", file.c_str());
        if (returnNullIfMissing) return nullptr;
        else return textures["alpha.png"];
    }else{
        return (*it).second;
    }
}

const NinePatch& Assets::getNinePatch(string id){
    std::map<std::string, NinePatch>::iterator it;
    it = ninePatches.find(id);

    if (it == ninePatches.end()){
        printf("NinePatch not mapped: %s\n", id.c_str());
        return ninePatches[""];
    }else{
        return (*it).second;
    }
}

const Unit& Assets::getUnit(Unit::ID id){
    map<Unit::ID, Unit>::iterator it;
    it = units.find(id);

    if (it == units.end()){
        printf("Unit not mapped: %d\n", id);
        return (*it).second;
    }else{
        return (*it).second;
    }
}

const Item& Assets::getItem(Item::ID id){
    map<Item::ID, Item>::iterator it;
    it = items.find(id);

    if (it == items.end()) printf("Item not mapped: %d\n", id);
    return (*it).second;
}

const Action& Assets::getAction(Action::ID id){
    map<Action::ID, Action>::iterator it;
    it = actions.find(id);

    if (it == actions.end()){
        printf("Action not mapped: %d\n", id);
        return (*it).second;
    }else{
        return (*it).second;
    }
}

const Projectile& Assets::getProjectile(Projectile::ID id){
    map<Projectile::ID, Projectile>::iterator it;
    it = projectiles.find(id);

    if (it == projectiles.end()){
        printf("Projectile not mapped: %d\n", id);
        return (*it).second;
    }else{
        return (*it).second;
    }
}

Animation& Assets::getAnimation(Animation::ID id){
    map<Animation::ID, Animation>::iterator it;
    it = animations.find(id);

    if (it == animations.end()){
        if (!id.empty()) printf("Animation not mapped: %s\n", id.c_str());
        return animations["alpha.png"];
    }else{
        return (*it).second;
    }
}

const Buff& Assets::getBuff(Buff::ID id){
    map<Buff::ID, Buff>::iterator it;
    it = buffs.find(id);

    if (it == buffs.end()){
        printf("Buff not mapped: %d\n", id);
        return (*it).second;
    }else{
        return (*it).second;
    }
}

sf::Font& Assets::getFont(string name){
    map<string, sf::Font>::iterator it;
    it = fonts.find(name);

    if (it == fonts.end()){
        printf("Font not mapped: %s\n", name.c_str());
        return (*it).second;
    }else{
        return (*it).second;
    }
}

string Assets::getPrimaryFont(){
    return primaryFont;
}

sf::SoundBuffer* Assets::getSound(string name){
    map<string, sf::SoundBuffer*>::iterator it;
    it = sounds.find(name);

    if (it == sounds.end()){
        printf("Sound not mapped: %s\n", name.c_str());
        return nullptr;
    }else{
        return (*it).second;
    }
}

sf::Music* Assets::getMusic(string name){
    map<string, sf::Music*>::iterator it;
    it = musics.find(name);

    if (it == musics.end()){
        printf("Music not mapped: %s\n", name.c_str());
        return nullptr;
    }else{
        return (*it).second;
    }
}

void Assets::readFonts(){
    /*
    ifstream file("../rsc-0.2/fonts.dat");
    if (!file.is_open()){
        printf("fonts.dat not found!\n");
        return;
    }
	string line;
	fonts.clear();
	while(!file.eof()){
		if (line.size() != 0 && line[0] != '%'){
            istringstream ss(line);
            string name;
            ss >> name;
            sf::Font font;
            if (!font.loadFromFile("../rsc-0.2/fonts/" + name)){
                printf("Fonts not found: %s\n", name.c_str());
            }
            fonts.insert(make_pair(name, font));
        }
        getline(file, line);
	}
	FILE* pipe = popen("ls ../rsc-0.2/fonts", "r");
	char buf[1024];
	while(fgets(buf, 1024, pipe)){
		string name = buf;
		if (name[name.size()-1] == '\n') name.resize(name.size()-1);

		if (hasEnding(name, ".ttf") || hasEnding(name, ".TTF")){
            sf::Font font;
            if (!font.loadFromFile("../rsc-0.2/fonts/" + name)){
                printf("Fonts not found: %s\n", name.c_str());
            }
            fonts.insert(make_pair(name, font));
		}
	}
	*/


    fs::path p(rscRoot + "fonts");

    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
        }else if (fs::is_directory(p)){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            //cout << "    " << x.path() << '\n';
            string name = x.path().filename().string();


            if (hasEnding(name, ".ttf") || hasEnding(name, ".TTF")){
                sf::Font font;
                if (!font.loadFromFile(rscRoot + "fonts/" + name)){
                    printf("Fonts not found: %s\n", name.c_str());
                }
                fonts.insert(make_pair(name, font));
            }
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

Formation Assets::getFormation(Formation::ID name){
    return formations[name];
}

void Assets::createBattleSolvings(){
    solvings.clear();

    BattleSolving draw;
    draw.id = BattleSolving::DRAW;
    draw.btDefTexture = "empty-button.png";
    draw.btHovTexture = "empty-button.png";
    draw.btActTexture = "empty-button.png";
    draw.strTitle = "NAME-BATTLE-SOLVING-DRAW";
    draw.strText = "DESCRIPTION-BATTLE-SOLVING-DRAW";

    BattleSolving oneVSone;
    oneVSone.id = BattleSolving::MAN_VS_MAN;
    oneVSone.btDefTexture = "empty-button.png";
    oneVSone.btHovTexture = "empty-button.png";
    oneVSone.btActTexture = "empty-button.png";
    oneVSone.strTitle = "NAME-BATTLE-SOLVING-1-VS-1";
    oneVSone.strText = "DESCRIPTION-BATTLE-SOLVING-1-VS-1";

    BattleSolving armyVSarmy;
    armyVSarmy.id = BattleSolving::MELEE_BATTLE;
    armyVSarmy.btDefTexture = "empty-button.png";
    armyVSarmy.btHovTexture = "empty-button.png";
    armyVSarmy.btActTexture = "empty-button.png";
    armyVSarmy.strTitle = "NAME-BATTLE-SOLVING-ARMY-VS-ARMY";
    armyVSarmy.strText = "DESCRIPTION-BATTLE-SOLVING-ARMY-VS-ARMY";

    BattleSolving duel;
    duel.id = BattleSolving::CHAMPIONS_DUEL;
    duel.btDefTexture = "empty-button.png";
    duel.btHovTexture = "empty-button.png";
    duel.btActTexture = "empty-button.png";
    duel.strTitle = "NAME-BATTLE-SOLVING-ARMY-VS-ARMY";
    duel.strText = "DESCRIPTION-BATTLE-SOLVING-ARMY-VS-ARMY";

    solvings.insert(make_pair(draw.id, draw));
    solvings.insert(make_pair(oneVSone.id, oneVSone));
    solvings.insert(make_pair(armyVSarmy.id, armyVSarmy));
    solvings.insert(make_pair(duel.id, duel));
}
const BattleSolving& Assets::getBattleSolving(BattleSolving::ID id){
    return solvings[id];
}
const string Assets::getString(string id){
    map<string, string>::iterator it;
    it = stringMap.find(id);

    if (it == stringMap.end()){
        printf("String not mapped: %s\n", id.c_str());
        return "";
    }else{
        return (*it).second;
    }
}

int Assets::getUnitsCount(){
    return units.size();
}

list<string> Assets::getBackgrounds(){
    list<string> List;
    for(map<string, sf::Texture*>::iterator i = textures.begin(); i != textures.end(); i++){
        string name = i->first;
        if (hasEnding(name, ".bg")){
            List.push_back(name);
        }
    }
    return List;
}

void Assets::shutdown(){
    for(auto& i : textures){
        if (i.second != nullptr) delete i.second;
    }
    for(auto& i : musics){
        if (i.second != nullptr) delete i.second;
    }
    for(auto& i : sounds){
        if (i.second != nullptr) delete i.second;
    }
    textures.clear();
    musics.clear();
    sounds.clear();
    colors.clear();
    for(auto& i : scale){
        i.clear();
    }
    for(auto& i : musicBuffers){
        delete i.second;
    }
    for(auto& i : fontBuffers){
        delete i.second;
    }
    scale.clear();
}

void Assets::packResources(string filename){
    ofstream file(filename, ios_base::binary);

    char ID[255];
    int nEntries;

    strcpy(ID, "Textures");
    nEntries = getFilesCount(rscRoot + "images", {".png", ".jpg", ".bg"});
    printf("Textures: %d\n", nEntries);
    file.write(ID, 255);
    file.write(reinterpret_cast<const char *>(&nEntries), sizeof(nEntries));
    writeFilesAt(file, rscRoot + "images", {".png", ".jpg", ".bg"});

    strcpy(ID, "Sounds");
    nEntries = getFilesCount(rscRoot + "sounds", {".ogg", ".wav"});
    file.write(ID, 255);
    file.write(reinterpret_cast<const char *>(&nEntries), sizeof(nEntries));
    writeFilesAt(file, rscRoot + "sounds", {".ogg", ".wav"});

    strcpy(ID, "Music");
    nEntries = getFilesCount(rscRoot + "music", {".ogg", ".wav"});
    printf("Musics: %d\n", nEntries);
    file.write(ID, 255);
    file.write(reinterpret_cast<const char *>(&nEntries), sizeof(nEntries));
    writeFilesAt(file, rscRoot + "music", {".ogg", ".wav"});

    strcpy(ID, "Fonts");
    nEntries = getFilesCount(rscRoot + "fonts", {".ttf", ".TTF"});
    printf("Fonts: %d\n", nEntries);
    file.write(ID, 255);
    file.write(reinterpret_cast<const char *>(&nEntries), sizeof(nEntries));
    writeFilesAt(file, rscRoot + "fonts", {".ttf", ".TTF"});

    file.close();
}

void Assets::unpackResources(string filename){
    ifstream file(filename, ios_base::binary);

    char ID[255];

    while (!file.eof()){
        strcpy(ID, "");
        file.read(ID, 255);
        if (strcmp(ID, "Textures") == 0){
            readTextureMap(file);
        }else if (strcmp(ID, "Sounds") == 0){
            readSoundsMap(file);
        }else if (strcmp(ID, "Music") == 0){
            readMusicMap(file);
        }else if (strcmp(ID, "Fonts") == 0){
            readFontsMap(file);
        }
    }
}

int Assets::getFilesCount(fs::path path, list<string> extensions){
    fs::path p = path;
    int count = 0;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
            string name = p.filename().string();
            for(string& ext : extensions){
                if (hasEnding(name, ext)){
                    count++;
                }
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            count += getFilesCount(subPath, extensions);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
    return count;
}

void Assets::writeFilesAt(ofstream& file, fs::path path, list<string> extensions){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
            string name = p.filename().string();
            for(string& ext : extensions){
                if (hasEnding(name, ext)){
                    int length;
                    char* memBlock = mallocFile(p.string(), length);
                    char charName[255];
                    strcpy(charName, name.c_str());
                    file.write(charName, 255);
                    file.write(reinterpret_cast<const char *>(&length), sizeof(length));
                    file.write(memBlock, length);
                    delete memBlock;
                }
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            writeFilesAt(file, subPath, extensions);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::writeTexturesAt(ofstream& file, fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".png") || hasEnding(name, ".jpg") || hasEnding(name, ".bg")){
                int length;
                char* memBlock = mallocFile(p.string(), length);
                char charName[255];
                strcpy(charName, name.c_str());
                file.write(charName, 255);
                file.write(reinterpret_cast<const char *>(&length), sizeof(length));
                file.write(memBlock, length);
                delete memBlock;
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            writeTexturesAt(file, subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::writeSoundsAt(ofstream& file, fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                int length;
                char* memBlock = mallocFile(p.string(), length);
                file << name;
                file << length;
                file.write(memBlock, length);
                delete memBlock;
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            writeSoundsAt(file, subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::writeMusicAt(ofstream& file, fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ogg") || hasEnding(name, ".wav")){
                int length;
                char* memBlock = mallocFile(p.string(), length);
                file << name;
                file << length;
                file.write(memBlock, length);
                delete memBlock;
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            writeMusicAt(file, subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::writeFontsAt(ofstream& file, fs::path path){
    fs::path p = path;
    try{
      if (fs::exists(p)){
        if (fs::is_regular_file(p)){
          //cout << p << " size is " << fs::file_size(p) << '\n';
            //cout << "    " << x.path() << '\n';
            string name = p.filename().string();
            if (hasEnding(name, ".ttf") || hasEnding(name, ".TTF")){
                int length;
                char* memBlock = mallocFile(p.string(), length);
                file << name;
                file << length;
                file.write(memBlock, length);
                delete memBlock;
            }
        }else if (fs::is_directory(p) && p.filename().string() != "obsolet" && p.filename().string() != "trash" && p.filename().string() != "ignore"){
          //cout << p << " is a directory containing:\n";

          for (fs::directory_entry& x : fs::directory_iterator(p)){
            string subPath = x.path().string();
            writeFontsAt(file, subPath);
          }
        }else{
          cout << p << " exists, but is not a regular file or directory\n";
        }
      }
      else
        cout << p << " does not exist\n";
    }catch (const fs::filesystem_error& ex){
      cout << ex.what() << '\n';
    }
}

void Assets::readTextureMap(ifstream& file){
    int nEntries;
    file.read(reinterpret_cast<char*> (&nEntries), sizeof(&nEntries));

    textures.clear();
    for (int i = 0; i < nEntries; i++){
        char entryKey[255];
        int entryLength;
        char* memBlock;
        file.read(entryKey, 255);
        file.read(reinterpret_cast<char*> (&entryLength), sizeof(&entryLength));
        memBlock = new char[entryLength];
        file.read(memBlock, entryLength);
        sf::Texture* texture = new sf::Texture();
        if (!texture->loadFromMemory(memBlock, entryLength)){
            printf("Failed to load texture from memory: %s\n", entryKey);
        }
        textures.insert(make_pair(entryKey, texture));
        delete memBlock;
    }
}

void Assets::readSoundsMap(ifstream& file){
    int nEntries;
    file.read(reinterpret_cast<char*> (&nEntries), sizeof(&nEntries));

    sounds.clear();
    for (int i = 0; i < nEntries; i++){
        char entryKey[255];
        int entryLength;
        char* memBlock;
        file.read(entryKey, 255);
        file.read(reinterpret_cast<char*> (&entryLength), sizeof(&entryLength));
        memBlock = new char[entryLength];
        file.read(memBlock, entryLength);
        sf::SoundBuffer* soundBuffer = new sf::SoundBuffer();
        if (!soundBuffer->loadFromMemory(memBlock, entryLength)){
            printf("Failed to load sound from memory: %s\n", entryKey);
        }
        sounds.insert(make_pair(entryKey, soundBuffer));
        delete memBlock;
    }
}

void Assets::readMusicMap(ifstream& file){
    int nEntries;
    file.read(reinterpret_cast<char*> (&nEntries), sizeof(&nEntries));
    printf("Reading %d musics", nEntries);

    musics.clear();
    for (int i = 0; i < nEntries; i++){
        char entryKey[255];
        int entryLength;
        char* memBlock;
        file.read(entryKey, 255);
        file.read(reinterpret_cast<char*> (&entryLength), sizeof(&entryLength));
        memBlock = new char[entryLength];
        file.read(memBlock, entryLength);
        sf::Music* music = new sf::Music();
        if (!music->openFromMemory(memBlock, entryLength)){
            printf("Failed to load music from memory: %s\n", entryKey);
        }
        musics.insert(make_pair(entryKey, music));
        musicBuffers.insert(make_pair(entryKey, memBlock));
    }
}

void Assets::readFontsMap(ifstream& file){
    int nEntries;
    file.read(reinterpret_cast<char*> (&nEntries), sizeof(&nEntries));
    printf("Reading %d fonts", nEntries);

    fonts.clear();
    for (int i = 0; i < nEntries; i++){
        char entryKey[255];
        int entryLength;
        char* memBlock;
        file.read(entryKey, 255);
        file.read(reinterpret_cast<char*> (&entryLength), sizeof(&entryLength));
        memBlock = new char[entryLength];
        file.read(memBlock, entryLength);
        sf::Font font;
        if (!font.loadFromMemory(memBlock, entryLength)){
            printf("Failed to load font from memory: %s (%d)\n", entryKey, i);
        }
        fonts.insert(make_pair(entryKey, font));
        fontBuffers.insert(make_pair(entryKey, memBlock));
    }
}

char* mallocFile(string filename, int& length){
    ///DELETE MUST BE CALLED AFTER USING MEMORY BLOCK
    ifstream file(filename, ios_base::binary);
    file.seekg (0, file.end);
    length = file.tellg();
    file.seekg (0, file.beg);
    char* memBlock = new char[length];
    file.read(memBlock, length);
    return memBlock;
}












