#include <stdio.h>    
#include <stdlib.h>
#include "libraries/colors.h"

//esta funci�n carga las configuraciones PRINCIPALES
int assetsLoad(char petName[]){
    char ruta[] = "../files/assets.txt"; //se vuelve una carpeta para atras para que el .exe pueda abrir el archivo
    char caracter;

	// el archivo assets contiene las configuraciones PRONCIPALES
    FILE* fileAssets = fopen(ruta, "rb");     //se habre en modo lectura tipo binario
    if(fileAssets == NULL){
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }
    
    while(feof(fileAssets) == 0){
        caracter = fgetc(fileAssets);
        printf("%c ", caracter);
    }
    return 0;
}
	
int main(int argc, char *argv[]){
    char petName;
    char userName;
    int gameAvatar;
    int petInmortality;
    int gameDifficult;

    assetsLoad(&petName);
    system("pause");
    return 0;
}
