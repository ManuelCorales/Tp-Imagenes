#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

pixel brightness(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel shades(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel merge(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel frame(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
ppm zoom(ppm& imagen, int n1);
pixel edgeDetection(pixel pixel1, vector<int> posicionPixel, vector<pixel> matrizDePixeles, vector<unsigned int> dimensiones1, float n1, float n2);
void recorrerPixeles(ppm *imagen1, ppm *imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2, vector<unsigned int> intervaloColumnas);
void recorrerPixelesConvolucion(ppm *imagen1, ppm *imagen2, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float parametro1, float parametro2, vector<unsigned int> intervaloColumnas);

// MULTI-THREAD FILTERS
void repartirImagenThreads(int nThreads, ppm& imagen1, ppm& imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2);
void repartirImagenThreadsConvolucion(int nThreads, ppm& imagen1, ppm& imagen2, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float parametro1, float parametro2);
void repartirImagenesPixelToPixelLoader(vector<ppm> imagenes, vector<string> nombreImagenes, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float n1, float n2);
void repartirImagenesConvolucionLoader(vector<ppm> imagenes, vector<string> nombreImagenes, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float n1, float n2);

#endif