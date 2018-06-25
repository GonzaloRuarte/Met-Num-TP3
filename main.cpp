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
#include "util.h"
#include <cmath>
#include <chrono>
#include "VectorMapMatrix.h"


using namespace std;


//VectorMapMatrix getTraspuesta(VectorMapMatrix &W)
/**
 * esta funcion toma como parametros las matrices D y V
 * @return los tiempos que tardan las seniales en atravesar el cuerpo
 */
vector<double> multMatPorVect(const vector<vector<double> > &M, const vector<double> &v){//recordar que el vector v son las inversas de las velocidades
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

//vector<double> uniformNoise(const vector<double>& t, double init, double end, double sign

VectorMapMatrix multMatPorMat(VectorMapMatrix &mat1, VectorMapMatrix &mat2) {
    	const unsigned long& n = mat1.cantFilas();
    	const unsigned long& m = mat2.cantColumnas();
    	const unsigned long& l = mat2.cantFilas();
    	VectorMapMatrix res = VectorMapMatrix(n, m);
	
    	for (uint i = 0; i < n; i++){
        	for (uint j = i; j < m; j++){
			double acum = 0;
			for (uint k = 0; k < l; k++){
                		acum += mat1.at(i,k)*mat2.at(k,j);
			}
			res.asignar(i,j,acum);
			res.asignar(j,i,acum);
		}
	}
    	return res;
}

vector<vector<uint16_t>> datosAMatriz(uchar &datos, uint ancho, uint alto) {
	vector<vector<uint16_t> > ret (0);
	for (size_t i = 0; i < alto; ++i) {
		vector<uint16_t> fila (0);
		for (size_t j = 0; j < ancho; ++j) {
			int indice = (i*ancho)+j;
			int16_t bt = static_cast<int16_t>(*((char *)&datos + indice));
			fila.push_back(bt);
		}
		ret.push_back(fila);
	}
	return ret;
}

unsigned char* readPPM(const char* fileName, char* pSix, int* width, int* height) {

	// open the file to read just the header reading
	FILE* fr = fopen(fileName, "r");

	// formatted read of header
	fscanf(fr, "%s", pSix);

	// check to see if it's a PPM image file
/*		if (strncmp(pSix, "P6" , 10) != 0) {
		printf("They are not the same\n");
	} else {
		printf("They are the same\n");
	}*/

	// read the rest of header
	fscanf(fr, "%d\n %d\n", width, height);

	//fscanf(fr, "%d\n", maximum);

	// check to see if they were stored properly
	printf("PSix: %s\n", pSix);
	printf("Width: %d\n", *width);
	printf("Height: %d\n", *height);
//		printf("maximum: %d\n", *maximum);

	int size = (*width) * (*height);
	//int size = 423800;

	// allocate array for pixels
	unsigned char* pixels = new unsigned char[size];

	// unformatted read of binary pixel data
	while (fread(pixels, sizeof(int), 128, fr)) {
		printf("%s", pixels);
	} // end of for loop

	// close file
	fclose(fr);

	// return the array
	return pixels;

} // end of readPPM


vector<vector<double>> obtenerTrayectorias() {

/*	for (vector<string>::iterator it = lecturas.begin(); it != lecturas.end(); ++it) {

	}*/
}

bool esTraspuesta(VectorMapMatrix &D, VectorMapMatrix &Dt) {
	bool ret = true;
	for (int i=0; i<D.cantFilas(); i++) {
		for (int j=0; j<D.cantColumnas(); j++) {
			ret = ret && D[i][j] == Dt[j][i];
		}
	}
	return ret;
}


vector<double> reconstruirCuerpo(string nombreAchivoEntrada, vector<double>* V, uint tamanoDiscretizacion, double inicioRuido, double finRuido, double signoRuido) {
	vector<vector<double> >* cuerpo;
	// 1) tomamos la imagen
	cuerpo = leerCSV(nombreAchivoEntrada);
	
	// 2) la discretizamos
	vector<vector<double> > cuerpoDiscretizado = discretizar(*cuerpo, tamanoDiscretizacion);
	size_t tamMatriz = cuerpoDiscretizado.size();
	// 3) obtenemos D (la matriz con las trayectorias de los rayos
	VectorMapMatrix  D = generarRayos(tamMatriz,true);
	// 4) pasamos la imagen discretizada a vector
	vector<double> Vtemp = pasarAVector(cuerpoDiscretizado);
	V = &Vtemp;
	// 5) invertimos el vector V
	vector<double> Vinv (V->size(), 0);
	for (uint i = 0; i< V->size(); i++){
		if ((*V)[i] != 0){
			Vinv[i] = 1/(*V)[i];
		}
	}
	// 6) multiplicamos la matriz D por el vector V invertido
	vector<double> T = D*Vinv;
	// 7) le aplicamos ruido al vector T
	vector<double> vectorCuerpoDiscretizadoConRuido = uniformNoise(T, inicioRuido, finRuido, signoRuido);
	// 8) generamos DtD
	VectorMapMatrix Dt = getTraspuesta(D);
	VectorMapMatrix DtD = Dt*D;//multMatPorMat(Dt,D);
	// 9) generamos el vector Dt*T
	vector<double> DtT = Dt*T;
	// 10) resolvemos el sistema DtDx = DtT con EG
	pair<vector<double>,short> solucion = DtD.EG(DtD, DtT);
	// invertir los valores de la solucion y volverlo a pasar a matriz para luego convertirlo en una imagen que podamos ver
	return solucion.first;
}

//double ECM(vector<double> original, vector<double> reconstruido)


vector<double>& medirErrorDeReconstruccion(string nombreDirectorioEntrada, uint tamanoDiscretizacion, double inicioRuido, double finRuido, double signoRuido) {
	vector<string> listadoDirectorio;
    listarDirectorio(nombreDirectorioEntrada, listadoDirectorio);
	vector<double>* cuerpoDiscretizado;
	vector<double> cuerpoDiscretizaqdoReconstruido;
    vector<double> ECMs (0);
	for (int i=0; i < listadoDirectorio.size(); i++) {
		cuerpoDiscretizaqdoReconstruido = reconstruirCuerpo(listadoDirectorio[i], cuerpoDiscretizado, tamanoDiscretizacion, inicioRuido, finRuido, signoRuido);
    	//cout << listadoDirectorio[i] << endl;
    	ECMs.push_back(ECM(*cuerpoDiscretizado, cuerpoDiscretizaqdoReconstruido));
    }
    return ECMs;
}


int main(int argc, char * argv[]) {

	//reconstruirCuerpos("dicom_csv2", 4, 3, 2, 1);

	//VectorMapMatrix  D = generarRayos(500,true);
    vector<double>* cuerpo;
    //cuerpo = leerCSV("dicom_csv2/1.2.826.0.1.3680043.2.656.1.138.1.csv");

	//cout << (*matriz)[0].size() << endl;

	vector<double> asd = reconstruirCuerpo("dicom_csv2/1.2.826.0.1.3680043.2.656.1.138.1.csv", cuerpo, 16, 0, 1, 0.5);

/*	vector<vector<double>> mat(20,vector<double> (20,0));

	for(uint i = 0; i< mat.size(); i++){
		for(uint j = 0; j < mat[0].size(); j++){
			mat[i][j]=i*2+j*2;
			cout << mat[i][j] << " ";
			
		}
		cout << endl;
		
		
	}
	vector<vector<double>> disc = discretizar(mat,5);
	for(uint i = 0; i< disc.size(); i++){
		for(uint j = 0; j < disc[0].size(); j++){
			cout << disc[i][j] << " ";
			
		}
		cout << endl;
		
		
	}
	vector<double> vec = pasarAVector(disc);
	for(uint i = 0; i< vec.size(); i++){
		cout << vec[i]<< " ";
	}
	cout << endl;
*/	return 0;
}



