#include <stdio.h>
#include <stdlib.h>
#include "libraries/colors.h"
#define N 12

// esta estructura contiene la información PRINCIPAL en timepo de ejecución
struct AssetsData
{
    char petName[N];
    char userName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;
};

// esta funci�n carga las configuraciones PRINCIPALES
int assetsLoad(struct AssetsData **ptrAssetsData)
{
    char ruta[] = "../files/assets.txt"; // se vuelve una carpeta para atras para que el .exe pueda abrir el archivo

    // el archivo assets contiene las configuraciones PRONCIPALES
    FILE *fileAssets = fopen(ruta, "rb"); // se habre en modo lectura tipo binario
    if (fileAssets == NULL)
    {
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }

    //lee los datos del archivo y los carga en la estructura
    fflush(stdin);
    fscanf(fileAssets, "%s%s%i%i%i", &(*ptrAssetsData)->userName, &(*ptrAssetsData)->petName, &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult);
    fclose(fileAssets); //terminada la carga del struct se ciera el archivo
    
    return 0;
}

int main()
{
    struct AssetsData* ptrAssetsData = (struct AssetsData*)malloc(sizeof(struct AssetsData)); //se le asigna un espacio en memoria a la estructura

    assetsLoad(&ptrAssetsData);
    system("pause");
    return 0;
}
