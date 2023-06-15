#include <stdio.h>
#include <stdlib.h>
#include "libraries/colors.h"
#define N 13

// esta estructura contiene la información PRINCIPAL en tiempo de ejecución
struct AssetsData
{
    //tanto el nombre de la mascomo como de usuario seran de maximo 12 caracteres
    char petName[N];
    char userName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;
};

struct dataStateBars{
    int health;
    int mood;
    int hungry;
};

// esta funcion carga las configuraciones PRINCIPALES
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

int stateBars(){
    
    return 0;
}

int main()
{
    struct AssetsData* ptrAssetsData = (struct AssetsData*)malloc(sizeof(struct AssetsData)); //se le asigna un espacio en memoria a la estructura
    struct dataStateBars* ptrDataStateBars = (struct dataStateBars*)malloc(sizeof(struct dataStateBars));

    if(assetsLoad(&ptrAssetsData)){
        printf("ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        return 1;
    }

    system("pause");
    return 0;
}
