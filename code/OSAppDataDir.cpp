/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
#include "OSAppDataDir.h"
namespace helper {
bool initializeAppDataDirectory() {
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
    TCHAR path[MAX_PATH];
    // TODO: handle error and exceptions (inspect HRESULT hr = ...)
    //HRESULT hr = SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
    std::string appRootDir = std::string(path) + "/Pointless Wars";
    createDirectory(appRootDir);
    if (!fileExists(appRootDir + "/animations.dat")) {
        //std::ofstream file(appRootDir + "/animations.dat");
        //file << getDefaultFileContent("animations.dat");
        //file.close();
    }
    if (!fileExists(appRootDir + "/strings-en.json")) {
        std::ofstream file(appRootDir + "/strings-en.json");
        file << getDefaultFileContent("strings-en.json");
        file.close();
    }
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
    //TODO: inspect HRESULT
    //HRESULT hr = SHGetFolderPath(HWND_DESKTOP, CSIDL_APPDATA, NULL, SHGFP_TYPE_DEFAULT, path);
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
    if (filename == "animations.dat") {
        return
//=====================================================================================
// FILE: animations.dat
//=====================================================================================
            R"(
%ID							WIDTH	HEIGHT	N_FRAMES	FPS		LOOP
%================
% NEW SCENERIES OBJECTS
%================

% -- WOODS
water-01.png                	    33      62      11          6       1
woods-static-objects-01.png 	    1280    179     1           1
cow-mooing.png            			32      28      7           4       1
cow-idle.png                    	32      28      18          4       1
flag-waving-default.png            	82      250     1           1
flag-waving-blue.png               	82      250     11          4
flag-waving-red.png               	82      250     11          4
flag-blue-01.png                  	47      24      6           6       1
flag-red-01.png                  	47      24      6           6       1
fisherman-01.png                		85      250     6           5       1

% -- BEACH
chicken-idle.png            26      21      2           4
chicken-singing.png         26      21      1           0.5
obj-sea.png                 73      173     12          3
obj-hut.png                 74      78      1           1
obj-party-house.png         74      78      4           1
obj-merchant.png            32      32      10          3
obj-deck.png                110     91      1           1

% -- SNOW
obj-dog.png                 30      24      5           3
obj-dog-woof.png            30      24      4           3
obj-flag-blue-s.png         120     80      9           4
obj-flag-red-s.png          120     80      9           4
obj-light-off.png           35      84      1           1
obj-light-on.png            35      84      8           3
obj-snowman-idle.png        50      50      6           3
obj-snowman-nose.png        50      50      6           3
obj-tree.png                80      120     1           1
obj-snow-fall.png           80      120     10          11

% -- TOWN WEST
coisas-2.png                  1280    174     1           1
ensign.png                  80      100     9           4
unit1.png                   46      46      2           2
unit2.png                   46      46      2           2
unit3.png                   32      32      5           2
unit4.png                   46      46      2           2
unit5.png                   46      46      2           2

% -- TOWN EAST
boat1.png                   100     100     6           2
boat2.png                   100     100     6           2
%castle.png                  320     226     1           1
castle.png                  380     226     1           1
cemitery.png                105     154     1           1
child.png                   100     100     8           5
civita1.png                 32      32      2           3
civita2.png                 32      32      2           3
civita3.png                 32      32      2           3
civita4.png                 32      32      2           3
civita5.png                 32      32      2           3
civita6.png                 32      32      2           3
civita7.png                 32      32      2           3
civita8.png                 32      32      2           3
civita9.png                 32      32      2           3
civita10.png                32      32      2           3
civita11.png                32      32      2           3
civita12.png                32      32      2           3
civita13.png                32      32      2           3
civita14.png                32      32      2           3
civita15.png                32      56      8           3
civita16.png                32      32      2           3
%cow.png                     32      28      9           3
dock.png                    91      124     1           1
fence.png                   10      22      1           1
%flag-blue.png               47      24      6           4
fountain.png                89      86      4           4
guardian.png                32      32      2           2
king.png                    96      180     10          3
market.png                  395     190     1           1
merchant1.png               32      32      2           3
merchant2.png               32      32      2           3
monastery.png               186     181     1           1
obj-pig-walk.png            50      30      10          3
obj-sheep-walk.png          80      30      12          3
plantation.png              586     20      1           1
pottery1.png                70      42      2           2
pottery2.png                70      42      2           2
priest-idle.png             32      32      4           2
rastelo1.png                55      35      8           2
rastelo2.png                55      35      8           2
sculpture.png               94      123     1           1
sleep-soldier1.png          75      75      8           2
sleep-soldier2.png          75      75      8           2
smoke.png                   100     100     9           3
tree.png                    48      77      1           1
trees.png                   165     128     1           1
blacksmith.png              131     88      1           1
training-field.png          155     101     1           1
%================
% SCENARIO OBJECTS
%================
agua.png                    33      62      11          6       1
coisas.png                 1280     177     1           1       1
vaca-mugindo.png            32      28      7           4       1
vaca.png                    32      28      18          4       1
flag-blue.png               47      24      6           6       1
flag-red.png                47      24      6           6       1
casa_a.png                  81      150     11          8       1
casa_v.png                  81      150     11          8       1
pescador.png                85      250     6           5       1
casa-default.png            81      150     1           1       1
casa-default-2.png          81      150     1           1       1

obj-crow.png                26      26      10          3
obj-pig-walk.png            50      30      10          3
obj-pig-grunt.png           50      30      5           4
obj-flag-blue.png           47      24      6           6
obj-flag-red.png            47      24      6           6
obj-door.png                54      139     1           1
obj-door-walk.png           54      139     15          4

obj-door-02.png             53      45      1           1
obj-hello.png               53      45      15          4
obj-merchant.png            32      32      10          3
obj-sleep.png               75      75      8           3
obj-enchant.png             75      75      10          2
obj-water-l.png             28      60      20          3
obj-water-r.png             28      60      20          3
obj-bridge.png              255     393     1           1
obj-door2.png               53      45      1           1
obj-hello2.png              53      45      15          4
%================
% OBJECTS
%================
alpha.png		   			32		32		1			8		1
arbalest-proj.png			20		10		3			12		1
arrow2.png					30		10		1			1		1
comet.png					80		80		4			10		1
bomb.png					26		26		3			6		1
flying-scimitar.png			32		32		4			6		1
eridu-blade.png 			180		180		1			6		1
eridu-blade-spining.png		180		180		4			20		1
coin-still.png				50		50		1			1		1
coin-spin.png				50		50		2			2		1
new-char-idle.png			58		58		5			10		1
new-char-walk.png			58		58		2			5		1
buffalo-run.png				64		64		3			20		1
stalagmite-in.png			75		130		5			10		1
stalagmite-still.png		75		130		2			10		1
stalagmite-out.png			75		130		5			10		1
rolling-stone.png			50		50		4			8		1
medal-blue-idle.png			34		61		1			1		1
medal-red-idle.png			34		61		1			1		1
tornado.png					130		128		4			8		1
puff-01.png					96		88		8			12		1
poof-02.png					61		61		5			12		1
poof-06.png					61		61		5			12		1
poof-03.png					96		88		8			12		1
poof-04.png					96		88		8			12		1
poof-05.png					96		88		3			12		1
explosion-01.png			48		48		12			7		1
geiser.png					36		36		4			8		1
bubble-02.png				40		40		1			8		1
ice-dragon-flying.png		85		50		6			8		1
ice-dragon-rolling.png		80		80		1			8		1
ice-dragon-diving.png		80		80		1			8		1
ice-01.png					32		59		1			8		1
coin-x.png					50		50		12			40		10
coin-blue.png				50		50		1			20		10
coin-red.png				50		50		1			20		10
coin-official.png           43      43      7           20
blue-coin-official.png           43      43      1           20
red-coin-official.png           43      43      1           20

bubble.png					50		50		4			8

chopper-idle.png			80		82		1			1		1
magnet-idle.png				33		36		1			1		1

air-balloon.png				52		83		1			1

camp-fire.png				64		64		5			8

crow1.png					30		30		3			8
crow1-gliding.png			30		30		1			8

heart-flying.png			31		17		2			8

tarot-card-back.png			44		65		1			1		1
tarot-luck.png				44		65		1			1		1
tarot-neutral.png			44		65		1			1		1
tarot-bad-luck.png			44		65		1			1		1
tarot-luck-spin.png			44		86		7			8
tarot-bad-luck-spin.png		44		86		7			8
tarot-neutral-spin.png		44		86		7			8

%================
% HEROES
%================
hero-00.png					46		46		1			8		1
hero-01.png					46		46		1			8		1
hero-02.png					60		60		1			4		1
hero-03.png					46		46		1			4		1
hero-04.png					46		46		1			8		1
hero-05.png					46		46		1			8		1
hero-06.png					46		46		1			10		1
hero-07.png					46		46		1			4		1
hero-08.png					46		46		1			4		1
hero-09.png					46		46		1			4		1
hero-10.png					46		46		1			4		1
hero-11.png					46		46		1			4		1
hero-12.png					58		58		1			4		1
hero-13.png					46		46		1			4		1
hero-14.png					46		46		1			4		1
% IDLE
hero-00-idle.png			46		46		6			12		1
hero-01-idle.png			46		46		1			8		1
hero-02-idle.png			60		60		3			6		1
hero-03-idle.png			46		46		8			12		1
hero-04-idle.png			46		46		8			8		1
hero-05-idle.png			46		46		2			8		1
hero-06-idle.png			46		46		12			10		1
hero-07-idle.png			46		46		2			4		1
hero-08-idle.png			46		46		12			24		1
hero-09-idle.png			46		46		12			12		1
hero-10-idle.png			46		46		2			4		1
hero-11-idle.png			46		46		3			6		1
hero-12-idle.png			46		46		10			6		1
hero-13-idle.png			64		64		2			4		1
hero-14-idle.png			46		46		5			10		1
% WALK
hero-00-walk.png			46		46		2			4		1
hero-01-walk.png			46		46		2			4		1
hero-02-walk.png			60		60		2			4		1
hero-03-walk.png			46		46		8			12		1
hero-04-walk.png			46		46		2			4		1
hero-05-walk.png			46		46		2			4		1
hero-06-walk.png			46		46		2			4		1
hero-07-walk.png			46		46		2			4		1
hero-08-walk.png			46		46		2			4		1
hero-09-walk.png			46		46		2			4		1
hero-10-walk.png			46		46		2			4		1
hero-11-walk.png			46		46		2			4		1
hero-12-walk.png			46		46		2			4		1
hero-13-walk.png			64		64		3			6		1
hero-14-walk.png			46		46		12			24		1
% INTIMIDATION
hero-00-intimidation.png	46		46		12			4		1
hero-01-intimidation.png	46		46		10			4		1
hero-02-intimidation.png	60		60		9			4		1
hero-03-intimidation.png	46		46		12			12		1
hero-04-intimidation.png	46		46		12			4		1
hero-05-intimidation.png	46		46		12			4		1
hero-06-intimidation.png	46		46		12			4		1
hero-07-intimidation.png	46		46		12			4		1
hero-08-intimidation.png	46		46		12			4		1
hero-09-intimidation.png	46		46		12			4		1
hero-10-intimidation.png	46		46		12			4		1
hero-11-intimidation.png	46		46		11			4		1
hero-12-intimidation.png	46		46		2			4		1
hero-13-intimidation.png	64		64		3			6		1
hero-14-intimidation.png	46		46		12			24		1
% OTHERS
etelka-hex.png				60		60		7			8
etelka-change-formation.png 60		60		2			8
goraidh-purification.png	46		46		12			8
%================
% UNITS
%================
% IDLE
unit-00-idle.png   			32		32		4			8		1
unit-01-idle.png   			32		32		4			8		1
unit-02-idle.png   			32		32		2			4		1
unit-03-idle.png   			32		32		5			10		1
unit-04-idle.png   			32		32		4			8		1
unit-05-idle.png   			32		32		2			4		1
unit-06-idle.png   			32		32		5			10		1
unit-07-idle.png   			32		32		5			5		1
unit-08-idle.png   			32		32		4			8		1
unit-09-idle.png   			32		32		2			4		1
unit-10-idle.png   			32		32		2			4		1
unit-11-idle.png   			32		32		2			4		1
unit-12-idle.png   			32		32		2			4		1
unit-13-idle.png   			32		32		5			10		1
unit-14-idle.png   			32		32		2			4		1
unit-15-idle.png   			32		32		6			3		1
unit-16-idle.png   			32		32		2			4		1
unit-17-idle.png   			32		32		2			4		1
unit-18-idle.png   			32		32		2			4		1
unit-19-01-idle.png			32		32		2			4		1
unit-19-02-idle.png			32		32		2			4		1
unit-20-01-idle.png			32		32		2			4		1
unit-20-02-idle.png			32		32		2			4		1
unit-20-03-idle.png			32		32		2			4		1
unit-20-04-idle.png			32		32		2			4		1
% WALK
unit-00-walk.png   			32		32		2			4		1
unit-01-walk.png   			32		32		2			4		1
unit-02-walk.png   			32		32		2			4		1
unit-03-walk.png   			32		32		2			4		1
unit-04-walk.png   			32		32		2			4		1
unit-05-walk.png   			32		32		2			4		1
unit-06-walk.png   			32		32		2			4		1
unit-07-walk.png   			32		32		2			4		1
unit-08-walk.png   			32		32		2			4		1
unit-09-walk.png   			32		32		2			4		1
unit-10-walk.png   			32		32		2			4		1
unit-11-walk.png   			32		32		2			4		1
unit-12-walk.png   			32		32		2			4		1
unit-13-walk.png   			32		32		2			4		1
unit-14-walk.png   			32		32		2			4		1
unit-15-walk.png   			32		32		6			4		1
unit-16-walk.png   			32		32		2			4		1
unit-17-walk.png   			32		32		2			4		1
unit-18-walk.png   			32		32		2			4		1
unit-19-01-walk.png			32		32		2			4		1
unit-19-02-walk.png			32		32		2			4		1
unit-20-01-walk.png			32		32		2			4		1
unit-20-02-walk.png			32		32		2			4		1
unit-20-03-walk.png			32		32		2			4		1
unit-20-04-walk.png			32		32		2			4		1
% DEATH
unit-00-death.png   			32		32		5			10		1
unit-01-death.png   			32		32		3			6		1
unit-02-death.png   			32		32		5			10		1
unit-03-death.png   			32		32		6			6		1
unit-04-death.png   			32		32		4			8		1
unit-05-death.png   			32		32		3			6		1
unit-06-death.png   			32		32		5			10		1
unit-07-death.png   			32		32		5			10		1
unit-08-death.png   			32		32		5			10		1
unit-09-death.png   			32		32		5			1.5		1
unit-10-death.png   			32		32		5			10		1
unit-11-death.png   			32		32		3			6		1
unit-12-death.png   			32		32		5			10		1
unit-13-death.png   			32		32		3			6		1
unit-14-death.png   			32		32		5			6		1
unit-15-death.png   			32		32		5			4		1
%unit-16-death.png   			32		32		2			4		1
%unit-17-death.png   			32		32		2			4		1
%unit-18-death.png   			32		32		2			4		1
% DEAD
unit-00-dead.png   			32		32		1			4		1
unit-01-dead.png   			32		32		1			4		1
unit-02-dead.png   			32		32		1			4		1
unit-03-dead.png   			32		32		1			4		1
unit-04-dead.png   			32		32		1			4		1
unit-05-dead.png   			32		32		1			4		1
unit-06-dead.png   			32		32		1			4		1
unit-07-dead.png   			32		32		1			4		1
unit-08-dead.png   			32		32		1			4		1
unit-09-dead.png   			32		32		1			4		1
unit-10-dead.png   			32		32		1			4		1
unit-11-dead.png   			32		32		1			4		1
unit-12-dead.png   			32		32		1			4		1
unit-13-dead.png   			32		32		1			4		1
unit-14-dead.png   			32		32		1			4		1
unit-15-dead.png   			32		32		1			4		1
%unit-16-dead.png   		32		32		1			4		1
%unit-17-dead.png   		32		32		1			4		1
%unit-18-dead.png   		32		32		1			4		1
% HURT
unit-00-hurt.png   			32		32		1			1		1
unit-01-hurt.png   			32		32		1			1		1
unit-02-hurt.png   			32		32		1			1		1
unit-03-hurt.png   			32		32		1			1		1
unit-04-hurt.png   			32		32		1			1		1
unit-05-hurt.png   			32		32		1			1		1
unit-06-hurt.png   			32		32		1			1		1
unit-07-hurt.png   			32		32		1			1		1
unit-08-hurt.png   			32		32		1			1		1
unit-09-hurt.png   			32		32		1			1		1
unit-10-hurt.png   			32		32		1			1		1
unit-11-hurt.png   			32		32		1			1		1
unit-12-hurt.png   			32		32		1			1		1
unit-13-hurt.png   			32		32		1			1		1
unit-14-hurt.png   			32		32		1			1		1
unit-15-hurt.png   			32		32		1			1		1
unit-16-hurt.png   			32		32		1			1		1
unit-17-hurt.png   			32		32		1			1		1
unit-18-hurt.png   			32		32		1			1		1
unit-19-01-hurt.png			32		32		1			1		1
unit-19-02-hurt.png			32		32		1			1		1
unit-20-01-hurt.png			32		32		1			1		1
unit-20-02-hurt.png			32		32		1			1		1
unit-20-03-hurt.png			32		32		1			1		1
unit-20-04-hurt.png			32		32		1			1		1
% ACTION 01
unit-00-action-01.png		32		32		5			10		1
unit-01-action-01.png		32		32		4			6		1
unit-02-action-01.png		32		32		4			8		1
unit-03-action-01.png		32		32		5			10		1
unit-04-action-01.png		32		32		3			10		1
unit-05-action-01.png		32		32		4			8		1
unit-06-action-01.png		80		80		4			12		1
unit-07-action-01.png		32		32		3			8		1
unit-08-action-01.png		32		32		4			6		1
unit-09-action-01.png		32		32		2			4		1
unit-10-action-01.png		32		32		10			20		1
unit-11-action-01.png		32		32		10			3		1
unit-12-action-01.png		32		32		5			10		1
unit-13-action-01.png		32		32		2			4		1
unit-14-action-01.png		32		32		2			4		1
unit-15-action-01.png		32		32		7			3		1
unit-16-action-01.png		32		32		2			4		1
unit-17-action-01.png		32		32		2			4		1
unit-18-action-01.png		32		32		2			4		1
% ACTION 02
unit-00-action-02.png		32		32		2			4		1
unit-01-action-02.png		32		32		2			4		1
unit-02-action-02.png		32		32		2			4		1
unit-03-action-02.png		32		32		1			4		1
unit-04-action-02.png		32		32		1			4		1
unit-05-action-02.png		32		32		4			8		1
unit-06-action-02.png		32		32		3			6		1
unit-07-action-02.png		32		32		1			4		1
%unit-08-action-02.png		32		32		2			4		1
%unit-09-action-02.png		32		32		2			4		1
%unit-10-action-02.png		32		32		2			4		1
%unit-11-action-02.png		32		32		2			4		1
%unit-12-action-02.png		32		32		2			4		1
%unit-13-action-02.png		32		32		2			4		1
%unit-14-action-02.png		32		32		2			4		1
%unit-15-action-02.png		32		32		2			4		1
%unit-16-action-02.png		32		32		2			4		1
%unit-17-action-02.png		32		32		2			4		1
%unit-18-action-02.png		32		32		2			4		1
% ACTION 03
unit-04-action-03.png		32		32		1			4		1

%snow
chicken-idle.png            26      21      2           4
chicken-singing.png         26      21      1           0.5
obj-dog.png                 30      24      5           3
obj-dog-woof.png            30      24      4           3
obj-snowflag-blue.png       120     80      9           4
obj-snowflag-red.png        120     80      9           4
obj-light-off.png           35      84      1           1
obj-light-on.png            35      84      8           3
obj-snowman-idle.png        50      50      6           3
obj-snowman-nose.png        50      50      6           3
obj-tree.png                80      120     1           1
obj-snow-fall.png           80      120     10          11
)";
         }else if (filename == "strings-en.json"){
             return
//=====================================================================================
// FILE: strings-en.json
//=====================================================================================
R"(
{
    "BUTTON-PROTOTYPE":"Prototype",
    "BUTTON-OPTIONS":"Options",
    "BUTTON-QUIT":"Quit",
    "BUTTON-READY":"Ready",
    "BUTTON-EDITOR":"Editor",
    "BUTTON-EDITOR-LOAD":"Load",
    "BUTTON-EDITOR-SAVE":"Save",
    "BUTTON-EDITOR-CLEAR":"Clear",
    "BUTTON-EDITOR-TEST":"Test",
    "BUTTON-FIGHT":"Fight!",
    "BUTTON-ACHIEVEMENTS":"My Achievements",
    "BUTTON-FORMATIONS":"Formations",
    "BUTTON-BACK":"Back",
    "BUTTON-CAMPAIGN":"For No Reason",
    "BUTTON-QUICK-GAME":"For Honor",
    "BUTTON-LAN":"For Pride",
    "BUTTON-ONLINE":"For Glory",
    "BUTTON-ARENA":"For Power",
    "BUTTON-CUSTOM-GAME":"For Fun",
    "BUTTON-FORMATION-EDITOR":"Formation Editor",
    "BUTTON-NEW-PLAYER":"New Player",
    "BUTTON-RESUME":"Resume",
    "NAME-ACTION-BOW":"Bow",
    "NAME-ACTION-SHIELD":"Shield",
    "DESCRIPTION-ACTION-SHIELD":"Blocks incoming projectiles",
    "DESCRIPTION-ACTION-BOW":"Shoots an arrow at a random target",
    "NAME-FORMATION-SQUARE":"Square Formation",
    "NAME-FORMATION-SPEAR":"Spear Formation",
    "NAME-FORMATION-OVAL":"Oval Formation",
    "NAME-FORMATION-OPEN":"Open Formation",
    "NAME-OFFENSIVE-FORMATION":"Offensive",
    "NAME-DEFENSIVE-FORMATION":"Defensive",
    "NAME-BALANCED-FORMATION":"Balanced",
    "DESCRIPTION-FORMATION-SQUARE":"+50 something",
    "DESCRIPTION-FORMATION-SPEAR":"+50 something",
    "DESCRIPTION-FORMATION-OVAL":"+50 something",
    "DESCRIPTION-FORMATION-OPEN":"+50 something",
    "NAME-BATTLE-SOLVING-1-VS-1":"Man vs Man",
    "NAME-BATTLE-SOLVING-ARMY-VS-ARMY":"Army vs Army",
    "NAME-BATTLE-SOLVING-DRAW":"Tie",
    "DESCRIPTION-BATTLE-SOLVING-1-VS-1":"A random unit fights to his army",
    "DESCRIPTION-BATTLE-SOLVING-ARMY-VS-ARMY":"The armies engage into melee battle",
    "DESCRIPTION-BATTLE-SOLVING-DRAW":"No one wins this battle",
    "START":"Start",
    "LABEL-QUIT":"Quit",
    "LABEL-RESUME":"Resume",
    "LABEL-LOAD":"Load",
    "LABEL-CLEAR":"Clear",
    "LABEL-SAVE":"Save",
    "LABEL-DELETE":"Delete",
    "LABEL-NEW-PLAYER":"New Player",
    "LABEL-SELECT-PROFILE":"Select",
    "LABEL-EDIT-PROFILE":"Edit",
    "LABEL-DELETE-PROFILE":"Delete",
    "LABEL-MENU":"Menu",
    "LABEL-MUSIC":"Music",
    "LABEL-SFX":"Sounds",
    "LABEL-OPTIONS":"Options",
    "LABEL-RESOLUTION":"Resolution",
    "LABEL-READY":"Ready?",
    "LABEL-FULLSCREEN":"Fullscreen",
    "LABEL-ON":"On",
    "LABEL-OFF":"Off",
    "LABEL-SINGLE-PLAYER":"Single Player",
    "LABEL-TUTORIAL":"Tutorial",
    "LABEL-QUICK-MATCH":"Quick Match",
    "LABEL-CAMPAIGN":"Campaign",
    "LABEL-ARENA":"Arena",
    "LABEL-CUSTOM-GAME":"Custom",
    "LABEL-MULTIPLAYER":"Multiplayer",
    "LABEL-NICKNAME":"Nickname",
    "LABEL-CANCEL":"Cancel",
    "LABEL-YES":"Yes",
    "LABEL-NO":"No",
    "LABEL-OK":"Ok",
    "LABEL-BACK":"Back",
    "LABEL-UNITS":"Units",
    "LABEL-HEROES":"Heroes",
    "LABEL-PLAYER-NAME":"Name",
    "LABEL-PLAYER":"Player",
    "LABEL-INTELLIGENCE":"Intelligence",
    "LABEL-INTELLIGENCE-HUMAN":"Human",
    "LABEL-INTELLIGENCE-ARTIFICIAL":"Artificial",
    "LABEL-DIFFICULTY":"Difficulty",
    "LABEL-DIFFICULTY-EASY":"Easy",
    "LABEL-DIFFICULTY-NORMAL":"Normal",
    "LABEL-DIFFICULTY-HARD":"Hard",
    "LABEL-MATCH-CONFIGURATION":"Match Config",
    "LABEL-RANDOM":"Random",
    "LABEL-TURNS":"Turns",
    "LABEL-UNITS":"Units",
    "LABEL-BATTLES":"Battles",
    "LABEL-HEROES-PER-PLAYER":"Heroes per Player",
    "LABEL-ARMY-SIZE":"Army Size",
    "LABEL-UNIT-OPTIONS":"Unit Options",
    "LABEL-BATTLE-DURATION":"Battle Duration",
    "LABEL-WAR-DURATION":"War Duration",
    "LABEL-WIN-STREAK-TO-END":"Win Streak to End",
    "MESSAGE-CONNECTING":"Connecting to server...",
    "MESSAGE-CONNECTION-FAILED":"Failed to connect to server :(",
    "MESSAGE-CONNECTION-LOST":"Connection lost :(",
    "MESSAGE-QUICK-MATCH-SEARCHING":"Looking for opponent...",
    "MESSAGE-QUICK-MATCH-STARTING":"Opponent found!",
    "MESSAGE-LEAVE-MATCH-CONFIRMATION":"Are you sure you want to leave this match?",
    "ARMY-SIZE-WARNING-LESS":"If you do not recruit 50 units,/your army will be filled with randomly",
    "ARMY-SIZE-WARNING-MORE":"You cannot recruit more than 50 units!",
    "NAME-UNIT-00":"Arbalest",
    "NAME-UNIT-01":"Odalisque",
    "NAME-UNIT-02":"Mamluk",
    "NAME-UNIT-03":"Wizard",
    "NAME-UNIT-04":"Axeman",
    "NAME-UNIT-05":"Spike",
    "NAME-UNIT-06":"Champion",
    "NAME-UNIT-07":"Taekwon",
    "NAME-UNIT-08":"Huli",
    "NAME-UNIT-09":"Samurai",
    "NAME-UNIT-10":"Shaolin",
    "NAME-UNIT-11":"Abomination",
    "NAME-UNIT-12":"Longbowman",
    "NAME-UNIT-13":"Halberdier",
    "NAME-UNIT-14":"Ninja",
    "NAME-UNIT-15":"Illusionist",
    "NAME-UNIT-16":"TBD",
    "NAME-UNIT-17":"Obsolete",
    "NAME-UNIT-18":"Obsolete",
    "NAME-UNIT-19":"Promise",
    "NAME-UNIT-20":"Vassal",
    "NAME-HERO-00":"Chimalmat",
    "NAME-HERO-01":"Abraham",
    "NAME-HERO-02":"Etelka",
    "NAME-HERO-03":"Nagendra",
    "NAME-HERO-04":"Goraidh",
    "NAME-HERO-05":"Mog-Ur",
    "NAME-HERO-06":"Caeser",
    "NAME-HERO-07":"Tarrega",
    "NAME-HERO-08":"Catherine",
    "NAME-HERO-09":"Jamyang",
    "NAME-HERO-10":"Imamu",
    "NAME-HERO-11":"Alfgeir",
    "NAME-HERO-12":"Lily",
    "NAME-HERO-13":"Frog",
    "NAME-HERO-14":"True Form",
    "NAME-ACTION-200":"<DEPRECATED> Some Action",
    "NAME-ACTION-201":"<DEPRECATED> Focus Fire",
    "NAME-ACTION-202":"Intimidate",
    "NAME-ACTION-203":"Breath of Life",
    "NAME-ACTION-204":"Fire Buff",
    "NAME-ACTION-205":"Water Buff",
    "NAME-ACTION-206":"Earth Buff",
    "NAME-ACTION-207":"Fire Debuff",
    "NAME-ACTION-208":"Water Debuff",
    "NAME-ACTION-209":"Earth Debuff",
    "NAME-ACTION-210":"<DEPRECATED> Fire Hatred",
    "NAME-ACTION-211":"<DEPRECATED> Water Hatred",
    "NAME-ACTION-212":"Change Formation",
    "NAME-ACTION-213":"Time Dilatation",
    "NAME-ACTION-214":"Water Attack",
    "NAME-ACTION-215":"Earth Attack",
    "NAME-ACTION-216":"Purify",
    "NAME-ACTION-217":"Convert",
    "NAME-ACTION-218":"Summon",
    "NAME-ACTION-219":"True Form",
    "NAME-ACTION-220":"Swap",
    "NAME-ACTION-221":"Somniferous",
    "NAME-ACTION-222":"Curse",
    "NAME-ACTION-223":"Hex",
    "NAME-ACTION-224":"Doom",
    "NAME-ACTION-225":"Hero vs Hero",
    "NAME-ACTION-226":"Love Formation",
    "NAME-ACTION-227":"Tarot",
    "NAME-ACTION-228":"Teleport",
    "NAME-ACTION-229":"Activate object",
    "NAME-ACTION-230":"Confusion",
    "NAME-ACTION-231":"Mind Control",
    "NAME-ACTION-232":"Air Buff",
    "NAME-ACTION-233":"Air Debuff",
    "NAME-ACTION-234":"<DEPRECATED> Air Hatred",
    "NAME-ACTION-235":"Silence",
    "NAME-ACTION-236":"Dead Attack",
    "NAME-ACTION-237":"Air Attack",
    "NAME-ACTION-238":"Immortality",
    "NAME-ACTION-239":"Call Vassals",
    "NAME-ACTION-240":"Ribbit",
    "NAME-ACTION-241":"True Form 1",
    "NAME-ACTION-242":"True Form 2",
    "NAME-ACTION-243":"True Form 3",
    "NAME-ACTION-300":"Balanced Formation",
    "NAME-ACTION-301":"Offensive Formation",
    "NAME-ACTION-302":"Defensive Formation",
    "NAME-ACTION-303":"Love Formation",
    "NAME-ACTION-400":"Grand Battle",
    "NAME-ACTION-401":"Duel",
    "NAME-ACTION-402":"Confine",
    "NAME-ACTION-403":"Mercy",
    "DESCRIPTION-ACTION-200":"Some description",
    "DESCRIPTION-ACTION-201":"Your units focus the enemies that are more likely to die to their attack",
    "DESCRIPTION-ACTION-202":"How scary are you?",
    "DESCRIPTION-ACTION-203":"Resurects up/to 5 allies",
    "DESCRIPTION-ACTION-204":"Grants +1 Fire Resistance to 20 random units",
    "DESCRIPTION-ACTION-205":"Grants +1 Water Resistance to 20 random units",
    "DESCRIPTION-ACTION-206":"Grants +1 Earth Resistance to 20 random units",
    "DESCRIPTION-ACTION-207":"Decreases/fire resistance by 1. Afects/up to 20 enemies.",
    "DESCRIPTION-ACTION-208":"Decreases/water resistance by 1. Afects/up to 20 enemies.",
    "DESCRIPTION-ACTION-209":"Decreases/earth resistance by 1. Afects/up to 20 enemies.",
    "DESCRIPTION-ACTION-210":"Silences fire units",
    "DESCRIPTION-ACTION-211":"Silences water units",
    "DESCRIPTION-ACTION-212":"Enemy/simply obbeys",
    "DESCRIPTION-ACTION-213":"Targets 10 units",
    "DESCRIPTION-ACTION-214":"Targets 10 units",
    "DESCRIPTION-ACTION-215":"Targets 10 units",
    "DESCRIPTION-ACTION-216":"Purge allies debuffs!",
    "DESCRIPTION-ACTION-217":"Convert units",
    "DESCRIPTION-ACTION-218":"Summon units",
    "DESCRIPTION-ACTION-219":"Imamu is/not a human.",
    "DESCRIPTION-ACTION-220":"Permanently swap heroes",
    "DESCRIPTION-ACTION-221":"Takes effect after this battle",
    "DESCRIPTION-ACTION-222":"Something really bad/will happen to your foes.",
    "DESCRIPTION-ACTION-223":"Temporarily turns/enemy hero into a frog",
    "DESCRIPTION-ACTION-224":"Kill everybody",
    "DESCRIPTION-ACTION-225":"Unique close combat option",
    "DESCRIPTION-ACTION-226":"Unique battle formation",
    "DESCRIPTION-ACTION-227":"Test your luck",
    "DESCRIPTION-ACTION-228":"Teleports everybody to another scenario",
    "DESCRIPTION-ACTION-229":"Specific to scenario",
    "DESCRIPTION-ACTION-230":"Enemy gets confused",
    "DESCRIPTION-ACTION-231":"Enemy hero uses a random ability against his army",
    "DESCRIPTION-ACTION-232":"Decreases/air resistance by 1. Afects/up to 20 enemies.",
    "DESCRIPTION-ACTION-233":"Air Debuff",
    "DESCRIPTION-ACTION-234":"Air Hathred",
    "DESCRIPTION-ACTION-235":"Silence",
    "DESCRIPTION-ACTION-236":"Dead Attack",
    "DESCRIPTION-ACTION-237":"Air Attack",
    "DESCRIPTION-ACTION-238":"Immortality",
    "DESCRIPTION-ACTION-239":"Not very/helpful.",
    "DESCRIPTION-ACTION-240":"Chance of returning to normal",
    "DESCRIPTION-ACTION-241":"True Form 1",
    "DESCRIPTION-ACTION-242":"True Form 2",
    "DESCRIPTION-ACTION-243":"True Form 3",
    "DESCRIPTION-ACTION-300":"Units aim at random enemies.",
    "DESCRIPTION-ACTION-301":"-1 resistance of all types./Units aim at enemies most likely to die.",
    "DESCRIPTION-ACTION-302":"+1 resistance of all types./Units aim at enemies most likely to defend.",
    "DESCRIPTION-ACTION-303":"Does nothing",
    "DESCRIPTION-ACTION-400":"Army size is all that matters./Winner gets a medal. Loser gets the almighty/coin... and loses a medal!",
    "DESCRIPTION-ACTION-401":"Two brave units fight to death. The player/represented by the loser will no longer be able/to recruit units of that type.",
    "DESCRIPTION-ACTION-402":"Put enemy hero into jail.",
    "DESCRIPTION-ACTION-403":"Enemy will no longer be able to/adopt its current formation.",
    "NAME-DAMAGE-TYPE-1":"Fire",
    "NAME-DAMAGE-TYPE-2":"Water",
    "NAME-DAMAGE-TYPE-3":"Earth",
    "NAME-DAMAGE-TYPE-4":"Air",
    "SPEECH-INTIMIDATION-ETELKA-01":"You have nothing against me!",
    "SPEECH-INTIMIDATION-ETELKA-02":"Me and Philip will crush you!",
    "SPEECH-INTIMIDATION-NAGENDRA-01":"I foresee death in your future!",
    "SPEECH-INTIMIDATION-NAGENDRA-02":"Sssssssssss!",
    "SPEECH-INTIMIDATION-GORAIDH-01":"Bow to your king, NOW!",
    "SPEECH-INTIMIDATION-GORAIDH-02":"Do you know who I am?",
    "SPEECH-INTIMIDATION-MOG-UR-01":"Ooga booga wooga!",
    "SPEECH-INTIMIDATION-MOG-UR-02":"Wooga nooka looga!",
    "SPEECH-INTIMIDATION-CATHERINE-01":"MUAHAHAHAHHA!",
    "SPEECH-INTIMIDATION-CATHERINE-02":"Communism, join us... or DIE!",
    "SPEECH-INTIMIDATION-IMAMU-01":"This is what will happen to you!",
    "SPEECH-INTIMIDATION-IMAMU-02":"All humans must suffer!",

    "SPEECH-PRESENTATION-ETELKA-01":"You have nothing against me!",
    "SPEECH-PRESENTATION-ETELKA-02":"Me and Philip will crush you!",
    "SPEECH-PRESENTATION-NAGENDRA-01":"I foresee death in your future!",
    "SPEECH-PRESENTATION-NAGENDRA-02":"Close your eyes, this will be over soon!",
    "SPEECH-PRESENTATION-GORAIDH-01":"Bow to your king, NOW!",
    "SPEECH-PRESENTATION-GORAIDH-02":"Do you know who I am?",
    "SPEECH-PRESENTATION-MOG-UR-01":"Ooga booga wooga!",
    "SPEECH-PRESENTATION-MOG-UR-02":"Wooga nooka looga!",
    "SPEECH-PRESENTATION-CATHERINE-01":"MUAHAHAHAHHA!",
    "SPEECH-PRESENTATION-CATHERINE-02":"Communism, join us... or DIE!",
    "SPEECH-PRESENTATION-IMAMU-01":"Finnally!",
    "SPEECH-PRESENTATION-IMAMU-02":"All humans must suffer!",

    "SPEECH-INTIMIDATION-1":"You are facing death!",
    "SPEECH-INTIMIDATION-2":"Come and I'll kill you!",
    "SPEECH-INTIMIDATION-3":"Are you scared?",
    "SPEECH-INTIMIDATION-4":"I never lose!",
    "SPEECH-INTIMIDATION-5":"Po po po po!",
    "SPEECH-INTIMIDATED-1":"I'm out!",
    "SPEECH-INTIMIDATED-2":"See ya!",
    "SPEECH-INTIMIDATED-3":"Later!",
    "SPEECH-INTIMIDATED-4":"Uh... be right back.",
    "SPEECH-INTIMIDATED-5":"Someone is calling me.",
    "SPEECH-DEFEATED-1":"Noooooo!",
    "SPEECH-DEFEATED-2":"Mercy, please!",
    "SPEECH-DEFEATED-3":"I'll be back!",
    "SPEECH-DEFEATED-4":"This won't happen again!",
    "SPEECH-DEFEATED-5":"Shit!",
    "SPEECH-VICTORY-1":"Huehuehuehuehue",
    "SPEECH-VICTORY-2":"Noob...",
    "SPEECH-VICTORY-3":"Easiest battle of my life",
    "SPEECH-VICTORY-4":"I never lose",
    "SPEECH-VICTORY-5":"Lol",
    "SPEECH-VOLUNTEER-1":"I volunteer as tribute!",
    "SPEECH-VOLUNTEER-2":"I fight for my people!",
    "SPEECH-VOLUNTEER-3":"Face me, face death!",
    "SPEECH-VOLUNTEER-4":"You can't against me!",
    "SPEECH-VOLUNTEER-5":"Let's do this!",
    "SPEECH-CAPTAIN-READY-1":"I'm so ready!",
    "SPEECH-CAPTAIN-READY-2":"Let's do this, boys!",
    "SPEECH-CAPTAIN-READY-3":"Who are my victims?",
    "SPEECH-CAPTAIN-READY-4":"I never lose",
    "SPEECH-CAPTAIN-READY-5":"Is this a joke?",
    "SPEECH-FROG":"Breedeet?",
    "SPEECH-BUFFALO":"Mooooo!",
    "SPEECH-IMAMU":"Being human sucks!",
    "SPEECH-ETELKA-HEX":"Hehehe",
    "SPEECH-ETELKA-HEX-FAIL":"Whoops!",
    "SPEECH-ETELKA-FORMATION":"Do it as I command!!!",
    "SPEECH-GORAIDH-PURIFY":"Holy water!",
    "SPEECH-GRR":"Grrr!!",
    "SPEECH-01-UNIT-00-PRE-ATTACK":"Fire!",
    "SPEECH-01-UNIT-01-PRE-ATTACK":"Here's a little gift for you!",
    "SPEECH-01-UNIT-02-PRE-ATTACK":"We have infinite swords",
    "SPEECH-01-UNIT-03-PRE-ATTACK":"We set fire to the rain!",
    "SPEECH-01-UNIT-04-PRE-ATTACK":"Togheter we are stronger",
    "SPEECH-01-UNIT-05-PRE-ATTACK":"We will feed your corpses to the birds",
    "SPEECH-01-UNIT-06-PRE-ATTACK":"Is it stalactite or stalagmite?",
    "SPEECH-01-UNIT-07-PRE-ATTACK":"Women can be strong too!",
    "SPEECH-01-UNIT-08-PRE-ATTACK":"This is a blowjob you will never forget",
    "SPEECH-01-UNIT-09-PRE-ATTACK":"Did you know we can fly?",
    "SPEECH-01-UNIT-10-PRE-ATTACK":"I feel a heavy cloud coming",
    "SPEECH-01-UNIT-11-PRE-ATTACK":"This will hurt!",
    "SPEECH-01-UNIT-12-PRE-ATTACK":"Don't move, or I'll miss you",
    "SPEECH-01-UNIT-13-PRE-ATTACK":"Did someone ordered a tornado?",
    "SPEECH-01-UNIT-14-PRE-ATTACK":"Don't blink, or you'll miss me",
    "SPEECH-01-UNIT-15-PRE-ATTACK":"So many cool powers, why do we get spears?",
    "SPEECH-01-UNIT-16-PRE-ATTACK":"...",
    "SPEECH-01-UNIT-17-PRE-ATTACK":"...",
    "SPEECH-01-UNIT-18-PRE-ATTACK":"...",
    "SPEECH-01-UNIT-19-PRE-ATTACK":"...",
    "SPEECH-01-UNIT-20-PRE-ATTACK":"...",
    "BANNER-COIN":"Behold the almighty coin!",
    "BANNER-PLAYER-PICKING-HERO":" is picking a hero...",
    "BANNER-PLAYER-SELECTING-FORMATION":" is adopting a battle formation...",
    "BANNER-PLAYER-SELECTING-HERO-ACTION":" is thinking...",
    "BANNER-PLAYER-USED-HERO-ACTION":" used ",
    "BANNER-PLAYER-SELECTING-ATTACK":" is thinking...",
    "BANNER-PLAYER-ATTACKING":" is attacking!",
    "BANNER-PLAYER-SELECTING-BATTLE-CLOSURE":" is bringing this battle to an end...!",
    "BANNER-WHEN-ALL-KILLED":"Wow!",
    "BANNER-WHEN-ARMY-VS-ARMY":"Numbers win wars, usually...",
    "BANNER-WHEN-MERCY":" is merciful!",
    "BANNER-WHEN-CONFINE":" is toasted!",
    "BANNER-WHEN-MAN-VS-MAN":"Who are these two?",
    "BANNER-PLAYER-RECRUITING":" is recruiting...",
    "BANNER-PLAYER-SELECTING-HERO":" is choosing a hero...",
    "BANNER-WHEN-PRESENTING-ARMIES":"Prepare for battle!",
    "BANNER-ANNOUNCE-VICTORY":" is the best!"
}
)";
         }else if (filename == "config.xml"){
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
    <Turns>3</Turns>
    <ArmySize>50</ArmySize>
    <BaseBlockChance>0.10</BaseBlockChance>
    <UnitaryBlockChance>0.25</UnitaryBlockChance>
    <OptionsOnRebuild>16</OptionsOnRebuild>
    <RecruitGroup>10</RecruitGroup>
    <GoalScore>2</GoalScore>
    <RandomArmy>false</RandomArmy>
<!--    <UnitPool>00 01 02 03 04 05 06 07 08 09 10 11 12 13 14 15</UnitPool> -->
    <UnitPool>00 01 02 03 04 05 06 07</UnitPool>
    <HeroPool>02 03 04 05 08 10</HeroPool>
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
