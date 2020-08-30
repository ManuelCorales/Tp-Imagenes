#include <iostream>
#include <string>
#include <stdlib.h>
#include <math.h>       /* sqrt */
#include <vector>
#include "filters.h"
#include <thread>  
#include <atomic>  

#define BLACK 0

using namespace std;

// COMPLETAR :)

// PixelToPixel: Shades, Brightness, Merge  | Convolution:  Edge Detection | Otros:  Frame, Zoom

//Llama a ppm y le pasa el filtro shades


void shades(ppm imagen){
    grisDelPixel = (pixel.r + pixel.g + pixel.b)/3
    for(int i = 0; i < n; i ++){
        if(i * 256/n > grisDePixel){
            if(grisDePixel >( i-1 * 256/n + i * 256/n) / 2){
                grisDePixel =  i-1 * 256/n
            } else {
                grisDePixel =  i-1 * 256/n
            }
        }
    }
    setPixel(i, j, grisDePixel);
}

ppm recorrerPixeles(ppm imagen1, ppm imagen2, funcionFiltro){
    dimesiones1 = [imagen1.width, imagen1.width];
    if(imagen2){
        dimesiones2 = [imagen2.width, imagen2.width];
    }
    for(int i = 0; i < dimesiones1[0]; i++){
        for(int j = 0; j < dimesiones1[0]; j++){
            pixelesAModificar = [imagen1.getPixel(i, j), imagen2.getPixel(i, j)];
            pixel = funcionFiltro(pixelesAModificar);
            imagen1.setPixel(i, j, pixel);
        }
    }
    return imagen1
}

// //Llama a ppm y le pasa el filtro merge
void merge(ppm imagen1, ppm imagen2, porcentaje){
    tuplaPixel1 = pixel1.r, pixel1.g, pixel1.b;
    tuplaPixel2 = pixel2.r, pixel2.g, pixel2.b;
    mergedPixel
    for(int i = 0; i < 3; i++){
        mergedPixel[i] = tuplaPixel1[i] * (porcentaje) + tuplaPixel2[i] * (1 - porcentaje);
    }
    
}

// -1 negro. 0 igual.1 todo blanco
// //Llama a ppm y le pasa el filtro brightness
// void brightness(){

// }

// //Llama a ppm y le pasa el filtro edgeDetection
// void edgeDetection(){

// }

// //Llama a ppm y le pasa el filtro zoom
// void zoom(){

// }

// //Llama a ppm y le pasa el filtro frame
// void frame(){

// }