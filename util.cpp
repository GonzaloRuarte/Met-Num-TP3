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


vector<short int> convertirAVectorDeValores(vector<string> lecturas) {
    vector<short int>* ret = new vector<short int>(0);
    for (vector<string>::iterator it = lecturas.begin() ; it != lecturas.end(); ++it) {
        string lectura = *it;
        ret->push_back(stoi(lectura));
    }
    return *ret;
}

vector<vector<short int>>* leerCSV(string nombreArchivo) {
    fstream entrada(nombreArchivo, ios_base::in);

    vector<vector<short int>>* ret = new vector<vector<short int>>(0);

    string lectura;
    vector<string> lecturas;
    bool path = true;
    while(entrada >> lectura) {
        ret->push_back(convertirAVectorDeValores(explode(lectura, ' ')));
    }
    entrada.close();
    return ret;
}