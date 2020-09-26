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

void repartirImagenesPixelToPixelLoader(vector<ppm> imagenes, vector<string> nombreImagenes, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float n1, float n2){
    for(int i = 0; i < nombreImagenes.size(); i++){
        ppm imagenAux(0, 0);
        recorrerPixeles(&imagenes[i], &imagenAux, (*f), n1, n2, {0, imagenes[i].width});
        cout << "Por escribir imagen " << nombreImagenes[i] << endl;
        imagenes[i].write("./out/" + nombreImagenes[i]);
    }
}

void repartirImagenesConvolucionLoader(vector<ppm> imagenes, vector<string> nombreImagenes, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float n1, float n2){
    for(int i = 0; i < nombreImagenes.size(); i++){
        ppm imagenAux(0, 0);
        recorrerPixelesConvolucion(&imagenes[i], &imagenAux, (*f), n1, n2, {0, imagenes[i].width});
        cout << "Por escribir imagen " << nombreImagenes[i] << endl;
        imagenes[i].write("./out/" + nombreImagenes[i]);
    }
}

void repartirImagenThreads(int nThreads, ppm& imagen1, ppm& imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2){
    vector<unsigned int> intervaloColumnas;
    if(nThreads != 1){
        vector<thread> vectorThreads;
        unsigned int resultado = imagen1.width / nThreads;
        for(int i = 1; i < nThreads + 1; i++){
            intervaloColumnas = {resultado * (i - 1), resultado * i};
            if(i == nThreads){
                intervaloColumnas = {resultado * (i - 1), imagen1.width};
            }
            vectorThreads.push_back(thread(recorrerPixeles, &imagen1, &imagen2, (*f), parametro1, parametro2, intervaloColumnas));
        }
        for(int i = 0; i < nThreads; i++){
            vectorThreads[i].join();
        }
    } else {
        intervaloColumnas = {0, imagen1.width};
        recorrerPixeles(&imagen1, &imagen2, (*f), parametro1, parametro2, intervaloColumnas);
    }
}

void repartirImagenThreadsConvolucion(int nThreads, ppm& imagen1, ppm& imagen2, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float parametro1, float parametro2){
    vector<unsigned int> intervaloColumnas;
    if(nThreads != 1){
        vector<thread> vectorThreads;
        unsigned int resultado = imagen1.width / nThreads;
        for(int i = 1; i < nThreads + 1; i++){
            intervaloColumnas = {resultado * (i - 1), resultado * i};
            if(i == nThreads){
                intervaloColumnas = {resultado * (i - 1), imagen1.width};
            }
            vectorThreads.push_back(thread(recorrerPixelesConvolucion, &imagen1, &imagen2, (*f), parametro1, parametro2, intervaloColumnas));
        }
        for(int i = 0; i < nThreads; i++){
            vectorThreads[i].join();
        }
    } else {
        intervaloColumnas = {0, imagen1.width};
        recorrerPixelesConvolucion(&imagen1, &imagen2, (*f), parametro1, parametro2, intervaloColumnas);
    }
}

void recorrerPixeles(ppm *imagen1, ppm *imagen2, pixel(*f)(pixel, pixel, vector<int>, vector<unsigned int>, float, float), float parametro1, float parametro2, vector<unsigned int> intervaloColumnas){
    vector <unsigned int> dimesiones1 = {imagen1->width, imagen1->height};
    pixel pixel1, pixel2;
    bool hayImagen2 = false;
    if(&imagen2 != NULL){
        vector <unsigned int> dimesiones2 = {imagen2->width, imagen2->height};
    }
    vector<int> posicionPixel = {0, 0};
    for(int i = 0; i < dimesiones1[1]; i++){
        for(int j = intervaloColumnas[0]; j < intervaloColumnas[1]; j++){
            posicionPixel = {i, j};
            pixel pixel1 = imagen1->getPixel(i, j);
            if((imagen2->width != 0) || (imagen2->height != 0)){
                pixel2 = imagen2->getPixel(i, j);
            }
            pixel pixel = (*f)(pixel1, pixel2, posicionPixel, dimesiones1, parametro1, parametro2);
            imagen1->setPixel(i, j, pixel);
        }
    }
}

