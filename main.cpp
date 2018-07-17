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

bool esTraspuesta(VectorMapMatrix &D, VectorMapMatrix &Dt) {
	bool ret = true;
	for (uint i=0; i<D.cantFilas(); i++) {
		for (uint j=0; j<D.cantColumnas(); j++) {
			ret = ret && D[i][j] == Dt[j][i];
		}
	}
	return ret;
}

vector<double> reconstruirCuerpo(string nombreAchivoEntrada, vector<double>& vecCuerpoDiscretizado, uint tamanoDiscretizacion, int metodo, double inicioRuido, double finRuido, double signoRuido, size_t& ancho) {
	vector<vector<double> > cuerpo;
	// 1) tomamos la imagen
	cuerpo = *leerCSV(nombreAchivoEntrada);

	// 2) la discretizamos
	vector<vector<double> > cuerpoDiscretizado = discretizar(cuerpo, tamanoDiscretizacion);
	size_t tamMatriz = cuerpoDiscretizado.size();
    ancho = cuerpoDiscretizado.size();
	// 3) obtenemos D (la matriz con las trayectorias de los rayos
	VectorMapMatrix  D = generarRayos(tamMatriz, metodo, tamMatriz/2, 1); //tamaño discretizado, metodo a utilizar, cantidad de rayos, pixeles salteados-1.
	// 4) pasamos la imagen discretizada a vector
    vecCuerpoDiscretizado = pasarAVector(cuerpoDiscretizado);
	// 5) invertimos el vector vecCuerpoDiscretizado
	/*vector<double> Vinv (vecCuerpoDiscretizado->size(), 0);
	for (uint i = 0; i< vecCuerpoDiscretizado->size(); i++){
		if ((*vecCuerpoDiscretizado)[i] != 0){
			Vinv[i] = 1/(*vecCuerpoDiscretizado)[i];
		}
	} No hay que invertir.*/
	// 6) multiplicamos la matriz D por el vector vecCuerpoDiscretizado
	vector<double> T = D*vecCuerpoDiscretizado;
	// 7) le aplicamos ruido al vector T
	vector<double> Tr = uniformNoise(T, inicioRuido, finRuido, signoRuido);
	// 8) generamos DtD
	VectorMapMatrix Dt = getTraspuesta(D);
	vector<vector<double>> DtD = Dt*D;//multMatPorMat(Dt,D);
	
	// 9) generamos el vector Dt*T
	vector<double> DtT = Dt*Tr;
	// 10) resolvemos el sistema DtDx = DtT con EG
	pair<vector<double>,short> solucion = EG2(DtD, DtT);
	/*vector<double> Check (vecCuerpoDiscretizado->size(), 0);
	for (uint i = 0; i< vecCuerpoDiscretizado->size(); i++){
		if (abs(solucion.first[i]) > 0.00001){
			Check[i] = 1/solucion.first[i];
		}
	} No hay que invertir.*/

	cout << "Cantidad del metodo" << metodo << ": " << D.cantFilas() << "; status del sistema: " << solucion.second << "; ECM: " << ECM(solucion.first, vecCuerpoDiscretizado) << endl;
	return solucion.first;
}

//double ECM(vector<double> original, vector<double> reconstruido)

vector<double> medirErrorDeReconstruccion(string nombreDirectorioEntrada, uint tamanoDiscretizacion, int metodo, double inicioRuido, double finRuido, double signoRuido) {
    vector<string> listadoDirectorio;
    listarDirectorio(nombreDirectorioEntrada, listadoDirectorio);
    vector<double> cuerpoDiscretizado;
    vector<double> cuerpoDiscretizaqdoReconstruido;
    vector<double> ECMs (0);
    size_t ancho;
    for (uint i=0; i < listadoDirectorio.size(); i++) {
        cuerpoDiscretizaqdoReconstruido = reconstruirCuerpo(listadoDirectorio[i], cuerpoDiscretizado, metodo, tamanoDiscretizacion, inicioRuido, finRuido, signoRuido, ancho);
        //cout << listadoDirectorio[i] << endl;
        ECMs.push_back(ECM(cuerpoDiscretizado, cuerpoDiscretizaqdoReconstruido));
    }
    return ECMs;
}

