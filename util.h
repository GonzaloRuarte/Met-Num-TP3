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
#include "VectorMapMatrix.h"
#include "calcular_rayos.h"

typedef unsigned short uint16_t;

using namespace std;

vector<vector<double>>* leerCSV(string nombreArchivo);
vector<vector<double>> discretizar(const vector<vector<double> >& mat, uint val);
map<uint, double> pasarAMap(const vector<vector<double> >& mat);
vector<double> pasarAVector(const vector<vector<double> >& mat);
VectorMapMatrix  generarRayos(size_t tamMatriz, bool fijos);
VectorMapMatrix  generarRayos_barrido_H(size_t tamMatriz, size_t cada_cuanto);
vector<double> uniformNoise(const vector<double>& t, double init, double end, double sign);
VectorMapMatrix getTraspuesta(VectorMapMatrix &W);
double ECM(vector<double> original, vector<double> reconstruido);
void experimentacion_barrido_H(unsigned char discretizacion, pair<float,float> ruido, unsigned short int espacio_entre_censores);
void listarDirectorio(const string& directorio,  vector<string>& v);

#endif //TC_UTIL_H