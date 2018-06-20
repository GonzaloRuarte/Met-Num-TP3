#ifndef TC_UTIL_H
#define TC_UTIL_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>
#include "ppmloader.h"
#include "dirent.h"
#include <sstream>
#include <stdio.h>

using namespace std;

vector<vector<short int>>* leerCSV(string nombreArchivo);

#endif //TC_UTIL_H
