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

ppm recorrerPixeles(ppm& imagen1, ppm* imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2){
    vector <unsigned int> dimesiones1 = {imagen1.width, imagen1.height};
    pixel pixel1, pixel2;
    cout << "Entré 1" << endl;
    cout << "Entré 2" << endl;
    if(imagen2 != NULL){
        vector <unsigned int> dimesiones2 = {(*imagen2).width, (*imagen2).height};
        cout << dimesiones2[0] << " " << dimesiones2[1] << endl;
    }
    cout << "La imagen 2 es: " << imagen2 << endl;
    cout << dimesiones1[0] << " " << dimesiones1[1] << endl;
    vector<int> posicionPixel = {0, 0};
    for(int j = 0; j < dimesiones1[0]; j++){
        for(int i = 0; i < dimesiones1[1]; i++){
            pixel pixel1 = imagen1.getPixel(i, j);
            posicionPixel[0] = i;
            posicionPixel[1] = j;
            // cout << "Sali3 " << i << " " << j << endl;
            // if(imagen2 != NULL){
            //     cout << "Sali4 " << endl;
            //     pixel pixel2 = imagen2 -> getPixel(i, j);
            //     cout << "Sali5 " << endl;
            // }
            // cout << i << " " << j << endl;
            pixel pixel = (*f)(pixel1, pixel2, posicionPixel, dimesiones1, parametro1, parametro2);
            // cout << "Sali" << endl;
            imagen1.setPixel(i, j, pixel);
            // cout << "Sali2" << endl;
        }
    }
    return imagen1;
}

pixel shades(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    int grisDelPixel = (pixel1.r + pixel1.g + pixel1.b)/3;
    for(int i = 0; i < n1; i ++){ 
        if((i * (256/n1)) > grisDelPixel){            
            grisDelPixel =  (i - 1) * (256/n1);
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


pixel merge(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    float porcentaje = n1 / 100;
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
pixel brightness(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    pixel1.r = pixel1.r + (255 * n1);
    pixel1.g = pixel1.g + (255 * n1);
    pixel1.b = pixel1.b + (255 * n1);
    pixel1.truncate();
    return pixel1;
}

// //Llama a ppm y le pasa el filtro edgeDetection
// void edgeDetection(){

// }

// //Llama a ppm y le pasa el filtro zoom
// void zoom(){

// }

// Color de gris n1 = [0, 255]
// Tamaño de marco (pixeles) n2
pixel frame(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    if((posicionPixel[0] < n2 || (dimensiones1[1] - n2) < posicionPixel[0]) || posicionPixel[1] < n2 || (dimensiones1[0] - n2) < posicionPixel[1]){
        pixel1.r = n1;
        pixel1.g = n1;
        pixel1.b = n1;
    }
    return pixel1;
}