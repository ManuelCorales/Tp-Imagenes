import os

for n_threads in range(1,26):
    for i in range(0,10): 
        cmd = "./main edgeDetection " + str(n_threads) + " 10 30 ../imagenesIn/imagen_grande.ppm ../imagenesIn/imagen_grande.ppm"
        os.system(cmd)