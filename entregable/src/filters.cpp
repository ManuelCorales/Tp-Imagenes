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

ppm repartirImagenThreads(int nThreads, ppm& imagen1, ppm& imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2){
    cout << "Entré" << endl;
    vector<thread> vectorThreads;
    vector<unsigned int> intervaloColumnas;
    for(int i = 1; i < nThreads + 1 ; i++){
        unsigned int resultado = imagen1.width / nThreads;
        intervaloColumnas = {resultado * (i - 1), resultado * i};
        if(i == nThreads){
            intervaloColumnas = {resultado * (i - 1), imagen1.width};
        }
        vectorThreads.push_back(thread(recorrerPixeles, &imagen1, &imagen2, (*f), parametro1, parametro2, intervaloColumnas));
    }
    cout << "tamaño vector: " << vectorThreads.size() << endl;
    for(int i = 0; i != nThreads; i++){
        vectorThreads[i].join();
    }
    return imagen1;
}

ppm recorrerPixeles(ppm *imagen1, ppm *imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2, vector<unsigned int> intervaloColumnas){
    vector <unsigned int> dimesiones1 = {imagen1->width, imagen1->height};
    pixel pixel1, pixel2;
    if(&imagen2 != NULL){
        vector <unsigned int> dimesiones2 = {imagen2->width, imagen2->height};
    }
    vector<int> posicionPixel = {0, 0};
    for(int i = 0; i < dimesiones1[1]; i++){
        for(int j = intervaloColumnas[0]; j < intervaloColumnas[1]; j++){
            // cout << i << " " << j << endl;
            posicionPixel = {i, j};
            pixel pixel1 = imagen1->getPixel(i, j);
            pixel pixel2 = imagen2->getPixel(i, j);
            pixel pixel = (*f)(pixel1, pixel2, posicionPixel, dimesiones1, parametro1, parametro2);
            imagen1->setPixel(i, j, pixel);
        }
    }
    return *imagen1;
}

ppm recorrerPixelesConvolucion(ppm& imagen1, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float parametro1, float parametro2){
    vector <unsigned int> dimesiones1 = {imagen1.width, imagen1.height};
    pixel pixelSeleccionado;
    vector<pixel> matrizDePixeles = {pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel()};
    vector<int> posicionPixel = {0, 0};
    for(int i = 1; i < imagen1.height - 1; i++){
        for(int j = 1; j < imagen1.width - 1; j++){
            pixel pixelSeleccionado = imagen1.getPixel(i, j);
            posicionPixel = {i, j};
            for(int z = -1; z < 8; z++){
                if(z < 2){
                    // cout << i - 1 << " " << j + z << endl;
                    matrizDePixeles[z] = imagen1.getPixel(i - 1, j + z);
                } else if(z < 5){
                    // cout << i  << " " << j + z - 3 << endl;
                    matrizDePixeles[z] = imagen1.getPixel(i, j + z - 3);
                } else {
                    // cout << i + 1 << " " << j + z - 6 << endl;
                    matrizDePixeles[z] = imagen1.getPixel(i + 1, j + z - 6);
                }
            }
            pixel pixelDevuelto = (*f)(pixelSeleccionado, posicionPixel, matrizDePixeles, dimesiones1, parametro1, parametro2);
            imagen1.setPixel(i, j, pixelDevuelto);
            // cout << i << " " << j << pixel.r << pixel.g << pixel.b << endl;
        }
    }
    cout << "Devolviendo" << endl;
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
    float porcentaje = n1;
    vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
    vector <int> vectorPixel2 = {pixel2.r, pixel2.g, pixel2.b};
    // cout << "Vector pixel 1 " << vectorPixel1[0] << " " << vectorPixel1[1] << " " << vectorPixel1[2] << " " << endl;
    // cout << "Vector pixel 2 " << vectorPixel2[0] << " " << vectorPixel2[1] << " " << vectorPixel2[2] << " " << endl;
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
pixel edgeDetection(pixel pixel1, vector<int> posicionPixel, vector<pixel> matrizDePixeles, vector<unsigned int> dimensiones1, float n1, float n2){
    vector<int> filtroValoresHorizontales = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    vector<int> filtroValoresVerticales = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
    pixel pixelFinal(0, 0, 0);
    int rH, gH, bH, rV, gV, bV;
    int grisDePixel, valorPixel, sumaPixelTotal;
    for(int i = 0; i < matrizDePixeles.size(); i++){
        rH += matrizDePixeles[i].r * filtroValoresHorizontales[i];
        gH += matrizDePixeles[i].g * filtroValoresHorizontales[i];
        bH += matrizDePixeles[i].b * filtroValoresHorizontales[i];

        rV += matrizDePixeles[i].r * filtroValoresVerticales[i];
        gV += matrizDePixeles[i].g * filtroValoresVerticales[i];
        bV += matrizDePixeles[i].b * filtroValoresVerticales[i];
    }
    // cout << "r: " << rH << " " << rV << endl;
    // cout << "g: " << gH << " " << gV << endl;
    // cout << "b: " << bH << " " << bV << endl;
    // cout << "rgb: " << r << " " << g << " " << b << endl;
    pixelFinal.r = (sqrt((rH * rH) + (rV * rV)));
    pixelFinal.g = (sqrt((gH * gH) + (gV * gV)));
    pixelFinal.b = (sqrt((bH * bH) + (bV * bV)));
    pixelFinal.truncate();
    // cout << "pixel final rgb: " << pixelFinal.r << " " << pixelFinal.g << " " << pixelFinal.b << endl; 
    return pixelFinal;
}

//Llama a ppm y le pasa el filtro zoom
ppm zoom(ppm& imagen, int n1){
    ppm img_zoomeada(imagen.width * n1, imagen.height * n1);
    vector <unsigned int> dimesiones1 = {imagen.width, imagen.height};
    cout << dimesiones1[0] << " " << dimesiones1[1] << endl;
    pixel pixel;
    unsigned int traslado_I, traslado_J;
    vector<int> posicionPixel = {0, 0};
    for(int i = 0; i < dimesiones1[1]; i++){
        for(int j = 0; j < dimesiones1[0]; j++){
            pixel = imagen.getPixel(i, j);
            traslado_I = (i + (i * (n1 - 1)));
            traslado_J = (j + (j * (n1 - 1)));
            for(int z = 0; z < n1; z++){
                for(int x = 0; x < n1; x++){
                    img_zoomeada.setPixel(traslado_I + z, traslado_J + x, pixel);
                }
            }
        }
    }
    imagen = img_zoomeada;
    return imagen;
}

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