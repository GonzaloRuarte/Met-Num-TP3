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

vector<vector<double>>* leerCSV(string nombreArchivo);
vector<vector<double>> discretizar(vector<vector<double>> mat, uint val);
vector<double> pasarAVector(vector<vector<double>> mat);

#endif //TC_UTIL_H
