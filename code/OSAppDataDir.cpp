#include "OSAppDataDir.h"

namespace helper {

bool initializeAppDataDirectory() {
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    TCHAR path[MAX_PATH];
    // @todo: handle error and exceptions (inspect HRESULT hr = ...)
    // HRESULT hr = SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    std::string appRootDir = std::string(path) + "/Pointless Wars";
    createDirectory(appRootDir);
    if (!fileExists(appRootDir + "/formations.xml")) {
        std::ofstream file(appRootDir + "/formations.xml");
        file << getDefaultFileContent("formations.xml");
        file.close();
    }
    if (!fileExists(appRootDir + "/match-config.xml")) {
        std::ofstream file(appRootDir + "/match-config.xml");
        file << getDefaultFileContent("match-config.xml");
        file.close();
    }
    if (!fileExists(appRootDir + "/config.xml")) {
        std::ofstream file(appRootDir + "/config.xml");
        file << getDefaultFileContent("config.xml");
        file.close();
    }

    return true;
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
    passwd* pw = getpwuid(getuid());
    std::string path(pw->pw_dir);
    path += "/.Pointless Wars";
#endif
}

std::string getAppDataDir() {
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    TCHAR path[MAX_PATH];
    // @todo: inspect HRESULT
    // HRESULT hr = SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    return std::string(std::string(path) + "/Pointless Wars");
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
    passwd* pw = getpwuid(getuid());
    std::string path(pw->pw_dir);
    path += "/.Pointless Wars";
#endif
}

bool createDirectory(std::string path) {
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    return CreateDirectory(path.c_str(), nullptr);
#elif defined(__unix__) || defined(__unix) || defined(__linux__)

#endif
}

bool fileExists(std::string path) {
    std::ifstream file(path.c_str());
    return file.good();
}

std::string getDefaultFileContent(std::string filename) {
         if (filename == "config.xml"){
             return
//=====================================================================================
// FILE: config.xml
//=====================================================================================
R"(

)";
         }else if (filename == "match-config.xml"){
             return
//=====================================================================================
// FILE: match-config.xml
//=====================================================================================
R"(
<!-- Match Config -->
<MatchConfig>
    <Name>Prototype</Name>
    <Scenery>Random</Scenery>
    <HeroesPerPlayer>3</HeroesPerPlayer>
    <MaxBattles>NONE</MaxBattles>
    <Turns>1</Turns>
    <ArmySize>50</ArmySize>
    <BaseBlockChance>0.10</BaseBlockChance>
    <UnitaryBlockChance>0.25</UnitaryBlockChance>
    <OptionsOnRebuild>16</OptionsOnRebuild>
    <RecruitGroup>10</RecruitGroup>
    <GoalScore>2</GoalScore>
    <RandomArmy>false</RandomArmy>
<!--    <UnitPool>00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15</UnitPool> -->
    <UnitPool>00 01 04 05 08 09 12 13</UnitPool>
    <HeroPool>02 03 04 05 08 10</HeroPool>
    <TurnDuration>
        <HeroPick>0</HeroPick>
        <BattleActions>0</BattleActions>
        <Recruitment>0</Recruitment>
    </TurnDuration>
<!--
    <TurnDuration>
        <HeroPick>15</HeroPick>
        <BattleActions>15</BattleActions>
        <Recruitment>40</Recruitment>
    </TurnDuration>
-->
</MatchConfig>
)";
         }else if (filename == "formations.xml"){
             return
//=====================================================================================
// FILE: formations.xml
//=====================================================================================
R"(
<Square>
    <Positions>
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        9   0  19   0  29   0  39   0  49   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        7   0  17   0  27   0  37   0  47   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        5   0  15   0  25   0  35   0  45   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        3   0  13   0  23   0  33   0  43   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        1   0  11   0  21   0  31   0  41   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        2   0  12   0  22   0  32   0  42   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        4   0  14   0  24   0  34   0  44   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        6   0  16   0  26   0  36   0  46   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        8   0  18   0  28   0  38   0  48   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
       10   0  20   0  30   0  40   0  50   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	</Positions>
</Square>

<Spear>
    <Positions>
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0  35   0  46   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0  24   0  33   0  44   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0  15   0  22   0  31   0  42   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   8   0  13   0  20   0  29   0  40   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   3   0   6   0  11   0  18   0  27   0  38   0  49
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        1   0   2   0   5   0  10   0  17   0  26   0  37   0  48
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   4   0   7   0  12   0  19   0  28   0  39   0  50
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   9   0  14   0  21   0  31   0  41   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0  16   0  23   0  32   0  43   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0  25   0  34   0  45   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
        0   0   0   0   0   0   0   0   0   0  36   0  47   0   0
        0   0   0   0   0   0   0   0   0   0   0   0   0   0   0
	</Positions>
</Spear>

<something>
    <Positions>
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        9 0 19 0 29 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
        7 0 17 0 27 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
        5 0 15 0 25 0 35 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
        3 0 13 0 23 0 33 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 1 0 0 0 0
        1 0 11 0 21 0 31 0 41 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        2 0 12 0 22 0 32 0 42 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        4 0 14 0 24 0 34 0 44 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        6 0 16 0 26 0 36 0 46 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        8 0 18 0 28 0 38 0 48 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        10 0 20 0 30 0 40 0 50 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
        0 0 0 0 0 0 0 0 0 0 0 0 0 0 0
</Positions>
</something>
)";
         }
        return "";
     }
}