/*void experimentacionVariandoElRuido() {
    string directorio = "imagenes_para_probar";
    vector<string> archivos;
    listarDirectorio(directorio, archivos);
    string carpeta_salida = "resultados de prueba";
    uint tamanio_imagenes = 512;
    vector<unsigned short int> discretizaciones = {16, 32, 64};
    vector<unsigned short int> cantidades_de_fuentes = {1, 2, 4, 8};
    vector<unsigned short int> separaciones = {1, 2, 4, 8};

    vector<pair<float,float> > ruidos;
    for (float i=0.0; i<=   0.2; i=i+0.01) {
        ruidos.push_back(make_pair(i, i));
    }
    uint16_t repeticiones = 20;

    experimentacion('H', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);

void experimentacionVariandoSeparaciones() {
    string directorio = "imagenes_para_probar";
    vector<string> archivos;
    listarDirectorio(directorio, archivos);
    string carpeta_salida = "resultados de prueba";
    uint tamanio_imagenes = 512;
    vector<unsigned short int> discretizaciones = {16, 32};
    vector<unsigned short int> cantidades_de_fuentes = {4, 16};
    vector<unsigned short int> separaciones = {1, 8};
	for(uint i = 1; i<=16; i+=2){
		separaciones.push_back(i);
	}
    vector<pair<float,float> > ruidos;

    ruidos.push_back(make_pair(0.005, 0.005));
        ruidos.push_back(make_pair(0.1, 0.1));
    uint16_t repeticiones = 20;

    experimentacion('H', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, carpeta_salida, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos, repeticiones);
}*/

