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
pixel brightness(ppm& img, float b, int start, float end);
pixel shades(pixel pixel1, pixel pixel2, float n);
pixel merge(pixel pixel1, pixel pixel2, float porcentaje);
void frame(ppm& img, pixel color, int x);
void boxBlur(ppm &img);
void zoom(ppm &img, ppm &img_zoomed, int n);
void edgeDetection(ppm &img, ppm &img_target);
ppm recorrerPixeles(ppm& imagen1, ppm* imagen2, pixel(*f)(pixel, pixel, float), float parametro);


// MULTI-THREAD FILTERS

#endif