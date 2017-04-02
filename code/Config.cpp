#include "Config.h"

Config::Config() {
    sf::VideoMode vMode;
    vMode.bitsPerPixel = sf::VideoMode::getDesktopMode().bitsPerPixel;
    vMode.width = 1280;
    vMode.height = 720;
    if (!vMode.isValid()) vMode = sf::VideoMode::getDesktopMode();

    musMaxVolume = 20;
    sfxMaxVolume = 70;
    deadBodies = false;
    fullscreen = true;
    skipIntro = false;
    quickStart.clear();
    resolution.x = vMode.width;
    resolution.y = vMode.height;
    lastNickname = "";
}

sf::Vector2u Config::getResolution() {
    return resolution;
}
bool Config::getFullscreen() {
    return fullscreen;
}
double Config::getMusMaxVolume() {
    return musMaxVolume;
}
double Config::getSfxMaxVolume() {
    return sfxMaxVolume;
}
bool Config::getDeadBodies() {
    return deadBodies;
}
bool Config::getSkipIntro() {
    return skipIntro;
}
string Config::getQuickStart() {
    return quickStart;
}
std::string Config::getLastNickname() {
    return lastNickname;
}

void Config::setResolution(unsigned w, unsigned h) {
    resolution = sf::Vector2u(w, h);
}
void Config::setFullscreen(bool value) {
    fullscreen = value;
}
void Config::setMusMaxVolume(double value) {
    musMaxVolume = value;
}
void Config::setSfxMaxVolume(double value) {
    sfxMaxVolume = value;
}
void Config::setDeadBodies(bool value) {
    deadBodies = value;
}
void Config::setSkipIntro(bool value) {
    skipIntro = value;
}
void Config::setQuickStart(string value) {
    quickStart = value;
}
void Config::setLastNickname(string nickname) {
    lastNickname = nickname;
}

bool Config::loadConfigFile(string file) {
    Config();
    tinyxml2::XMLDocument doc;
    string path = file;
    if (doc.LoadFile(path.c_str()) != tinyxml2::XML_NO_ERROR) {
        printf("Could not open config.xml!\n");
        printf("config.xml will be created.\n");
    } else {
        tinyxml2::XMLElement* el = doc.FirstChildElement();
        for( ; el != nullptr; el = el->NextSiblingElement()) {
            string element = el->Value();
            if (element == "Resolution") {
                setResolution(el->IntAttribute("width"), el->IntAttribute("height"));
            } else if (element == "Fullscreen") {
                el->QueryBoolText(&fullscreen);
            } else if (element == "Language") {
                if (el->GetText()) {
                    language = el->GetText();
                }
            } else if (element == "Volume") {
                setMusMaxVolume(el->DoubleAttribute("music"));
                setSfxMaxVolume(el->DoubleAttribute("sfx"));
            } else if (element == "DeadBodies") {
                el->QueryBoolText(&deadBodies);
            } else if (element == "LastNickname") {
                if (el->GetText()) {
                    lastNickname = el->GetText();
                }
            } else if (element == "SkipIntro") {
                el->QueryBoolText(&skipIntro);
                secretOptions.push_back("SkipIntro");
            } else if (element == "QuickStart") {
                quickStart = el->GetText();
                secretOptions.push_back("QuickStart");
            }
        }
    }

    validateValues();
    saveConfigFile(file);
    return true;
}

bool Config::saveConfigFile(string name) {
    std::ofstream file;
    file.open(name);
    char str[1000];
    sprintf(str, "<!-- Config File -->\n");
    file << str;
    sprintf(str, "<Resolution width=\"%u\" height=\"%u\"/>\n", resolution.x, resolution.y);
    file << str;
    sprintf(str, "<Fullscreen>%s</Fullscreen>\n", fullscreen ? "true" : "false");
    file << str;
    sprintf(str, "<Language>%s</Language>\n", language.c_str());
    file << str;
    sprintf(str, "<Volume music=\"%.4f\" sfx=\"%.4f\"/>\n", musMaxVolume, sfxMaxVolume);
    file << str;
    sprintf(str, "<DeadBodies>%s</DeadBodies>\n", deadBodies ? "true" : "false");
    file << str;
    sprintf(str, "<LastNickname>%s</LastNickname>\n", lastNickname.c_str());
    file << str;

    if (contains(secretOptions, "SkipIntro")) {
        sprintf(str, "<SkipIntro>%s</SkipIntro>\n", skipIntro ? "true" : "false");
        file << str;
    }
    if (contains(secretOptions, "QuickStart")) {
        sprintf(str, "<QuickStart>%s</QuickStart>\n", quickStart.c_str());
        file << str;
    }
    file.close();
    return true;
}

void Config::validateValues() {
    musMaxVolume = max(0.f, (float)musMaxVolume);
    musMaxVolume = min(100.f, (float)musMaxVolume);

    sfxMaxVolume = max(0.f, (float)sfxMaxVolume);
    sfxMaxVolume = min(100.f, (float)sfxMaxVolume);

    if (resolution.x == 0 || resolution.y == 0) {
        resolution.x = sf::VideoMode::getDesktopMode().width;
        resolution.y = sf::VideoMode::getDesktopMode().height;
    }

    if (fullscreen) {
        sf::VideoMode vMode(resolution.x, resolution.y);
        if (!vMode.isValid()) {
            resolution.x = sf::VideoMode::getDesktopMode().width;
            resolution.y = sf::VideoMode::getDesktopMode().height;
        }
    }

    if (!language.empty() && !contains(getSupportedLanguages(), language)) {
        language = "en";
    }
}

void Config::setWindowIcon(sf::Texture* texture) {
    windowIcon = texture->copyToImage();
}

sf::Image& Config::getWindowIcon() {
    return windowIcon;
}

void Config::setLanguage(string lan) {
    language = lan;
}

std::string Config::getLanguage() {
    return language;
}

std::list<std::string> Config::getSupportedLanguages() {
    std::string rscRoot = "../../rsc/";
    if (supportedLanguages.empty()) {
        std::list<std::string> candidates = {"en", "pt", "fr", "de", "es"};
        for (auto& i : candidates) {
            if (helper::fileExists(rscRoot + "/lan/strings-" + i + ".json")) {
                supportedLanguages.push_back(i);
            }
        }
    }

    return supportedLanguages;
}

bool isVideoModeSupported(sf::VideoMode mode) {
    std::vector<sf::VideoMode> supported = sf::VideoMode::getFullscreenModes();
    for (unsigned int i = 0; i < supported.size(); i++) {
        if (supported[i] == mode) return true;
    }
    return false;
}

