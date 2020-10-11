#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <time.h>
#include "filters.h"
#include <thread>
#include <fstream>
#include <dirent.h>

#define ONE_OVER_BILLION 1E-9

using namespace std;
// Un esqueleto como base, modificar a gusto pero respetando la consigna

int main(int argc , char* argv[]){

    cout << "Arranco el programa" << endl;
	// Pasamos 0 en caso de no utilizar p2 o  "" en img2
	if(string(argv[1]) == "-help")
	{
		cout << "Uso: ./loader <filtro> <nthreads> <root_dir>" << endl;
		return 0; 
	} else if (argc != 4){
        cout << "Cantidad inadecuada de flags" << endl;
		return 0;
    }
    string filtro(argv[1]);
    int nThreads = atoi(argv[2]);
    string directorio(argv[3]);
    ppm imagen2(0, 0);
	vector<ppm> imagenes;
	vector<string> imagenesNombre;
	vector<thread> vectorThreads;
	bool filtradoTerminado = false;
    struct timespec start, stop;
    double accum;
	clock_gettime(CLOCK_REALTIME, &start);

    char path[directorio.length()]; 
    for (int i = 0; i < sizeof(path); i++) { 
        path[i] = directorio[i];
    }
    struct dirent *entry = nullptr;
    DIR *dp = nullptr;

    dp = opendir(path);
	int i = 0;
	int superficieTotal, superficiePromedioPorThread;
	ppm imagenLoop;
    if (dp != nullptr) {
        while ((entry = readdir(dp))){
			if (i > 1 && entry->d_name != "")
			{
				cout << entry->d_name << endl;
				imagenLoop = ppm(directorio + "/" + string(entry->d_name));
				superficieTotal += imagenLoop.width * imagenLoop.height; 
				imagenes.push_back(imagenLoop);
				imagenesNombre.push_back(string(entry->d_name));
			}
			i++;
		}
    }
    closedir(dp);
	superficiePromedioPorThread = superficieTotal / nThreads;
	cout << "Superficie total: " << superficieTotal << " Superficie promedio por threads: " << superficiePromedioPorThread << endl;
	vector<ppm> auxImagenes;
	vector<string> auxImagenesNombres;
	int superficieAuxiliar;
	int indiceImagen = 0;
	for(int i = 0; i < nThreads; i++){
		for(int x = indiceImagen; x < imagenes.size(); x++){
			superficieAuxiliar += imagenes[x].width * imagenes[x].height;
			auxImagenes.push_back(imagenes[x]);
			auxImagenesNombres.push_back(imagenesNombre[x]);
			indiceImagen++;
			if(superficieAuxiliar > superficiePromedioPorThread){
				break;
			}
		}
		if(nThreads > 1 && i != nThreads - 1){
			if(filtro == "shades"){
				vectorThreads.push_back(thread(repartirImagenesPixelToPixelLoader, auxImagenes, auxImagenesNombres, shades, 30, 30));
				filtradoTerminado = true;
			}
			if(filtro == "brightness"){
				vectorThreads.push_back(thread(repartirImagenesPixelToPixelLoader, auxImagenes, auxImagenesNombres, brightness, 0.5, 0.5));
				filtradoTerminado = true;
			}
			if(filtro == "frame"){
				vectorThreads.push_back(thread(repartirImagenesPixelToPixelLoader, auxImagenes, auxImagenesNombres, frame, 30, 30));
				filtradoTerminado = true;
			}
			if(filtro == "edgeDetection"){
				vectorThreads.push_back(thread(repartirImagenesConvolucionLoader, auxImagenes, auxImagenesNombres, edgeDetection, 1, 1));
				filtradoTerminado = true;
			}
		} else {
			if(filtro == "shades"){
				repartirImagenesPixelToPixelLoader(auxImagenes, auxImagenesNombres, shades, 30, 30);
				filtradoTerminado = true;
			}
			if(filtro == "brightness"){
				repartirImagenesPixelToPixelLoader(auxImagenes, auxImagenesNombres, brightness, 0.5, 0.5);
				filtradoTerminado = true;
			}
			if(filtro == "frame"){
				repartirImagenesPixelToPixelLoader(auxImagenes, auxImagenesNombres, frame, 30, 30);
				filtradoTerminado = true;
			}
			if(filtro == "edgeDetection"){
				repartirImagenesConvolucionLoader(auxImagenes, auxImagenesNombres, edgeDetection, 1, 1);
				filtradoTerminado = true;
			}
		}
		superficieAuxiliar = 0;
		auxImagenes = {};
		auxImagenesNombres = {};
	}
	if(nThreads > 1){
		for(int i = 0; i < nThreads - 1; i++){
			cout << "Esperando al thread: " << i << endl;
			vectorThreads[i].join();
		}
	}

	if(filtradoTerminado){
		cout << "Termine de loopear" << endl;
		clock_gettime(CLOCK_REALTIME, &stop);
		accum = ( stop.tv_sec - start.tv_sec ) + ( stop.tv_nsec - start.tv_nsec ) * ONE_OVER_BILLION;
		printf( "Duración del filtrado: %lf s\n", accum);

		string pathDump = "./dumpsTiempos/loader.csv";
		ofstream archivoDeSalida(pathDump.c_str(), ios_base::app);
		if (archivoDeSalida.is_open())
		{
			cout << "Dumpeando información" << endl;
			// Formato: Tiempo, Número de threads, filtro y cantidad de pixeles
			string dump = to_string(accum) + "," + to_string(nThreads) + "," + filtro + "," + to_string(superficieTotal) + "\n";
			archivoDeSalida << dump;
			archivoDeSalida.close();
		}

		cout << "Listo" << endl;
	} else {
		cout << "No se conoce ese filtro" << endl;
	}
}