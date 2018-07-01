#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <tuple>
#include <stdlib.h>


#include <cmath>


#include "calcular_rayos.h"
#include "VectorMapMatrix.h"

using namespace std;

int main() {
    pair<vector<pair<uint,uint> >, vector<pair<uint,uint> > > rayos = generarRayosDeControl(12,5,12,1);
    ofstream archivoSalidaRectas("matlabFiles/rectas_a_b.csv", ios::trunc);
    ofstream archivoSalidaVerticales("matlabFiles/columnas.csv", ios::trunc);
    for (int i = 0; i<rayos.first.size(); i++) {
        if(double(rayos.second[i].second) != double(rayos.first[i].second)) {
            double a = (double(rayos.second[i].first) - double(rayos.first[i].first)) / (double(rayos.second[i].second) - double(rayos.first[i].second)); // No importa el orden
            // de p1 y p2, calcula la pendiente, fila 2 menos fila 1 sobre columna 2 menos columna 1.
            double b = double(rayos.first[i].first) - a * double(rayos.first[i].second) + 0.5; // calcula b = y - ax, con el punto p1.
            archivoSalidaRectas << a << "," << b << endl;
        }
        if(double(rayos.second[i].second) == double(rayos.first[i].second)) {
            archivoSalidaVerticales << rayos.first[i].second << endl;
        }

    }
    return 0;
}