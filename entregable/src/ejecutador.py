import os

for n_threads in range(1,26):
    for i in range(1,11): 
        cmd = "./main edgeDetection " + str(n_threads) + " 1 1 imgs/imagen_grande.ppm imgs/imagen_grande.ppm"
        os.system(cmd)