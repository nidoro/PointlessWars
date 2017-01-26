#ifndef OSAPPDATADIR_H
#define OSAPPDATADIR_H

#include <string>
#include <fstream>
#include <iostream>
#if defined(_WIN32) || defined(__WIN32__) || defined(_WIN64)
#include <windows.h>
#include <Shlobj.h>
#elif defined(__unix__) || defined(__unix) || defined(__linux__)
#include <pwd.h>
#endif

namespace helper {
bool fileExists(std::string path);
bool initializeAppDataDirectory();
std::string getAppDataDir();
bool createDirectory(std::string path);
std::string getDefaultFileContent(std::string filename);
};
#endif /* OSAPPDATADIR_H */

