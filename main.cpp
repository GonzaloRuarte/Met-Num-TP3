#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <functional>
#include <tuple>
#include <stdlib.h>
#include "calcular_rayos.h"

#include <cmath>
#include <chrono>



using namespace std;


/**
 * esta funcion toma como parametros las matrices D y V
 * @return el tiempo que tarda la senial en atravesar el cuerpo
 */
vector<double> medir_tiempos_senial(const vector<vector<double> > &M, const vector<double> &v){//recordar que el vector v son las inversas de las velocidades
    const size_t& n = v.size();
    vector<double> res(n);
    for(size_t i = 0; i < n; ++i) {
        res[i] = 0;
        for (size_t k = 0; k < n; ++k)
            res[i] += M[i][k]*v[k];
    }
    return res;
}

vector<vector<double> > trasponer(const vector<vector<double> >& mat){
    const unsigned long& n = mat.size();
    const unsigned long& m = mat[0].size();
    vector<vector<double> > res (m, vector<double>(n));
    for (uint i = 0; i<n;i++)
        for (uint j = 0; j<m;j++)
            res[j][i] = mat[i][j];
    return res;

}

pair<vector<double>,short> EG(const vector<vector<double>> &mat, vector<double> bb) {
	unsigned int i,j,l;
	vector<double> res(mat[0].size(),0);
	short status = 0; //status default, el sistema tiene una unica solucion posible
	double A_kk, A_jk;
	vector<vector<double>> copy = mat;
	bool cont;


	for(i = 0; i < copy[0].size()-1; i++){ //itero sobre las filas, excepto la ultima porque ahi no tengo que hacer nada
		cont = false;
		for(j = i; j < copy.size(); j++){ //itero sobre las filas desde i en adelante, estaria por fijarme si tengo que hacer o no calculo en el paso i de la EG
			if(abs(copy[j][i]) > 0.00001){ //si no hay un 0 en la posicion j,i
				cont = true;
				if(abs(copy[i][i]) <= 0.00001){
					copy[i].swap(copy[j]); //cambio de lugar las filas porque habia un 0 en la diagonal pero no en el resto de la columna
                    			double temp = bb[i];
                   			bb[i] = bb[j];         //como se cambiaron de lugar las filas, también se cambian de lugar los valores de "bb"
                    			bb[j] = temp;
                		}
				break;
			}
		}
		A_kk = copy[i][i];
		for(j = i+1; j < copy.size()-i-1; j++){ //cálculo del paso i si corresponde
			
			if (!cont){break;} //si me tengo que saltear este paso no calculo nada
			if(abs(copy[j][i]) >= 0.00001){//si el elemento j,i es 0 no hago nada en la fila j
				A_jk = copy[j][i];
				for(l = i+1; l < copy[0].size()-i-1; l++){
					copy[j][l] = copy[j][l]-(copy[i][l]*A_jk/A_kk);
				}
				bb[j] -= A_jk/A_kk*bb[i];
			} //no me olvido de actualizar el vector b
		}
		
	}
	
	


	for(i = 0; i < copy[0].size(); i++){
		j = copy[0].size()-1-i;
		if(copy[j][j] == 0 && bb[j] != 0){
			status = -1; //el sistema es incompatible
			break;
		}
		if(copy[j][j] == 0 && bb[j] == 0){
			status = 1; //hay infinitos resultados
			res[j] = 0;
		}
		else{
			res[j] = bb[j]/copy[j][j]; //tengo A_jj*x_j = b_j, paso dividiendo el A_jj
			
			if (j!=0){
				for(unsigned int l = 0; l < j; l++){
					bb[l] = bb[l] - res[j]*copy[l][j]; //esto es importante, al b_l con l de 0 a j-1 le paso restando el A_lj*x_j, porque ya conozco el resultado de X_j, de forma que en la siguiente iteracion solo voy a tener algo de esta pinta A_jj*x_j = b_j
				}
			}
		}
	}
	return make_pair(res,status);
}



vector<vector<double> > multMatPorMat(const vector<vector<double> >& mat1, const vector<vector<double> >& mat2) {
    const unsigned long& n = mat1.size();
    const unsigned long& m = mat2[0].size();
    const unsigned long& l = mat2.size();
    vector<vector<double> > res (n, vector<double>(m, 0));
    for (uint i = 0; i < n; i++)
        for (uint j = 0; j < m; j++)
            for (uint k = 0; k < l; k++)
                res[i][j] += mat1[i][k]*mat2[k][j];
    return res;
}

vector<double> uniformNoise(vector<double> t, double init, double end, double sign){
	vector<double> res(t.size());
	default_random_engine generator;
	uniform_real_distribution<double> distribution(init,end);
	for(uint i = 0; i< t.size(); i++){
		double number = distribution(generator);
		if (sign != 0){
			res[i] = sign*number*t[i] + t[i];
		}
		else {
			random_device rd;
			mt19937 gen(rd());
			uniform_int_distribution<> dis(1, 2);
			if (dis(gen) == 1){
				res[i] = number*t[i] + t[i];	
			}
			else {
				res[i] = t[i] - number*t[i];
			}
		}
		
	}
	return res;
} 

int main(int argc, char * argv[]) {

    return 0;
}
