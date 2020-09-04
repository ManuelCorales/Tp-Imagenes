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
    pixel pixel1, pixel2;
    cout << "Entré 1" << endl;
    cout << "Entré 2" << endl;
    if(imagen2 == NULL){
        vector <unsigned int> dimesiones2 = {(*imagen2).width, (*imagen2).height};
    }
    cout << dimesiones1[0] << " " << dimesiones1[1] << endl;
    for(int j = 0; j < dimesiones1[0]; j++){
        for(int i = 0; i < dimesiones1[1]; i++){
            pixel pixel1 = imagen1.getPixel(i, j);
            if(imagen2 != NULL){
                pixel pixel2 = imagen2 -> getPixel(i, j);
            }
            cout << i << " " << j << endl;
            pixel pixel = (*f)(pixel1, pixel2, parametro);
            imagen1.setPixel(i, j, pixel);
            
        }
    }
    return imagen1;
}

pixel shades(pixel pixel1, pixel pixel2, float n){
    int grisDelPixel = (pixel1.r + pixel1.g + pixel1.b)/3;
    for(int i = 0; i < n; i ++){ 
        if((i * (256/n)) > grisDelPixel){            
            grisDelPixel =  (i - 1) * (256/n);
            break;
        }
    }
    return pixel(grisDelPixel, grisDelPixel, grisDelPixel);
}



// //Llama a ppm y le pasa el filtro merge
// pixel shitpostFilter(pixel pixel1, pixel pixel2, float porcentaje){
//     vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
//     vector <int> vectorPixel2 = {pixel2.r, pixel2.g, pixel2.b};
//     vector <int> mergedPixel;
//     for(int i = 0; i < 3; i++){
//         mergedPixel.push_back(vectorPixel1[i] * (porcentaje) + vectorPixel2[i] * (1 - porcentaje));
//     }
//     return pixel(mergedPixel[0], mergedPixel[1], mergedPixel[2]);
// }


pixel merge(pixel pixel1, pixel pixel2, float n){
    float porcentaje = n / 100;
    vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
    vector <int> vectorPixel2 = {pixel2.r, pixel2.g, pixel2.b};
    vector <int> mergedPixel;
    for(int i = 0; i < 3; i++){
        mergedPixel.push_back(vectorPixel1[i] * (porcentaje) + vectorPixel2[i] * (1 - porcentaje));
    }
    return pixel(mergedPixel[0], mergedPixel[1], mergedPixel[2]);
}
//

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