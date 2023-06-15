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
int assetsLoad(struct AssetsData **assetsData)
{
    char ruta[] = "../files/assets.txt"; // se vuelve una carpeta para atras para que el .exe pueda abrir el archivo

    // el archivo assets contiene las configuraciones PRONCIPALES
    FILE *fileAssets = fopen(ruta, "rb"); // se habre en modo lectura tipo binario
    if (fileAssets == NULL)
    {
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }

    while (feof(fileAssets) == 0)
    {
        fflush(stdin);
        fscanf(fileAssets, "%c", &(*assetsData)->petName);
    }
    printf("%s", (*assetsData)->petName);
    return 0;
}

int main()
{
    struct AssetsData* assetsData = (struct AssetsData*)malloc(sizeof(struct AssetsData));

    assetsLoad(&assetsData);
    system("pause");
    return 0;
}
