#include "calcular_rayos.h"
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
        int pintar_desde = floor(a*i + b);
        int pintar_hasta = floor(a*(i+1) +b);
        if (pintar_hasta < pintar_desde) {
            int aux = pintar_desde; //si estan al revez los swappeo
            pintar_desde = pintar_hasta;
            pintar_hasta = aux;
        }
        for(int j = pintar_desde; j <= pintar_hasta ; j++) {
            result[j][i] = 1;
        }
    }
    int pintar_desde = floor(a*fin + b);
    int pintar_hasta = floor(a*(fin+1) +b);
    if(pintar_hasta >= int(n)) {
        pintar_hasta = n-1;
    } else if (pintar_hasta < 0) {
        pintar_hasta = 0;
    }
    if (pintar_hasta < pintar_desde) {
        int aux = pintar_desde; //si estan al revez los swappeo
        pintar_desde = pintar_hasta;
        pintar_hasta = aux;
    }

    for(int j = pintar_desde; j <= pintar_hasta ; j++) {
        result[j][fin] = 1;
    }
    return result;
}
