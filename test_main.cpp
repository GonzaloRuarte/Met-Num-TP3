#define utiles

#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <tuple>
#include <stdlib.h>


#include <cmath>


#include "calcular_rayos.cpp"

using namespace std;

int main() {
    pair<uint,uint> p1 = make_pair(uint(31),uint(24));
    pair<uint,uint> p2 = make_pair(uint(0),uint(5));
    int n = 32;
    vector<vector<double> > D = trazar_recta_en_matriz_D(p1,p2,n);
    pair<uint,uint> p3 = make_pair(uint(31),uint(4));
    pair<uint,uint> p4 = make_pair(uint(0),uint(7));
    int n2 = 32;
    vector<vector<double> > D2 = trazar_recta_en_matriz_D(p3,p4,n2);
    return 0;
}