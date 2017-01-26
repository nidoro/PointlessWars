#ifndef STANDARD_H
#define STANDARD_H
#include <cmath>
#include <cstdlib>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <list>
#include <map>
#include <vector>
#include <limits>
#include <algorithm>
#include <iostream>
#include <random>
#include <ctime>

const double M_RAD = M_PI/180;
const double M_DEG = 180/M_PI;
/*
using std::abs;
using std::sin;
using std::cos;
using std::sqrt;
using std::cout;
using std::endl;
using std::vector;
using std::list;
using std::map;
using std::string;
using std::ifstream;
using std::fstream;
using std::ofstream;
using std::istringstream;
using std::max;
using std::min;
using std::make_pair;
using std::numeric_limits;
using std::swap;
using std::pair;
using std::stringstream;
*/
using namespace std;
typedef list<int> IntList;
typedef list<int>::iterator IntListIt;

int str2int(string s);

string int2str(int n, int width = 0);

string db2str(double n);

template <typename T> inline int sgn(T val) {
    return (T(0) < val) - (val < T(0));
}

double str2db(string s);

int randomInt(int min, int max);
double randomDouble(double min, double max);

double getDistance(double x1, double y1, double x2, double y2);

double angleBetweenPoints(double x1, double y1, double x2, double y2);

bool oddNumber(int n);

bool evenNumber(int n);

bool compare(double a, double b, double eps = numeric_limits<double>::epsilon());

bool listIntContains(list<int>& List, int n);

bool hasEnding(std::string &fullString, std::string ending);

bool hasBegining(std::string &fullString, std::string begining);

double getTravelTime(double xa, double ya, double xb, double yb, double speed);

template<typename T>
inline T popFront(list<T>& L) {
    if (L.empty()) return T();
    T obj = L.front();
    L.pop_front();
    return obj;
}

int getRandom(list<int>& L);

int popRandom(list<int>& L);

bool opposite(bool value);

bool contains(list<string> l, string v);

bool contains(list<int> l, int v);

bool contains(vector<int> l, int v);

string toUpper(string str);

void clearBuff(char* buff, int size);

template<typename T>
bool isWithinClosedRange(T value, T rangeMin, T rangeMax) {
    return value >= rangeMin && value <= rangeMax;
}

template<typename T>
bool isWithinOpenRange(T value, T rangeMin, T rangeMax) {
    return value > rangeMin && value < rangeMax;
}

bool checkNumOnly(string str);

vector<string> splitString(string str, string tokens);
#endif // STANDARD_H
