#include <stdio.h>    
#include <stdlib.h>
#include "libraries/colors.h"

//esta función carga las configuraciones PRINCIPALES
int assetsLoad(char petName[]){
    char ruta[] = "../files/assets.txt"; //se vuelve una carpeta para atras para que el .exe pueda abrir el archivo
	// el archivo assets contiene las configuraciones PRONCIPALES
    FILE *fileAssets = fopen(ruta, "r");
    if(fileAssets == NULL){
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }
    
    return 0;
}

int main(int argc, char *argv[]){
    char petName[] = "Mojojo";
    char userName[] = "LoKk";
    int gameAvatar = 1;
    int petInmortality = 1;
    int gameDifficult = 0;

    assetsLoad(petName);
    system("pause");
    return 0;
}