void variacion_de_parametros_con_imagenes_de_la_catedra(){
    vector<string> archivos;
    listarDirectorio("Imagenes con padding/128", archivos);
    string var_dis128 = "128Variando discretizacion";
    string var_fuen128 = "128Variando cant. de fuentes";
    string var_sep128 = "128Variando separacion";
    string var_rui128 = "128Variando ruido";
    uint tam_imag = 128;
    vector<unsigned short int> discretizacion32 = {tam_imag/32}; //4
    vector<unsigned short int> discretizacion16 = {tam_imag/16}; //8
    vector<unsigned short int> discretizacion8 = {tam_imag/8};   //16. Cuando varío discretización
    vector<unsigned short int> cantidades_de_fuentes32 = {32/4, 32}; //
    vector<unsigned short int> cantidades_de_fuentes16 = {16/4, 16};
    vector<unsigned short int> cantidades_de_fuentes8  = { 8/4,  8};
    vector<unsigned short int> separaciones = {1, 4};
    vector<pair<float,float> > ruidos = {make_pair(0.005, 0.005), make_pair(0.1, 0.1)};
    uint16_t repeticiones = 20;

    vector<unsigned short int> cantidades_de_fuentes32V = {32/8, 32/2};
    vector<unsigned short int> cantidades_de_fuentes16V = {16/8, 16/2};
    vector<unsigned short int> separacionesV = {2, 8};
    vector<pair<float,float> > ruidosV;
    for(float i = 0.0; i < 0.1; i += 0.02)
        ruidosV.push_back(make_pair(i,i));

    experimentacion('H', archivos, var_dis128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_dis128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_dis128, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis128, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis128, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);

    experimentacion('H', archivos, var_fuen128, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_fuen128, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_fuen128, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_fuen128, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_fuen128, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_fuen128, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);

    experimentacion('H', archivos, var_sep128, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('V', archivos, var_sep128, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('r', archivos, var_sep128, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('H', archivos, var_sep128, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);
    experimentacion('V', archivos, var_sep128, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);
    experimentacion('r', archivos, var_sep128, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);

    experimentacion('H', archivos, var_rui128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('V', archivos, var_rui128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('r', archivos, var_rui128, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('H', archivos, var_rui128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);
    experimentacion('V', archivos, var_rui128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);
    experimentacion('r', archivos, var_rui128, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);


    tam_imag = 1024;
    discretizacion32 = {tam_imag/32}; //32
    discretizacion16 = {tam_imag/16}; //64
    discretizacion8 = {tam_imag/8};   //128. Cuando varío discretización
    string var_dis1024 = "1024Variando discretizacion";
    string var_fuen1024 = "1024Variando cant. de fuentes";
    string var_sep1024 = "1024Variando separacion";
    string var_rui1024 = "1024Variando ruido";

    experimentacion('H', archivos, var_dis1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_dis1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_dis1024, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_dis1024, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_dis1024, tam_imag, discretizacion8, cantidades_de_fuentes8, separaciones, ruidos, repeticiones);

    experimentacion('H', archivos, var_fuen1024, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_fuen1024, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_fuen1024, tam_imag, discretizacion32, cantidades_de_fuentes32V, separaciones, ruidos, repeticiones);
    experimentacion('H', archivos, var_fuen1024, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);
    experimentacion('V', archivos, var_fuen1024, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);
    experimentacion('r', archivos, var_fuen1024, tam_imag, discretizacion16, cantidades_de_fuentes16V, separaciones, ruidos, repeticiones);

    experimentacion('H', archivos, var_sep1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('V', archivos, var_sep1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('r', archivos, var_sep1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separacionesV, ruidos, repeticiones);
    experimentacion('H', archivos, var_sep1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);
    experimentacion('V', archivos, var_sep1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);
    experimentacion('r', archivos, var_sep1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separacionesV, ruidos, repeticiones);

    experimentacion('H', archivos, var_rui1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('V', archivos, var_rui1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('r', archivos, var_rui1024, tam_imag, discretizacion32, cantidades_de_fuentes32, separaciones, ruidosV, repeticiones);
    experimentacion('H', archivos, var_rui1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);
    experimentacion('V', archivos, var_rui1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);
    experimentacion('r', archivos, var_rui1024, tam_imag, discretizacion16, cantidades_de_fuentes16, separaciones, ruidosV, repeticiones);
}

void reconstruccion_especifica(uint16_t cantidad_celdas, uint16_t metodo, uint16_t cant_rayos_inic, uint16_t separacion, string nom_arch_datos, string carpeta_archivos_salida, const vector<double>& vecCuerpoDiscretizado){
    VectorMapMatrix D = generarRayos(cantidad_celdas, metodo, cant_rayos_inic, separacion);
    vector<double> T = D*vecCuerpoDiscretizado;
    VectorMapMatrix Dt = getTraspuesta(D);
    vector<vector<double>> DtD = Dt*D;
    vector<double> DtT = Dt*T;
    pair<vector<double>,short> solucion = EG2(DtD, DtT);
    char char_metodo;
    switch(metodo){
        case 0: char_metodo = 'R'; break;
        case 1: char_metodo = 'H'; break;
        case 2: char_metodo = 'V'; break;
        case 3: char_metodo = 'O'; break;
        case 4: char_metodo = 'o'; break;
        case 5: char_metodo = 'h'; break;
        case 6: char_metodo = 'v'; break;
    }
    ofstream arch_datos(nom_arch_datos, ios::app);
    arch_datos << "Estrategia = " << char_metodo << ", cantidad rayos iniciales =" << cant_rayos_inic << ", separación =" << separacion << ":" << endl;
    arch_datos << "Cantidad de rayos: " << D.cantFilas() << ";\tstatus de la EG: " << solucion.second << ";\tECM: " << ECM(solucion.first, vecCuerpoDiscretizado) << endl << endl;
    string nombre_arch_reconst = carpeta_archivos_salida+"/Reconstr. "+char_metodo+","+to_string(cant_rayos_inic)+","+to_string(separacion)+".csv";
    escribirCSV(nombre_arch_reconst, solucion.first, cantidad_celdas);
}

void reconstrucciones_especificas(const string& archivo_imagen_entrada, string carpeta_archivos_salida, uint16_t cantidad_celdas){
    string nom_arch_datos = carpeta_archivos_salida+"/datos de las reconstrucciones.txt";
    ofstream arch_datos(nom_arch_datos);
    arch_datos << "En todos los casos el tamaño de las reconstrucciones es de " << cantidad_celdas << endl << endl;
    arch_datos.close();
    vector<vector<double> > cuerpo = *leerCSV(archivo_imagen_entrada);
    uint discretizacion_para_cantidad_celdas = cuerpo.size()/cantidad_celdas;     //Discretización necesaria para tener una grilla de "cantidad_celdas"x"cantidad_celdas".
    vector<vector<double> > cuerpoDiscretizado = discretizar(cuerpo, discretizacion_para_cantidad_celdas);
    vector<double> vecCuerpoDiscretizado = pasarAVector(cuerpoDiscretizado);
    string nombre_arch_reconst = carpeta_archivos_salida+"/Reconstr. del original.csv";
    escribirCSV(nombre_arch_reconst, vecCuerpoDiscretizado, cantidad_celdas);
    //R con máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 0, cantidad_celdas, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //R con 1/2 de la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 0, cantidad_celdas/2, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //R con la máxima cant. de rayos iniciales y separación 2.
    reconstruccion_especifica(cantidad_celdas, 0, cantidad_celdas, 2, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);

    //H con la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //H con la máxima cant. de rayos iniciales y separación 2.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas, 2, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //H con la máxima cant. de rayos iniciales y separación 4.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas, 4, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //H con 1/2 de la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas/2, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //H con 1/2 de la máxima cant. de rayos iniciales y separación 2.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas/2, 2, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //H con 1/4 de la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 1, cantidad_celdas/4, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);

    //V con la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //V con la máxima cant. de rayos iniciales y separación 2.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas, 2, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //V con la máxima cant. de rayos iniciales y separación 4.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas, 4, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //V con 1/2 de la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas/2, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //V con 1/2 de la máxima cant. de rayos iniciales y separación 2.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas/2, 2, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
    //V con 1/4 de la máxima cant. de rayos iniciales y separación 1.
    reconstruccion_especifica(cantidad_celdas, 2, cantidad_celdas/4, 1, nom_arch_datos, carpeta_archivos_salida, vecCuerpoDiscretizado);
}

//------------------------ Parseo de la entrada -------------------------------//

bool contiene(char *argv[], const string *cadena) {
    string param1 = argv[1], param2 = argv[3], param3 = argv[5];//, param4 = argv[5];
    return param1.compare(*cadena) || param2.compare(*cadena) || param3.compare(*cadena); //|| param4.compare(*cadena);
}

string obtener(char *argv[], const string *cadena) {
    string ret;
    string param1 = argv[1], param2 = argv[3], param3 = argv[5];//, param4 = argv[7];

    if (param1.compare(*cadena) == 0) ret = argv[2];
    if (param2.compare(*cadena) == 0) ret = argv[4];
    if (param3.compare(*cadena) == 0) ret = argv[6];
//    if (param4.compare(*cadena) == 0) ret = argv[8];
    return ret;
}

bool obtenerParametros(int argc, char * argv[], string *ruido, string *nombreArchivoEntrada, string *nombreArchivoSalida) {
    bool ret = false;
    const string param1 = "-r", param2 = "-i", param3 = "-o";
    bool estan_todos = argc==7;
    estan_todos = estan_todos && contiene(argv, &param1);
    estan_todos = estan_todos && contiene(argv, &param2);
    estan_todos = estan_todos && contiene(argv, &param3);
    //if (argc == 7 && contiene(argv, &param1) && contiene(argv, &param2) && contiene(argv, &param3)) {
    if (estan_todos) {
        *ruido = obtener(argv, &param1);
        *nombreArchivoEntrada = obtener(argv, &param2);
        *nombreArchivoSalida = obtener(argv, &param3);
        ret = (ruido != NULL && nombreArchivoEntrada != NULL && nombreArchivoSalida != NULL);
    }
    return ret;
}
//------------------------ Parseo de la entrada -------------------------------//

int main(int argc, char * argv[]) {

    //Viendo centidad de rayos:
    /*VectorMapMatrix R = generarRayos(32, 0, 32, 1);
    VectorMapMatrix H = generarRayos(32, 1, 32/2, 1);
    cout << "cant. rayos de R: " << R.cantFilas() << endl;
    cout << "cant. rayos de H: " << H.cantFilas() << endl;*/

    string nombreAchivoEntrada;
    string nombreAchivoSalida;
    vector<double> vecCuerpoDiscretizado;
    uint discretizacion;
    string ruido;
    size_t ancho;
    if (!obtenerParametros(argc, argv, &ruido, &nombreAchivoEntrada, &nombreAchivoSalida)) {
        cout << "Modo de uso: tp3 -r <nivel_ruido> -i <nombre_archivo_entrada> -o <nombre_archivo_salida>\n";
    } else {
        double nivelRuido = atof(ruido.c_str());
        //Comparación de las reconstrucciones de las distintas estratégias.
        /*vector<double> reconstruccionR = reconstruirCuerpo(nombreAchivoEntrada, vecCuerpoDiscretizado, 16, 0, 0, nivelRuido, 0, ancho);
        escribirCSV("Resultados_de_reconstrucción/reconstruccionR.csv", reconstruccionR, ancho);
        vector<double> reconstruccionH = reconstruirCuerpo(nombreAchivoEntrada, vecCuerpoDiscretizado, 16, 1, 0, nivelRuido, 0, ancho);
        escribirCSV("Resultados_de_reconstrucción/reconstruccionH.csv", reconstruccionH, ancho);
        vector<double> reconstruccionV = reconstruirCuerpo(nombreAchivoEntrada, vecCuerpoDiscretizado, 16, 2, 0, nivelRuido, 0, ancho);
        escribirCSV("Resultados_de_reconstrucción/reconstruccionV.csv", reconstruccionV, ancho);
        escribirCSV("Resultados_de_reconstrucción/original discretizado.csv", vecCuerpoDiscretizado, ancho);*/
        //Otra comparación de las reconstrucciones de las distintas estratégias.
        reconstrucciones_especificas(nombreAchivoEntrada, "Resultados_de_reconstrucción", 32);
        //Lo que debe quedar en la entrega:
        /*vector<double> reconstruccion = reconstruirCuerpo(nombreAchivoEntrada, vecCuerpoDiscretizado, 16, 0, 0, nivelRuido, 0, ancho);
        escribirCSV(nombreAchivoSalida, reconstruccion, ancho);*/
    }

    return 0;
}
