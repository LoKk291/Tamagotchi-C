#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#define N 13

// esta estructura contiene la información PRINCIPAL en tiempo de ejecución
struct AssetsData
{
    // tanto el nombre de la mascomo como de usuario seran de maximo 12 caracteres
    char petName[N];
    char userName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;
    int gameTryHard; // si esta opción es verdadera, una vez que la mascota muera no se podrá volver a jugar
};

// esta estructura almacena los datos de las barras de estado
struct dataStateBars
{
    int health;
    int mood;
    int hungry;
};

//esta estructura almacena la fecha antes de que sea formateada (se usa para calcular la fecha actual a partir de los segundos) 
struct tm *currentDate;

struct currentDateFormated{
    int day;
    int month;
    int year;
    int hour;
    int minute;
    int second;
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

    // lee los datos del archivo y los carga en la estructura
    fflush(stdin);
    fscanf(fileAssets, "%s%s%i%i%i%i", &(*ptrAssetsData)->userName, &(*ptrAssetsData)->petName, &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult, &(*ptrAssetsData)->gameTryHard);
    fclose(fileAssets); // terminada la carga del struct se ciera el archivo
    return 0;
}

//convierte los segundos obtenidos en la variable "timeNow" en fecha y hora exacta
int timeConverter(time_t timeNow, struct tm *currentDate, struct currentDateFormated* ptrCurrentDateFormated)
{
    currentDate= localtime(timeNow);

    
    return 0;
}

// para hacer la actualizacion de las barras de estado usaré la librería time.h con su función "clock_t clock (void)"
int stateBars()
{

    return 0;
}

//esta funcion abrira el archivo "lastOpen" y dependiendo del modo, escribirá en el u obtendrá su contenido
//mode = 1 (guarda) mode = 0 (lee)
int lastOpenGetter(int mode, time_t timeNow){
    time_t timeLast; //guardara lo que contenga el archivo
    double timeResult = 0;
    if(mode){
        // guarda el instante de salida en el archivo "lastClose.txt"
        FILE *fileLastClose = fopen("../files/lastClose.txt", "w");
        fprintf(fileLastClose, "%ld", timeNow);
        fclose(fileLastClose);
    }else{
        // lee el contenido
        FILE *fileLastClose = fopen("../files/lastClose.txt", "r");
        fscanf(fileLastClose, "%ld", &timeLast);
        fclose(fileLastClose);

        /*Recibe dos variables de tipo time_t, calcula su diferencia
        y devuelve el resultado (double) expresado en segundos.*/
        timeResult = difftime(timeNow, timeLast);

    }
    
    printf("\nLa diferencia de tiempo es: %f \n", timeResult);
    return 0;
}

int main()
{
    char optMenu;

    // time_t es un tipo de dato que permite guardar una "marca de tiempo"
    time_t timeNow = time(NULL);

    struct AssetsData *ptrAssetsData = (struct AssetsData *)malloc(sizeof(struct AssetsData)); // se le asigna un espacio en memoria a la estructura
    struct dataStateBars *ptrDataStateBars = (struct dataStateBars *)malloc(sizeof(struct dataStateBars));

    if (assetsLoad(&ptrAssetsData))
    {
        printf("ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        return 1;
    }
    
    //lee y calcula la diferencia con la ultima sesion
    lastOpenGetter(0, timeNow); 

    /*
    //esteregg "time since 1970"
    time_t since1970 = time(NULL);
    printf("\n%ld\n", since1970);
    */

    do
    {
        printf("Desea salir? (s/n): ");
        scanf("%c", &optMenu);
    } while (optMenu != 's');

    // captura el instante de tiempo de la salida del juego para calcular el tiempo transcurrido cuando el jeugo se vuelve a abrir
    timeNow = time(NULL);
    
    //guarda la salida de la ultima sesion
    lastOpenGetter(1, timeNow); 

    system("pause");
    return 0;
}
