#ifndef FILTERS_H
#define FILTERS_H

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include "ppm.h"
#include <atomic>


// SINGLE-THREAD FILTERS

void blackWhite(ppm& img);
void contrast(ppm& img, float contrast);
pixel brightness(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel shades(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel merge(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
pixel frame(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2);
void boxBlur(ppm &img);
void zoom(ppm &img, ppm &img_zoomed, int n);
void edgeDetection(ppm &img, ppm &img_target);
ppm recorrerPixeles(ppm& imagen1, ppm* imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2);


// MULTI-THREAD FILTERS

#endif