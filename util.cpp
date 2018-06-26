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

VectorMapMatrix generarRayos_barrido_H(size_t tamMatriz, size_t cada_cuanto) {
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

double ECM(const vector<double>& original, const vector<double>& reconstruido) {
    uint n = original.size();
    double ret = 0;
    double dif;
    for(uint i = 0; i< n; i++){
        dif = original[i] - reconstruido[i];
        ret += dif*dif;
    }
    return ret/n*n;
}

pair<vector<double>,short> EG2(vector<vector<double>> &mat, vector<double> bb) {
    unsigned int i,j,l;
    vector<double> res(mat[0].size(),0);
    short status = 0; //status default, el sistema tiene una unica solucion posible
    double A_kk, A_jk;
    bool cont;

    for(i = 0; i < mat[0].size()-1; i++){ //itero sobre las filas, excepto la ultima porque ahi no tengo que hacer nada
        cont = false;
        for(j = i; j < mat.size(); j++){ //itero sobre las filas desde i en adelante, estaria por fijarme si tengo que hacer o no calculo en el paso i de la EG
            if(abs(mat[j][i]) > 0.00001){ //si no hay un 0 en la posicion j,i
                cont = true;
                if(abs(mat[i][i]) <= 0.00001){
                    mat[i].swap(mat[j]); //cambio de lugar las filas porque habia un 0 en la diagonal pero no en el resto de la columna
                    double temp = bb[i];
                    bb[i] = bb[j];         //como se cambiaron de lugar las filas, también se cambian de lugar los valores de "bb"
                    bb[j] = temp;
                }
                break;
            }
        }
        A_kk = mat[i][i];
        for(j = i+1; j < mat.size()-i-1; j++){ //cálculo del paso i si corresponde
            if (!cont){break;} //si me tengo que saltear este paso no calculo nada
            if(abs(mat[j][i]) >= 0.00001){//si el elemento j,i es 0 no hago nada en la fila j
                A_jk = mat[j][i];
                for(l = i+1; l < mat[0].size()-i-1; l++){
                    mat[j][l] = mat[j][l]-(mat[i][l]*A_jk/A_kk);
                }
                bb[j] -= A_jk/A_kk*bb[i];
            } //no me olvido de actualizar el vector b
        }

    }

    for(i = 0; i < mat[0].size(); i++){
        j = mat[0].size()-1-i;
        if(mat[j][j] == 0 && bb[j] != 0){
            status = -1; //el sistema es incompatible
            break;
        }
        if(mat[j][j] == 0 && bb[j] == 0){
            status = 1; //hay infinitos resultados
            res[j] = 0;
        }
        else{
            res[j] = bb[j]/mat[j][j]; //tengo A_jj*x_j = b_j, paso dividiendo el A_jj

            if (j!=0){
                for(unsigned int l = 0; l < j; l++){
                    bb[l] = bb[l] - res[j]*mat[l][j]; //esto es importante, al b_l con l de 0 a j-1 le paso restando el A_lj*x_j, porque ya conozco el resultado de X_j, de forma que en la siguiente iteracion solo voy a tener algo de esta pinta A_jj*x_j = b_j
                }
            }
        }
    }
    return make_pair(res,status);
}

double operator*(vector<double> u, vector<double> v){   //Deben ser del mismo tamaño.
    double res = 0;
    for(size_t i = 0; i < u.size(); ++i)
        res += u[i]*v[i];
    return res;
}

vector<double> operator*(vector<vector<double> > M, vector<double> v){
    vector<double> res(v.size());
    for(size_t i = 0; i < M.size(); ++i)
        res[i] = M[i]*v;
    return res;
}

/**
 *
 * @param discretizacion: cantidad de pixeles de lado y alto por casillero.
 * @param ruido: intervalo del porcentaje de ruido (expresado como valor entre 0 y 1)
 * @param espacio_entre_censores
 */
void experimentacion_barrido_H(const string& directorio, uint taman_imags, const vector<unsigned char>& discretizaciones, const vector<pair<float,float> >& ruidos, const vector<unsigned short int>& espacios_entre_censores) {   //Necesito saber el tamaño de las imagenes de antemano.
    vector<string> archivos;
    listarDirectorio(directorio, archivos);
    ofstream salida;
    for(size_t ind_disc = 0; ind_disc < discretizaciones.size(); ++ind_disc){
        for(size_t ind_espac = 0; ind_espac < espacios_entre_censores.size(); ++ind_espac){
            VectorMapMatrix D = generarRayos_barrido_H(taman_imags/discretizaciones[ind_disc], espacios_entre_censores[ind_espac]);
            VectorMapMatrix Dt = getTraspuesta(D);
            vector<vector<double> > Dt_D = Dt * D;
            salida.open("Discretización:"+to_string(discretizaciones[ind_disc])+" espaciado:"+to_string(espacios_entre_censores[ind_espac])+" .txt";
            salida.close(); //La intención de estas 2 lineas es poner en blanco el archivo si ya existe.
            for(size_t ind_arch = 0; ind_arch < archivos.size(); ++ind_arch){
                vector<vector<double> > *imagen_entera = leerCSV(archivos[ind_arch]);
                vector<vector<double> > imagen_discreta = discretizar(*imagen_entera, discretizaciones[ind_disc]);
                vector<double> vec_imagen_discreta = pasarAVector(imagen_discreta);
                vector<double> t_sin_ruido = D * vec_imagen_discreta;
                salida.open("Discretización:"+to_string(discretizaciones[ind_disc])+" espaciado:"+to_string(espacios_entre_censores[ind_espac])+" .txt", ios::app);
                salida << "Imagen "+archivos[ind_arch]+":\t";
                salida.close();
                for(size_t ind_ruido = 0; ind_ruido < ruidos.size(); ++ind_ruido){
                    vector<double> t_con_ruido = uniformNoise(t_sin_ruido, ruidos[ind_ruido].first, ruidos[ind_ruido].second, 0);
                    pair<vector<double>, short> v = EG2(Dt_D, Dt * t_con_ruido);
                    double error = ECM(vec_imagen_discreta, v.first);
                    salida.open("Discretización:"+to_string(discretizaciones[ind_disc])+" espaciado:"+to_string(espacios_entre_censores[ind_espac])+" .txt", ios::app);
                    salida << error << ",\t";
                    salida.close();
                }
                salida.open("Discretización:"+to_string(discretizaciones[ind_disc])+" espaciado:"+to_string(espacios_entre_censores[ind_espac])+" .txt", ios::app);
                salida << endl;
                salida << endl; //Lo hago 2 veces para mejor visibilidad.
                salida.close();
            }
        }
    }
    return;
}

void listarDirectorio(const string& directorio,  vector<string>& v)
{
    string nomArch;
    DIR* dirp = opendir(directorio.c_str());
    struct dirent * dp;
    if (dirp == NULL) {
        throw runtime_error("no se encontro directorio " + directorio + "!");
    }
    while ((dp = readdir(dirp)) != NULL) {
        string nomArch = dp->d_name;
        if (nomArch.compare(".") != 0 && nomArch.compare("..") != 0)
            v.push_back(directorio + "/" + nomArch);
    }
    closedir(dirp);
}

