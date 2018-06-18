#include <iostream>
#include <fstream>
#include <vector>
#include <map>
#include <algorithm>
#include <random>
#include <functional>
#include <tuple>
#include <stdlib.h>


#include <cmath>
#include <chrono>

using namespace std;


/**
 * Dado 2 puntos por los que pasa una recta (que esten en el borde), los mueve para que la recta se rote en dirección
 * contraria a las agujas del reloj. Importante, estoy considerando que el pixel [0,0] será la esquina superior
 * izquierda, se debería cambiar esto si lo vamos a pensar distinto.
 * @param p1 es el punto 1 donde el primer valor es la fila, el segundo la columna, y debe estar en el borde de la
 * imagen
 * @param p2 es el segundo punto donde termina la recta, con las mismas condiciones que p1.
 * @param n es el tamaño total de la imágen final resultante de todos los métodos.
 */
void rotarContrarreloj(pair<uint,uint>& p1, pair<uint,uint>& p2, size_t n) {
    if(p1.second == 0 and p1.first != n-1) { //Si la columna es 0, entonces si la fila no es la última lo movemos para
        // abajo.
        p1.first++;
    } else if(p1.first == n-1 and p1.second != n-1) { //Estamos en última fila, y no estamos en última columna,
        // movemos a derecha.
        p1.second++;
    } else if(p1.second == n-1 and p1.first != 0) { //Última columna, y no en la primera fila, movemos para arriba.
        p1.first--;
    } else { //Estamos en la primera fila, y no estamos en la última columna, entonces movemos a izquierda.
        p1.second--;
    }
    //repetimos lo anterior para p2.
    if(p2.second == 0 and p2.first != n-1) { //Si la columna es 0, entonces si la fila no es la última lo movemos para
        // abajo.
        p2.first++;
    } else if(p2.first == n-1 and p2.second != n-1) { //Estamos en última fila, y no estamos en última columna,
        // movemos a derecha.
        p2.second++;
    } else if(p2.second == n-1 and p2.first != 0) { //Última columna, y no en la primera fila, movemos para arriba.
        p2.first--;
    } else { //Estamos en la primera fila, y no estamos en la última columna, entonces movemos a izquierda.
        p2.second--;
    }
    return;
}

/**
 * Crea un par, de dos vectores (solo para tenerlos todos juntos y ordenados) donde el primer vector tiene en la
 * posición i-ésima al punto del pixel de inicio, de la recta rayo, que pasa por la fila i-ésima, el segundo vector
 * tiene el punto del pixel donde termina dicha recta.
 * @param n tamaño de la matriz imagen resultante, que será del mismo tamaño que la matriz D^k.
 * @result par de vectores con los pixeles inicio fin de las rectas horizontales de toda una matriz imagen de nxn.
 */
pair<vector<pair<uint,uint> >, vector<pair<uint,uint> > > inicios_fines_horizontales(size_t n) {
    pair<vector<pair<uint,uint> >, vector<pair<uint,uint> > > result =
            make_pair(vector<pair<uint,uint> >(n), vector<pair<uint,uint> >(n));
    for(uint i = 0; i < n; i++) {
        result.first[i].first = i;
        result.first[i].second = 0;
        result.second[i].first = i;
        result.second[i].second = n-1;
    }
    return result;
}


/**
 * Crea una matriz D_k del mismo tamaño que la imágen que será el resultado (nxn), donde un 1 en esa posición indica
 * que la recta pasa por ese pixel. (osea, un 1 en D[0][37] indica que el rayo pasa por el pixel de la fila 1,
 * de la columna 38).
 * @param p1 es el primer punto, la función funciona para cualquier punto inicial EN EL BORDE
 * (no me fije que pasa si no esta en el borde, quizá ande)
 * A diferencia de matlab, decidi que para mejorar la comprensión, queda mas claro si lo pensamos matricial y no por ejes.
 * O sea, p1= [3,7], significa que p1 es el punto en el pixel de la fila 4, la columna 8. En matlab hice que sea
 * el punto 3 en el eje x, 7 en el eje y.
 * Por último el algoritmo supone que p1 y p2 son distintos, si son el mismo no funciona.
 * @param p2 es el segundo punto, donde termina la recta (lo mismo que el primer punto, puede ser cualquiera pero en borde)
 * @param n es el tamaño de la matriz resultado del método que estamos desarrollando (Por ahora sería cuadrada).
 * @param result es la matriz resultante con unos donde pasa el rayo, en este caso del tp sería la matriz D^k.
 */
vector<vector<double> > trazar_recta_en_matriz_D(pair<uint,uint> p1, pair<uint,uint> p2, size_t n) {
    vector<vector<double> > result(n, vector<double>(n,0));
    uint inicio;
    uint fin;
    if(p1.second == p2.second) {
        // La recta es vertical, la trazo y termino el método.
        for(uint i = 0; i<n ; i++) {
            result[i][p1.second] = 1;
        }
        return result;
    } else if(p1.second < p2.second) { //me fijo cual va primero.
        inicio = p1.second;
        fin = p2.second;
    } else {
        fin = p1.second;
        inicio = p2.second;
    }
    double a = (double(p2.first) - double(p1.first)) / (double(p2.second) - double(p1.second)); // No importa el orden
            // de p1 y p2, calcula la pendiente, fila 2 menos fila 1 sobre columna 2 menos columna 1.
    double b = double(p1.first) - a * double(p1.second); // calcula b = y - ax, con el punto p1.
    for(uint i = inicio; i<fin ; i++) { //voy de la columna inicio a la fin, pintando los pixeles por los que pase.
        uint pintar_desde = floor(a*i + b);
        uint pintar_hasta = floor(a*(i+1) +b);
        if (pintar_hasta < pintar_desde) {
            uint aux = pintar_desde; //si estan al revez los swappeo
            pintar_desde = pintar_hasta;
            pintar_hasta = aux;
        }
        for(uint j = pintar_desde; j <= pintar_hasta ; j++) {
            result[j][i] = 1;
        }
    }
    uint pintar_desde = floor(a*fin + b);
    uint pintar_hasta = floor(a*(fin+1) +b);
    if (pintar_hasta < pintar_desde) {
        uint aux = pintar_desde; //si estan al revez los swappeo
        pintar_desde = pintar_hasta;
        pintar_hasta = aux;
    }
    for(uint j = pintar_desde; j <= pintar_hasta ; j++) {
        result[j][fin] = 1;
    }
    return result;
}

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
