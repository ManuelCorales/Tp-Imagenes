#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#define ONE_OVER_BILLION 1E-9

using namespace std;
// Un esqueleto como base, modificar a gusto pero respetando la consigna

int main(int argc , char* argv[]){
	
	cout << "Arranco el programa" << endl;
	// Pasamos 0 en caso de no utilizar p2 o  "" en img2
	if(string(argv[1]) == "-help")
	{
		cout << "Uso: ./tp <filtro> <nthreads> <p1> <p2> <img1> <img2>" << endl;
		return 0; 
	}
	cout << "pasé" << endl;
	string filtro(argv[1]);
	string nthreads(argv[2]);
	string parametro1(argv[3]);
	string parametro2(argv[4]);
	string img1Path(argv[5]);
	ppm *img2 = NULL;
	cout << argv[6] << endl;
	cout << "pasé 2" << endl;
	if(argv[6] != ""){
		string img2Path(argv[6]);
		ppm imagen2(img2Path);
		img2 = &imagen2;
	}
	cout << "pasé 3" << endl;
	ppm img1(img1Path);

	cout << "Aplicando filtro" << endl;

	struct timespec start, stop;
    double accum;
	clock_gettime(CLOCK_REALTIME, &start);

    cout << "Por loopear imagenes" << endl;

	if(filtro == "shades"){
		recorrerPixeles(img1, img2, shades, stof(parametro1));
	}
	if(filtro == "merge"){
		recorrerPixeles(img1, img2, merge, stof(parametro1));
	}
	// if(filtro == "brightness"){
	// 	recorrerPixeles(img1, img2, ,stof(parametro1));
	// }
    cout << "Termine de loopear" << endl;

	clock_gettime(CLOCK_REALTIME, &stop);
	accum = ( stop.tv_sec - start.tv_sec )
	        + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
	printf( "Duración del filtrado: %lf s\n", accum);

	cout << "Escribiendo imagen" << endl;
	
	string out = "../imagenesOut/imagen1.ppm";
	//out = /c/usuarios/documentos/nombredelarchivo.ppm
	
	img1.write(out);  

	cout << "Listo" << endl;
	return 0;
}