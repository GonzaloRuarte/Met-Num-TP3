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
 * @return el tiempo que tarda la senial en atravesar el cuerpo
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

//pair<vector<double>,short> EG2(vector<vector<double>> &mat, vector<double> bb)


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

//vector<double> uniformNoise(vector<double> t, double init, double end, double sign)

vector<vector<uint16_t>> datosAMatriz(uchar &datos, uint ancho, uint alto) {
	vector<vector<uint16_t>> ret (0);
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

//VectorMapMatrix  generarRayos(size_t tamMatriz, int metodo_usado, int cantLaseres, int saltear_hasta_n)

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
	VectorMapMatrix  D = generarRayos(tamMatriz,0, 32, 4); //tamaño discretizado, metodo a utilizar, cantidad de rayos, pixeles salteados-1.
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
	vector<double> Tr = uniformNoise(T, inicioRuido, finRuido, signoRuido);
	// 8) generamos DtD
	VectorMapMatrix Dt = getTraspuesta(D);
	vector<vector<double>> DtD = Dt*D;//multMatPorMat(Dt,D);
	
	// 9) generamos el vector Dt*T
	vector<double> DtT = Dt*Tr;
	// 10) resolvemos el sistema DtDx = DtT con EG
	pair<vector<double>,short> solucion = EG2(DtD, DtT);
	vector<double> Check (V->size(), 0);
	for (uint i = 0; i< V->size(); i++){
		if (abs(solucion.first[i]) > 0.00001){
			Check[i] = 1/solucion.first[i];
		}
	}

	cout << ECM(*V,Check) << endl;
	// invertir los valores de la solucion y volverlo a pasar a matriz para luego convertirlo en una imagen que podamos ver
	return solucion.first;
}



//double ECM(vector<double> original, vector<double> reconstruido)

vector<double> medirErrorDeReconstruccion(string nombreDirectorioEntrada, uint tamanoDiscretizacion, double inicioRuido, double finRuido, double signoRuido) {
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
/*
void experimentacion_barrido_H(unsigned char discretizacion, pair<float,float> ruido, unsigned short int espacio_entre_censores){
   vector<vector<double> > *imagen_entera = leerCSV(archivo_imagen);
    vector<vector<double> > imagen_discreta = discretizar(*imagen_entera, discretizacion);
    vector<double> vec_imagen_discreta = pasarAVector(imagen_discreta);
    VectorMapMatrix D = generarRayos_barrido_H(imagen_discreta.size(), espacio_entre_censores);
    vector<double> t_sin_ruido = D*vec_imagen_discreta;
    vector<double> t_con_ruido = uniformNoise(t_sin_ruido, ruido.first, ruido.second, 0);
    VectorMapMatrix Dt_D = getTraspuesta(D)*D;
    pair<vector<double>, short> v = Dt_D.EG(Dt_D, t_con_ruido);
    double error = ECM(vec_imagen_discreta, v.first);
    ofstream salida(archivo_salida);
    salida << error;
    salida.close();
    return;
}*/

int main(int argc, char * argv[]) {

	//reconstruirCuerpos("dicom_csv2", 4, 3, 2, 1);
	
	//VectorMapMatrix  D = generarRayos(500,true);
    vector<double>* cuerpo;
    //cuerpo = leerCSV("dicom_csv2/1.2.826.0.1.3680043.2.656.1.138.1.csv");

	//cout << (*matriz)[0].size() << endl;

	vector<double> asd = reconstruirCuerpo("dicom_csv2/1.2.826.0.1.3680043.2.656.1.138.1.csv", cuerpo, 16, 0.4, 0.5, 0);

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



