#include <stdio.h>    
#include <stdlib.h>
#include "libraries/colors.h"

int assetsLoad(){
    char ruta[] = "../files/assets.txt"; //se vuelve una carpeta para atras para que el .exe pueda abrir el archivo
    FILE *fileAssets = fopen(ruta, "r");
    if(fileAssets == NULL){
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }
    
    return 0;
}

int main(){
    char petName[] = "Mojojo";
    char userName[] = "LoKk";
    int gameAvatar = 1;
    int petInmortality = 1;
    int gameDifficult = 0;

    assetsLoad();
    system("pause");
    return 0;
}