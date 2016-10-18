#include "Config.h"

Config::Config(){
    musMaxVolume = 70;
    sfxMaxVolume = 70;
    deadBodies = false;
    fullscreen = false;
    resolution.x = 0;
    resolution.y = 0;
}

Config::~Config(){
    //dtor
}

sf::Vector2u Config::getResolution(){
    return resolution;
}
bool Config::getFullscreen(){
    return fullscreen;
}
double Config::getMusMaxVolume(){
    return musMaxVolume;
}
double Config::getSfxMaxVolume(){
    return sfxMaxVolume;
}
bool Config::getDeadBodies(){
    return deadBodies;
}

void Config::setResolution(unsigned w, unsigned h){
    resolution = sf::Vector2u(w, h);
}
void Config::setFullscreen(bool value){
    fullscreen = value;
}
void Config::setMusMaxVolume(double value){
    musMaxVolume = value;
}
void Config::setSfxMaxVolume(double value){
    sfxMaxVolume = value;
}
void Config::setDeadBodies(bool value){
    deadBodies = value;
}

bool Config::loadConfigFile(string file){
    Config();
    tinyxml2::XMLDocument doc;
    string path = file;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR){
        printf("Could not open config.xml!\n");
        printf("config.xml will be created.\n");
    }else{
        tinyxml2::XMLElement* el = doc.FirstChildElement();
        for(el; el != nullptr; el = el->NextSiblingElement()){
            string element = el->Value();
            if (element == "Resolution"){
                setResolution(el->IntAttribute("width"), el->IntAttribute("height"));
            }else if (element == "Fullscreen"){
                el->QueryBoolText(&fullscreen);
            }else if (element == "Volume"){
                setMusMaxVolume(el->DoubleAttribute("music"));
                setSfxMaxVolume(el->DoubleAttribute("sfx"));
            }else if (element == "DeadBodies"){
                el->QueryBoolText(&deadBodies);
            }
        }
    }

    validateValues();
    saveConfigFile(file);
}

bool Config::saveConfigFile(string name){
    std::ofstream file;
    file.open(name);
    char str[1000];
    sprintf(str, "<!-- Config File -->\n"); file << str;
    sprintf(str, "<Resolution width=\"%u\" height=\"%u\"/>\n", resolution.x, resolution.y); file << str;
    sprintf(str, "<Fullscreen>%s</Fullscreen>\n", fullscreen ? "true" : "false"); file << str;
    sprintf(str, "<Volume music=\"%.4f\" sfx=\"%.4f\"/>\n", musMaxVolume, sfxMaxVolume); file << str;
    sprintf(str, "<DeadBodies>%s</DeadBodies>\n", deadBodies ? "true" : "false"); file << str;

    file.close();
}

void Config::validateValues(){
    musMaxVolume = max(0.f, (float)musMaxVolume);
    musMaxVolume = min(100.f, (float)musMaxVolume);

    sfxMaxVolume = max(0.f, (float)sfxMaxVolume);
    sfxMaxVolume = min(100.f, (float)sfxMaxVolume);

    if (resolution.x == 0 || resolution.y == 0){
        resolution.x = sf::VideoMode::getDesktopMode().width;
        resolution.y = sf::VideoMode::getDesktopMode().height;
    }

    if (fullscreen){
        sf::VideoMode vMode(resolution.x, resolution.y);
        if (!vMode.isValid()){
            resolution.x = sf::VideoMode::getDesktopMode().width;
            resolution.y = sf::VideoMode::getDesktopMode().height;
        }
    }
}

void Config::setWindowIcon(sf::Texture* texture){
    windowIcon = texture->copyToImage();
}

sf::Image& Config::getWindowIcon(){
    return windowIcon;
}







