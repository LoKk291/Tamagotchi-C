#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#define N 13

// contiene la información PRINCIPAL en tiempo de ejecución
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

// almacena los datos de las barras de estado
struct dataStateBars
{
    int health;
    int mood;
    int hungry;
};

// almacena las horas/minutos/segundos que transcurrieron desde la ultima sesión de juego
struct elpasedTime
{
    int seconds;
    int minutes;
    int hours;
};

// esta funcion carga las configuraciones PRINCIPALES
int assetsLoad(struct AssetsData **ptrAssetsData)
{
    char ruta[] = "../files/assets.txt"; // se vuelve una carpeta para atras para que el .exe pueda abrir el archivo

    // el archivo assets contiene las configuraciones PRINCIPALES
    FILE *fileAssets = fopen(ruta, "rb");
    if (fileAssets == NULL)
    {
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }

    // lee los datos del archivo y los carga en la estructura
    fflush(stdin);
    fscanf(fileAssets, "%s%s%i%i%i%i", &(*ptrAssetsData)->userName, &(*ptrAssetsData)->petName, &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult, &(*ptrAssetsData)->gameTryHard);
    fclose(fileAssets);
    return 0;
}

// convierte los segundos de la variable timeResult de la funcion lastOpenGetter horas/minutos/segundos
// y almacena los datos procesados en struct elapsedTime
int timeConverter(double timeResult, struct elpasedTime **ptrElpasedTime)
{
    int hours = 0;
    int minutes = 0;
    int seconds = timeResult;

    hours = seconds / 3600;
    minutes = (seconds - hours * 3600) / 60;
    seconds = seconds - (hours * 3600 + minutes * 60);
    printf("\nla cantidad de segundos es: %i\n", seconds);
    printf("\n%i %i %i\n", hours, minutes, seconds);

    (*ptrElpasedTime)->hours = hours;
    (*ptrElpasedTime)->minutes = minutes;
    (*ptrElpasedTime)->seconds = seconds;

    printf("\n%i %i %i\n", (*ptrElpasedTime)->hours, (*ptrElpasedTime)->minutes, (*ptrElpasedTime)->seconds);
    return 0;
}

// para hacer la actualizacion de las barras de estado usaré la librería time.h con su función "clock_t clock (void)"
int stateBars()
{

    return 0;
}

// esta funcion abrira el archivo "lastOpen" y dependiendo del modo, escribirá en el u obtendrá su contenido
// mode = 1 (guarda) mode = 0 (lee)
int lastOpenGetter(int mode, time_t timeNow)
{
    time_t timeLast; // guardara lo que contenga el archivo
    double timeResult = 0;
    if (mode)
    {
        // guarda el instante de salida en el archivo "lastClose.txt"
        FILE *fileLastClose = fopen("../files/lastClose.txt", "w");
        fprintf(fileLastClose, "%ld", timeNow);
        fclose(fileLastClose);
    }
    else
    {
        // lee el contenido
        FILE *fileLastClose = fopen("../files/lastClose.txt", "r");
        fscanf(fileLastClose, "%ld", &timeLast);
        fclose(fileLastClose);

        /*Recibe dos variables de tipo time_t, calcula su diferencia
        y devuelve el resultado (double) expresado en segundos.*/
        // guarda la diferencia entre la sesion anterior y el timeNow
        timeResult = difftime(timeNow, timeLast);
    }

    // printf("\nLa diferencia de tiempo es: %f \n", timeResult);
    return timeResult;
}

int main()
{
    // time_t es un tipo de dato que permite guardar una "marca de tiempo"
    time_t timeNow = time(NULL);

    int timeResult = lastOpenGetter(0, timeNow);
    ;

    char optMenu;

    struct AssetsData *ptrAssetsData = (struct AssetsData *)malloc(sizeof(struct AssetsData)); // se le asigna un espacio en memoria a la estructura
    struct dataStateBars *ptrDataStateBars = (struct dataStateBars *)malloc(sizeof(struct dataStateBars));
    struct elpasedTime *ptrElpasedTime = (struct elpasedTime *)malloc(sizeof(struct elpasedTime));

    if (assetsLoad(&ptrAssetsData))
    {
        printf("ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        return 1;
    }

    // lee y calcula la diferencia con la ultima sesion y lo guarda

    timeConverter(timeResult, &ptrElpasedTime);

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

    // guarda la salida de la ultima sesion
    lastOpenGetter(1, timeNow);

    system("pause");
    return 0;
}