void recorrerPixelesConvolucion(ppm *imagen1, ppm *imagen2, pixel(*f)(pixel, vector<int>, vector<pixel>, vector<unsigned int>, float, float), float parametro1, float parametro2, vector<unsigned int> intervaloColumnas){
    vector <unsigned int> dimesiones1 = {imagen1->width, imagen1->height};
    pixel pixelSeleccionado;
    vector<pixel> matrizDePixeles = {pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel(), pixel()};
    vector<int> posicionPixel = {0, 0};
    if(intervaloColumnas[0] != 0){
        intervaloColumnas[0] = intervaloColumnas[0] + 1;
    }
    if(intervaloColumnas[1] != imagen1->width){
        intervaloColumnas[1] = intervaloColumnas[1] - 1;
    }

    for(int i = 1; i < imagen1->height - 1; i++){
        for(int j = intervaloColumnas[0]; j < intervaloColumnas[1]; j++){
            pixel pixelSeleccionado = imagen1->getPixel(i, j);
            posicionPixel = {i, j};
            for(int z = -1; z < 8; z++){
                if(z < 2){
                    matrizDePixeles[z + 1] = imagen1->getPixel(i - 1, j + z);
                } else if(z < 5){
                    matrizDePixeles[z + 1] = imagen1->getPixel(i, j + z - 3);
                } else {
                    matrizDePixeles[z + 1] = imagen1->getPixel(i + 1, j + z - 6);
                }
            }
            pixel pixelDevuelto = (*f)(pixelSeleccionado, posicionPixel, matrizDePixeles, dimesiones1, parametro1, parametro2);
            imagen1->setPixel(i - 1, j - 1, pixelDevuelto);
        }
    }
}

pixel shades(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    int grisDelPixel = (pixel1.r + pixel1.g + pixel1.b)/3;
    for(int i = 0; i < n1; i ++){ 
        if((i * (256 / n1)) > grisDelPixel){            
            grisDelPixel =  (i - 1) * (256/n1);
            break;
        }
    }
    return pixel(grisDelPixel, grisDelPixel, grisDelPixel);
}

pixel merge(pixel pixel1, pixel pixel2, vector<int> posicionPixel, vector<unsigned int> dimensiones1, float n1, float n2){
    float porcentaje = n1;
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
pixel edgeDetection(pixel pixel1, vector<int> posicionPixel, vector<pixel> matrizDePixeles, vector<unsigned int> dimensiones1, float n1, float n2){
    vector<int> filtroValoresVerticales = {1, 0, -1, 2, 0, -2, 1, 0, -1};
    vector<int> filtroValoresHorizontales = {1, 2, 1, 0, 0, 0, -1, -2, -1};
    
    vector <int> vectorPixel1 = {pixel1.r, pixel1.g, pixel1.b};
    pixel pixelFinal(0, 0, 0);
    int rH, gH, bH, rV, gV, bV = 0;
    int grisDePixel, valorPixel, sumaPixelTotal;
    for(int i = 0; i < matrizDePixeles.size(); i++){        
        rV += matrizDePixeles[i].r * filtroValoresVerticales[i];
        gV += matrizDePixeles[i].g * filtroValoresVerticales[i];
        bV += matrizDePixeles[i].b * filtroValoresVerticales[i];

        rH += matrizDePixeles[i].r * filtroValoresHorizontales[i];
        gH += matrizDePixeles[i].g * filtroValoresHorizontales[i];
        bH += matrizDePixeles[i].b * filtroValoresHorizontales[i];
    }
    pixelFinal.r = (sqrt((rH * rH) + (rV * rV)));
    pixelFinal.g = (sqrt((gH * gH) + (gV * gV)));
    pixelFinal.b = (sqrt((bH * bH) + (bV * bV)));
    pixelFinal.truncate();
    
    return pixelFinal;
}

//Llama a ppm y le pasa el filtro zoom
ppm zoom(ppm& imagen, int n1){
    ppm img_zoomeada(imagen.width * n1, imagen.height * n1);
    vector <unsigned int> dimesiones1 = {imagen.width, imagen.height};
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