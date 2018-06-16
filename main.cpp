#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <functional>
#include <tuple>
#include <stdlib.h>


#include <cmath>
#include <chrono>

using namespace std;

/**
 * esta funcion toma como parametros las matrices D y V
 * @return el tiempo que tarda la senial en atravesar el cuerpo
 */
double medirTiempoDeRetrasoDeSenial(vector<vector<double>>& D, vector<vector<double>>& V) {
    double ret = 0.0;
    int n = D.size();
    for (int i=0; i<n; i++) {
        for (int j=0; j<n; j++) {
            ret += D[i][j] * V[i][j];
        }
    }
}

int main(int argc, char * argv[]) {

    vector<vector<double>> D = {{1,0,0,0}, {0,1,0,0}, {0,0,1,0}, {0,0,0,1}};
    vector<vector<double>> V = {{0,1,2,3}, {4,5,6,7}, {8,9,10,11}, {14,25,36,57}};

    double t = medirTiempoDeRetrasoDeSenial(D, V);

    cout << t;
    return 0;
}
