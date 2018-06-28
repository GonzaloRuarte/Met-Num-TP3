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


#include "calcular_rayos.h"
#include "util.h"
#include "VectorMapMatrix.h"

using namespace std;

int main() {
    /*
    pair<uint,uint> p1 = make_pair(uint(31),uint(24));
    pair<uint,uint> p2 = make_pair(uint(0),uint(5));
    int n = 32;
    vector<vector<double> > D = trazar_recta_en_matriz_D(p1,p2,n);
    pair<uint,uint> p3 = make_pair(uint(31),uint(4));
    pair<uint,uint> p4 = make_pair(uint(0),uint(7));
    int n2 = 32;
    vector<vector<double> > D2 = trazar_recta_en_matriz_D(p3,p4,n2);

    vector<pair<uint,uint> > Laseres = crearLaseres(500,25,12,0);

    vector<pair<uint,uint> > Miras = crearPuntosDeFin(Laseres,500);

    vector<vector<double> > D = trazar_recta_en_matriz_D(Laseres[0],Miras[0],500);

    barrerLaseres_H(Laseres,Miras,500);
    barrerLaseres_H(Laseres,Miras,500);
    barrerLaseres_H(Laseres,Miras,500);
    barrerLaseres_H(Laseres,Miras,500);
    barrerLaseres_H(Laseres,Miras,500);

    D = trazar_recta_en_matriz_D(Laseres[0],Miras[0],500);
    */
    vector<unsigned short int> discretizaciones = {32, 64};
    vector<pair<float,float> > ruidos = {make_pair(0.01, 0.04), make_pair(0.05, 0.09)};
    vector<unsigned short int> espacios_entre_censores = {4, 8, 16};
    string directorio = "Imagenes_para_probar";
    vector<string> archivos;
    listarDirectorio(directorio, archivos);
    uint tamanio_imagenes = 512;
    vector<unsigned short int> cantidades_de_fuentes = {4, 8, 16, 32, 64};
    vector<unsigned short int> separaciones = {1, 2, 4, 6, 8, 12, 16};
    //experimentacion_barrido_H(directorio, tamanio_imagenes, discretizaciones, ruidos, espacios_entre_censores);
/*    vector<string> archivos;
    listarDirectorio(directorio, archivos);
    for(size_t ind_disc = 0; ind_disc < discretizaciones.size(); ++ind_disc) {
        for(size_t ind_espac = 0; ind_espac < espacios_entre_censores.size(); ++ind_espac) {
            ofstream salida("resultados de prueba/Discretización:" + to_string(discretizaciones[ind_disc]) + " espaciado:" +to_string(espacios_entre_censores[ind_espac]) + " .txt");
            salida.close();
            for(size_t ind_arch = 0; ind_arch < archivos.size(); ++ind_arch) {
                salida.open("resultados de prueba/Discretización:" + to_string(discretizaciones[ind_disc]) + " espaciado:" +to_string(espacios_entre_censores[ind_espac]) + " .txt", ios::app);
                salida << "Imagen " + archivos[ind_arch] + ":\t";
                salida.close();
                for(size_t ind_ruido = 0; ind_ruido < ruidos.size(); ++ind_ruido){
                    salida.open("resultados de prueba/Discretización:" + to_string(discretizaciones[ind_disc]) + " espaciado:" +to_string(espacios_entre_censores[ind_espac]) + " .txt", ios::app);
                    salida << discretizaciones[ind_disc] << ", " << espacios_entre_censores[ind_espac] << ", " << ruidos[ind_ruido].first << ",\t";
                    salida.close();
                }
                salida.open("resultados de prueba/Discretización:"+to_string(discretizaciones[ind_disc])+" espaciado:"+to_string(espacios_entre_censores[ind_espac])+" .txt", ios::app);
                salida << endl << endl; //Lo hago 2 veces para mejor visibilidad.
                salida.close();
            }
        }
    }*/
    experimentacion('h', archivos, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos);
    experimentacion('v', archivos, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos);
    experimentacion('r', archivos, tamanio_imagenes, discretizaciones, cantidades_de_fuentes, separaciones, ruidos);
    return 0;
}