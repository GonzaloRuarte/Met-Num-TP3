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
    } else { //Estamos en la primera fila, y no estamos en la primera columna, entonces movemos a izquierda.
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
    } else { //Estamos en la primera fila, y no estamos en la primera columna, entonces movemos a izquierda.
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

/**
 * Carga las posiciones donde se colocarían laseres a lo alto (en el borde izquierdo y derecho) para una imagen de nxn.
 * esto indica la posicion de inicio de los laseres, su punto al que apuntan será manejado con otra función.
 * @param n: Tamaño de la matriz imágen donde se colocaran los laseres.
 * @param cada_cuanto: Cada cuantos pixeles se añadirá un nuevo laser, el primero se añade sin dejar espacio (si offset
 * es 0).
 * @param offset: espacio dejado antes del primer laser (se empiezan a colocar de arriba a abajo, solo en los bordes
 * izquierdo y derecho)
 * @param cant_maxima: parámetro extra para evitar que el método cree mas laseres que cant_maxima (por cada lado, o sea
 * se crean cant_maxima de laseres por el lado izquierdo, y cant_maxima por el lado derecho). Si cant_maxima es igual
 * a 0, se ignora el valor y se coloca tantos como sean posibles.
 * @return Devuelve los puntos donde inician los laseres para una matriz de nxn.
 */

vector<pair<uint,uint> > crearLaseres(size_t n, size_t cada_cuanto, size_t offset, size_t cant_maxima){
    vector<pair<uint,uint> > result;
    uint cant_creada = 0;
    for(uint i = offset; i<n and (cant_maxima == 0 or cant_creada < cant_maxima); i += cada_cuanto) {
        result.emplace_back(make_pair(i,uint(0)));
    }

    cant_creada = 0;

    for(uint i = offset; i<n and (cant_maxima == 0 or cant_creada < cant_maxima); i += cada_cuanto) {
        result.emplace_back(i,uint(n-1));
    }

    return result;
}


/**
 * Dado un vector con los laseres de inicio, crea los n puntos a donde empezarían a apuntar para dar una sola pasada.
 * @param Laseres: resultado dado por la función crearLaseres.
 * @param n: tamaño de la imágen.
 * @return vector con los puntos a los que apuntan los laseres, ordenados de la misma forma que se encuentran los
 * Laseres en el vector de Laseres.
 */

vector<pair<uint,uint> > crearPuntosDeFin(vector<pair<uint,uint> > Laseres, size_t n) {
    vector<pair<uint,uint> > result;
    result.reserve(Laseres.size()*2);
    for(int i = 0; i<Laseres.size(); i++) {
        uint fila;
        uint columna;
        if(Laseres[i].second == 0){
            fila = uint(0);
            columna = uint(1);
        } else {
            fila = uint(0);
            columna = uint(n-2);
        }
        pair<uint,uint> p = make_pair(fila,columna);
        result.emplace_back(p);
    }
    return result;
}


/**
 * Función que rota el punto donde termina el laser creado en crearPuntosDeFin. A los laseres que hayan empezado del
 * lado derecho, los rota contrarreloj (empezando de la esquina arriba a la derecha), y los laseres del lado izquierdo
 * los rota en dirección de las agujas del reloj, en el unico caso en el que los laseres quedan horizontales, para no
 * repetir laseres, decidimos que los laseres de la izquierda roten un paso más, y asi saltearse el valor horizontal,
 * ya que los laseres de la derecha ya lo representan, para que no tengamos 2 veces laseres horizontales.
 * @param Laseres: vector de puntos creado con la función crearLaseres.
 * @param A_donde_apuntan: vector de puntos creado con la función crearPuntosDeFin, este lo toma por referencia y los
 * rota acorde a lo mencionado mas arriba.
 */

void rotarLaseres(vector<pair<uint,uint> > Laseres, vector<pair<uint,uint> >& A_donde_apuntan, size_t n) {
    for(int i = 0; i<Laseres.size(); i++) {
        if(Laseres[i].second == 0){ //roto en direccion del reloj.
            if(A_donde_apuntan[i].second == 0 and A_donde_apuntan[i].first != 0) { //Si la columna es 0, entonces si la fila no es la primera lo movemos para
                // arriba.
                A_donde_apuntan[i].first--;
            } else if(A_donde_apuntan[i].first == n-1 and A_donde_apuntan[i].second != 0) { //Estamos en última fila, y no estamos en primera columna,
                // movemos a izquierda.
                A_donde_apuntan[i].second--;
            } else if(A_donde_apuntan[i].second == n-1 and A_donde_apuntan[i].first != n-1) { //Última columna, y no en la ultima fila, movemos para abajo.
                A_donde_apuntan[i].first++;
                if (A_donde_apuntan[i].first == Laseres[i].first and A_donde_apuntan[i].first != n-1) { //Si el laser de la izquierda queda horizontal, lo muevo hacia abajo, excepto si el laser es el ultimo mas abajo.
                    A_donde_apuntan[i].first++;
                } else if (A_donde_apuntan[i].first == Laseres[i].first and A_donde_apuntan[i].first == n-1) { // Si es el ultimo mas abajo, lo muevo hacia la izquierda (porque sino se va de rango).
                    A_donde_apuntan[i].second--;
                }
            } else { //Estamos en la primera fila, y no estamos en la última columna, entonces movemos a derecha.
                A_donde_apuntan[i].second++;
                if(Laseres[i].first == 0 and A_donde_apuntan[i].second == n-1) { // Si es el primer laser y cae en la ultima columna, entonces lo debemos mover hacia abajo ya que no lo queremos horizontal.
                    A_donde_apuntan[i].first++;
                }
            }
        } else { //rota contrarreloj.
            if(A_donde_apuntan[i].second == 0 and A_donde_apuntan[i].first != n-1) { //Si la columna es 0, entonces si la fila no es la última lo movemos para
                // abajo.
                A_donde_apuntan[i].first++;
            } else if(A_donde_apuntan[i].first == n-1 and A_donde_apuntan[i].second != n-1) { //Estamos en última fila, y no estamos en última columna,
                // movemos a derecha.
                A_donde_apuntan[i].second++;
            } else if(A_donde_apuntan[i].second == n-1 and A_donde_apuntan[i].first != 0) { //Última columna, y no en la primera fila, movemos para arriba.
                A_donde_apuntan[i].first--;
            } else { //Estamos en la primera fila, y no estamos en la primera columna, entonces movemos a izquierda.
                A_donde_apuntan[i].second--;
            }
        }
    }
    return;
}