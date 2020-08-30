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

ppm recorrerPixeles(ppm& imagen1, ppm* imagen2, pixel(*f)(pixel, pixel, float), float parametro){
    vector <unsigned int> dimesiones1 = {imagen1.width, imagen1.height};
    dimesiones1.push_back(imagen1.height);
    if(imagen2 == NULL){ //preguntar profe
        vector <unsigned int> dimesiones2 = {(*imagen2).width, (*imagen2).height};
    }
    for(int i = 0; i < dimesiones1[0]; i++){
        for(int j = 0; j < dimesiones1[1]; j++){
            pixel pixel1 = imagen1.getPixel(i, j);
            pixel pixel2 = (*imagen2).getPixel(i, j);
            pixel pixel = (*f)(pixel1, pixel2, parametro);
            imagen1.setPixel(i, j, pixel);
        }
    }
    return imagen1;
}

pixel shades(pixel pixel1, pixel pixel2, float n){
    int grisDelPixel = (pixel1.r + pixel1.g + pixel1.b)/3;
    for(int i = 0; i < n; i ++){
        if(i * 256/n > grisDelPixel){
            if(grisDelPixel >( i-1 * 256/n + i * 256/n) / 2){
                grisDelPixel =  i - 1 * 256/n;
            } else {
                grisDelPixel =  i - 1 * 256/n;
            }
        }
    }
    return pixel(grisDelPixel, grisDelPixel, grisDelPixel);
}



// //Llama a ppm y le pasa el filtro merge
pixel merge(pixel pixel1, pixel pixel2, float porcentaje){
    vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
    vector <int> vectorPixel2 = {pixel2.r, pixel2.g, pixel2.b};
    vector <int> mergedPixel;
    for(int i = 0; i < 3; i++){
        mergedPixel.push_back(vectorPixel1[i] * (porcentaje) + vectorPixel2[i] * (1 - porcentaje));
    }
    return pixel(mergedPixel[0], mergedPixel[1], mergedPixel[2]);
}

// -1 negro. 0 igual.1 todo blanco

//Llama a ppm y le pasa el filtro brightness
// pixel brightness(){

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