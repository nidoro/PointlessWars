#include "Standard.h"

int str2int(string s){
    return atoi(s.c_str());
}

string int2str(int n, int width){
    char arg[255];
    sprintf(arg, "%%0%dd", width);
    char c[255];
    sprintf(c, arg, n);
    return string(c);
}

string db2str(double n){
    char c[255];
    sprintf(c, "%.2f", n);
    return string(c);
}

double str2db(string s){
    return atof(s.c_str());
}

int randomInt(int min, int max){
    return min + (rand() % (int)(max - min + 1));
    /*
    std::default_random_engine generator(time(nullptr));
    std::uniform_int_distribution<int> distribution(min, max);
    return distribution(generator);
    */
}

double randomDouble(double min, double max){
    return min + static_cast <float> (rand()) /( static_cast <float> (RAND_MAX/(max-min)));

    /*
    std::default_random_engine generator(time(nullptr));
    std::uniform_real_distribution<double> distribution(min, max);
    return distribution(generator);
    */
}

double getDistance(double x1, double y1, double x2, double y2){
    return sqrt((x1-x2)*(x1-x2) + (y1-y2)*(y1-y2));
}

double angleBetweenPoints(double x1, double y1, double x2, double y2){
    double dy = y2 - y1;
    double dx = x2 - x1;
    return atan2(dy, dx) * M_DEG;
}

bool oddNumber(int n){
    return n%2 != 0;
}

bool evenNumber(int n){
    return n%2 == 0;
}

bool compare(double a, double b, double eps){
    return abs(a-b) < abs(min(a,b))*eps;
}

bool listIntContains(list<int>& List, int n){
    for(list<int>::iterator i = List.begin(); i != List.end(); i++){
        if (*i == n) return true;
    }
    return false;
}

bool hasEnding(std::string &fullString, std::string ending) {
    if (fullString.length() >= ending.length()) {
        return (0 == fullString.compare(fullString.length() - ending.length(), ending.length(), ending));
    } else {
        return false;
    }
}

bool hasBegining(std::string &fullString, std::string begining) {
    if (fullString.length() >= begining.length()) {
        return (0 == fullString.compare(0, begining.length(), begining));
    } else {
        return false;
    }
}

double getTravelTime(double xa, double ya, double xb, double yb, double speed){
    return getDistance(xa, ya, xb, yb)/abs(speed);
}

bool opposite(bool value){
    return value == true? false:true;
}

bool contains(list<string> l, string v){
    for(list<string>::iterator i = l.begin(); i != l.end(); i++){
        if (*i == v) return true;
    }
    return false;
}

bool contains(list<int> l, int v){
    for(list<int>::iterator i = l.begin(); i != l.end(); i++){
        if (*i == v) return true;
    }
    return false;
}

bool contains(vector<int> l, int v){
    for(vector<int>::iterator i = l.begin(); i != l.end(); i++){
        if (*i == v) return true;
    }
    return false;
}

string toUpper(string str){
    string s;
    for(int i = 0; i < str.size(); i++){
        s.push_back(toupper(str[i]));
    }
    return s;
}

int getRandom(list<int>& L){
    list<int>::iterator i = L.begin();
    std::advance(i, randomInt(0, L.size()-1));
    return *i;
}

int popRandom(list<int>& L){
    list<int>::iterator i = L.begin();
    std::advance(i, randomInt(0, L.size()-1));
    int value = *i;
    L.erase(i);
    return value;
}

void clearBuff(char* buff, int size){
    for(int i = 0; i < size; i++){
        buff[i] = '\0';
    }
}

bool checkNumOnly(string str){
    for(int i = 0; i < str.size(); i++){
        char c = str[i];
        if (!(c >= 48 && c <= 57)){
            return false;
        }
    }
    return true;
}

vector<string> splitString(string str, string tokens){
    vector<string> stringList;
    char* stringValue;
    char* charStr = (char*) str.c_str();
    stringValue = strtok(charStr, tokens.c_str());
    while (stringValue != nullptr){
        stringList.push_back(stringValue);
        stringValue = strtok(charStr, tokens.c_str());
    }
    return stringList;
}
