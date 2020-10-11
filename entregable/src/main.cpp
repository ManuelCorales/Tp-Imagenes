#include <iostream>
#include <fstream>
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
	string filtro(argv[1]);
	string nThreads(argv[2]);
	string parametro1(argv[3]);
	string parametro2(argv[4]);
	string img1Path(argv[5]);
	ppm img2(0, 0);
	
	if(argc > 6){
		string img2Path(argv[6]);
		ppm imagenAux(img2Path);
		img2 = imagenAux;
		
	}
	ppm img1(img1Path);

	struct timespec start, stop;
    double accum;
	clock_gettime(CLOCK_REALTIME, &start);

	bool filtradoTerminado = false;
	if(filtro == "shades"){
		repartirImagenThreads(stoi(nThreads), img1, img2, shades, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "merge"){
		repartirImagenThreads(stoi(nThreads), img1, img2, merge, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "brightness"){
		repartirImagenThreads(stoi(nThreads), img1, img2, brightness, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "calido"){
		repartirImagenThreads(stoi(nThreads), img1, img2, calido, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "frame"){
		repartirImagenThreads(stoi(nThreads), img1, img2, frame, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "edgeDetection"){
		repartirImagenThreadsConvolucion(stoi(nThreads), img1, img2, edgeDetection, stof(parametro1), stof(parametro2));
		filtradoTerminado = true;
	}
	if(filtro == "zoom"){
		zoom(img1, stoi(parametro1));
		filtradoTerminado = true;
	}

	clock_gettime(CLOCK_REALTIME, &stop);
	accum = ( stop.tv_sec - start.tv_sec )
	        + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;

	if(filtradoTerminado){
		cout << "Termine de loopear" << endl;
		printf( "Duración del filtrado: %lf s\n", accum);
		cout << "Escribiendo imagen" << endl;

		string pathDump = "./dumpsTiempos/tp.csv";
		ofstream archivoDeSalida(pathDump.c_str(), ios_base::app);
		if (archivoDeSalida.is_open())
		{
			cout << "Dumpeando información" << endl;
			// Formato: Tiempo, Número de threads, filtro y cantidad de pixeles
			string dump = to_string(accum) + "," + nThreads + "," + filtro + "," + to_string(img1.height * img1.width) + "\n";
			archivoDeSalida << dump;
			archivoDeSalida.close();
		}

		string out = "./imagenOut.ppm";
		img1.write(out);  

		cout << "Listo" << endl;
	} else {
		cout << "No se conoce ese filtro" << endl;
	}
	return 0;
}