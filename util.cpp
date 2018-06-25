#include "util.h"

using namespace std;

const vector<string> explode(const string& s, const char& c)
{
    string buff{""};
    vector<string> v;

    for(auto n:s)
    {
        if(n != c) buff+=n; else
        if(n == c && buff != "") { v.push_back(buff); buff = ""; }
    }
    if(buff != "") v.push_back(buff);

    return v;
}


vector<double> convertirAVectorDeValores(vector<string> lecturas) {
    vector<double>* ret = new vector<double>(0);
    for (vector<string>::iterator it = lecturas.begin() ; it != lecturas.end(); ++it) {
        string lectura = *it;
        ret->push_back(stoi(lectura));
    }
    return *ret;
}

vector<vector<double>>* leerCSV(string nombreArchivo) {
    fstream entrada(nombreArchivo, ios_base::in);
    if(entrada.fail()){
        throw runtime_error("no se existe el archivo " + nombreArchivo + "!");
    }
    vector<vector<double>>* ret = new vector<vector<double>>(0);

    string lectura;
    vector<string> lecturas;
    bool path = true;
    while(entrada >> lectura) {
        ret->push_back(convertirAVectorDeValores(explode(lectura, ',')));
    }
    entrada.close();
    return ret;
}

vector<vector<double>> discretizar(const vector<vector<double> >& mat, uint val){//supongo que la matriz mat es cuadrada
	vector<vector<double>> res (mat.size()/val, vector<double> (mat.size()/val));
	for(uint i = 0; i< res.size(); i++){
		for(uint j = 0; j < res.size(); j++){
			double temp = 0;
			for (uint k = 0+i*val; k < (i+1)*val; k++){
				for(uint h = 0+j*val; h < (j+1)*val; h++){
					temp+= mat[k][h];
				}
			}
			res[i][j] = temp/(val*val);
		}
	}
	return res;
}

map<uint, double> pasarAMap(const vector<vector<double> >& mat){
    map<uint, double> res;
    for(uint i = 0; i< mat.size(); i++){
        for(uint j = 0; j < mat[0].size(); j++){
            if(abs(mat[i][j]) > 0.001) {
                res[i*mat[0].size()+j] = mat[i][j];
            }
        }
    }
    return res;

}

vector<double> pasarAVector(const vector<vector<double> >& mat){
	vector<double> res (mat.size()*mat[0].size(),0);
	for(uint i = 0; i< mat.size(); i++){
		for(uint j = 0; j < mat[0].size(); j++){
			res[i*mat[0].size()+j] = mat[i][j];
		}
	}
	return res;

}
/*

*/
/**
 * Genera Matriz con todos los D_kij
 * @param tamMatriz tamaño de la imagen discretizada.
 * @param horizontales Si se desea generar rayos fijos se coloca en true, si se coloca en false se crean n rayos horizontales y se rotan 2n veces.
 * @return La matriz D con todos los D_k (falta convertir las matrices D_k en vectores.
 *//*

VectorMapMatrix  generarRayos(size_t tamMatriz, bool fijos) {
    // creamos un laser de cada una de las esquinas
    vector<pair<uint,uint> > laseres = crearLaseres(tamMatriz, tamMatriz/4, tamMatriz/8, 0); //tamano, despues cada_cuanta_dist, offset, max_cant de rayos.
    vector<pair<uint,uint> > sensores = crearPuntosDeFin(laseres, tamMatriz);

    VectorMapMatrix D_ks((tamMatriz^2)*6, tamMatriz*tamMatriz); // Este es el vector con las matrices D, para cada uno de los K rayos (hay que convertirlas en vectores).
    //Tenemos 2n rayos que rotaremos aproximadamente 3n veces.

    vector<vector<double> > D_k; //matriz auxiliar del D_k del laser calculado recien.
    while(sensores[0].first != tamMatriz - 1 or sensores[0].second != 0) { //Esto quiza es dificil de ver, pero para los laseres izquierdos
        // que se saltean el horizontal, este es la ultima posicion interesante a la que apuntan. NOTA IMPORTANTE,
        // SI SE HACEN MAS DE UN SALTO PUEDE QUE ESTO NO TERMINE. ASIQUE CUIDADO CON PONER MAS DE UN rotarLaseres.
        for(uint i = 0; i < laseres.size(); i++) {
            D_k = trazar_recta_en_matriz_D(laseres[i], sensores[i], tamMatriz);
            map<uint, double> D_k_map = pasarAMap(D_k);
            D_ks.agregarFila(D_k_map);
        }
        rotarLaseres(laseres,sensores,tamMatriz);
    }

    return D_ks;

    */
/*for (int i = 0; i<tamMatriz; i++) {
//		trazar_recta_en_matriz_D(laseres, sensores, tamMatriz);

    }*//*

}
*/

VectorMapMatrix  generarRayos_barrido_H(size_t tamMatriz, size_t cada_cuanto) {
    vector<pair<uint,uint> > laseres = crearLaseres(tamMatriz, cada_cuanto, cada_cuanto/2, 0);
    vector<pair<uint,uint> > sensores = crearPuntosDeFin(laseres, tamMatriz);
    VectorMapMatrix D_ks(0, tamMatriz*tamMatriz);
    while(sensores[0].first != tamMatriz - 1 or sensores[0].second != 0) { //Esto quiza es dificil de ver, pero para los laseres izquierdos
        // que se saltean el horizontal, este es la ultima posicion interesante a la que apuntan. NOTA IMPORTANTE,
        // SI SE HACEN MAS DE UN SALTO PUEDE QUE ESTO NO TERMINE. ASIQUE CUIDADO CON PONER MAS DE UN rotarLaseres.
        for(uint i = 0; i < laseres.size(); i++) {
            map<uint, double> D_k_map = pasarAMap(trazar_recta_en_matriz_D(laseres[i], sensores[i], tamMatriz));
            D_ks.agregarFila(D_k_map);
        }
        barrerLaseres_H(laseres,sensores,tamMatriz);
    }
    return D_ks;
}

vector<double> uniformNoise(const vector<double>& t, double init, double end, double sign){
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

VectorMapMatrix getTraspuesta(VectorMapMatrix &W) {
    VectorMapMatrix ret(W.cantColumnas(), W.cantFilas());

    for(uint i = 0; i < W.cantFilas(); ++i)
        for (unsigned int j=0; j<W.cantColumnas(); ++j)
            ret.asignar(j, i, W.at(i, j));
    return ret;

}

double ECM(vector<double> original, vector<double> reconstruido) {
    uint n = original.size();
    double ret = 0;
    double dif;
    for(uint i = 0; i< n; i++){
        dif = original[i] - reconstruido[i];
        ret += dif*dif;
    }
    return ret/n*n;
}

/**
 *
 * @param discretizacion: cantidad de pixeles de lado y alto por casillero.
 * @param ruido: intervalo del porcentaje de ruido (expresado como valor entre 0 y 1)
 * @param espacio_entre_censores
 */
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
}