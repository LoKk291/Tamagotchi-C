#include <stdio.h>
#include <stdlib.h>
#include <time.h>
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
    int gameTryHard; //si esta opción es verdadera, una vez que la mascota muera no se podrá volver a jugar
};

//esta estructura almacena los datos de las barras de estado
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
    fscanf(fileAssets, "%s%s%i%i%i%i", &(*ptrAssetsData)->userName, &(*ptrAssetsData)->petName, &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult, &(*ptrAssetsData)->gameTryHard);
    fclose(fileAssets); //terminada la carga del struct se ciera el archivo
    return 0;
}

//para hacer la actualizacion de las barras de estado usaré la librería time.h con su función "clock_t clock (void)"
int stateBars(){

    return 0;
}

int main()
{
    char optMenu;
    
    //time_t es un tipo de dato que permite guardar una "marca de tiempo"
    time_t timeNow;

    struct AssetsData* ptrAssetsData = (struct AssetsData*)malloc(sizeof(struct AssetsData)); //se le asigna un espacio en memoria a la estructura
    struct dataStateBars* ptrDataStateBars = (struct dataStateBars*)malloc(sizeof(struct dataStateBars));

    if(assetsLoad(&ptrAssetsData)){
        printf("ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        return 1;
    }

    /*
    //esteregg "time since 1970"
    time_t since1970 = time(NULL);
    printf("\n%ld\n", since1970);
    */
    
    do{
        printf("Desea salir? (s/n): ");
        scanf("%c", &optMenu);
        
        if(optMenu == 's'){
            //captura el instante de salida del juego para calcular el tiempo transcurrido cuando el jeugo se vuelve a abrir
            timeNow = time(NULL);
            /*
            esto dara como resultado la cantidad de tiempo transcurridad desde el 1 de Enero de 1970
            (conviertiendo esos numeros obtenemos la fecha actual)
            */
           
            //guarda el instante de salida en el archivo "lastClose.txt"
            FILE* fileLastClose = fopen("../files/lastClose.txt", "w");
            fprintf(fileLastClose, "%i", timeNow);
            fclose(fileLastClose);
        }
    }while(optMenu != 's');
    
    system("pause");
    return 0;
}
