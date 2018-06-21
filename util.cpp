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
        ret->push_back(convertirAVectorDeValores(explode(lectura, ' ')));
    }
    entrada.close();
    return ret;
}

vector<vector<double>> discretizar(vector<vector<double>> mat, uint val){//supongo que la matriz mat es cuadrada
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

vector<double> pasarAVector(vector<vector<double>> mat){
	vector<double> res (mat.size()*mat[0].size(),0);
	for(uint i = 0; i< mat.size(); i++){
		for(uint j = 0; j < mat[0].size(); j++){
			res[i*mat[0].size()+j] = mat[i][j];
		}
	}
	return res;

}
