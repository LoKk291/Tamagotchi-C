#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#include <windows.h>

#define N 13
#define M 100

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
        return 1;
    }

    // lee los datos del archivo y los carga en la estructura
    fflush(stdin);
    fscanf(fileAssets, "%s%s%i%i%i%i", &(*ptrAssetsData)->userName, &(*ptrAssetsData)->petName, &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult, &(*ptrAssetsData)->gameTryHard);
    fclose(fileAssets);
    return 0;
}

// convierte los segundos de la variable timeResult de la funcion lastOpenGetterAndSaver horas/minutos/segundos
// y almacena los datos procesados en struct elapsedTime
int timeConverter(double timeResult, struct elpasedTime **ptrElpasedTime)
{
    int hours = 0;
    int minutes = 0;
    int seconds = timeResult;

    hours = seconds / 3600;
    minutes = (seconds - hours * 3600) / 60;
    seconds = seconds - (hours * 3600 + minutes * 60);
    // printf("\nla cantidad de segundos es: %i\n", seconds);
    // printf("\n%i %i %i\n", hours, minutes, seconds);

    (*ptrElpasedTime)->hours = hours;
    (*ptrElpasedTime)->minutes = minutes;
    (*ptrElpasedTime)->seconds = seconds;

    // printf("\n%i %i %i\n", (*ptrElpasedTime)->hours, (*ptrElpasedTime)->minutes, (*ptrElpasedTime)->seconds);
    return 0;
}

// esta funcion abrira el archivo "lastOpen" y dependiendo del modo, escribirá en el u obtendrá su contenido
// mode = 1 (guarda) mode = 0 (lee)
int lastOpenGetterAndSaver(int mode, time_t timeNow)
{
    time_t timeLast; // guardara lo que contenga el archivo
    double timeResult = 0;
    if (mode)
    {
        // guarda el instante de salida en el archivo "lastClose.txt"
        FILE *fileLastClose = fopen("../files/lastClose.txt", "w");
        fprintf(fileLastClose, "%ld", timeNow);
        fclose(fileLastClose);
        // printf("\nGuardado finalizado..\n");
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
        // printf("\nCarga finalizada..\n");
    }

    // printf("\nLa diferencia de tiempo es: %f \n", timeResult);
    return timeResult;
}

// carga los estados de la barra anterior y calcula el valor real en base al tiempo transcurrido
int stateBarsGetterAndSaver(int mode, struct dataStateBars **ptrDataStateBars)
{
    if (mode)
    {
        // guarda los estados actuales
        FILE *filelastStateBars = fopen("../files/lastStateBars.txt", "w");
        fprintf(filelastStateBars, "%i %i %i", (*ptrDataStateBars)->health, (*ptrDataStateBars)->mood, (*ptrDataStateBars)->hungry);
        fclose(filelastStateBars);

        // printf("\n%i%i%i\n", (*ptrDataStateBars)->health, (*ptrDataStateBars)->mood, (*ptrDataStateBars)->hungry);    //PRUEBAS
        // printf("\nGuardado finalizado..\n");  //PRUEBAS
    }
    else
    {
        // lee los estados anteriores
        FILE *filelastStateBars = fopen("../files/lastStateBars.txt", "r");
        fscanf(filelastStateBars, "%i %i %i", &(*ptrDataStateBars)->health, &(*ptrDataStateBars)->mood, &(*ptrDataStateBars)->hungry);
        //(*ptrDataStateBars)->health = 20;
        //(*ptrDataStateBars)->mood = 40;       //PRUEBAS
        //(*ptrDataStateBars)->hungry = 30;
        fclose(filelastStateBars);

        // printf("\n%i%i%i\n", (*ptrDataStateBars)->health, (*ptrDataStateBars)->mood, (*ptrDataStateBars)->hungry); //PRUEBAS
        // printf("\nCarga finalizada..\n"); //PRUEBAS
    }
    return 0;
}

//incrementa ptrDataStateBars->hungry dependiendo de la comida empleada (si es comida chatarra decrementa la barra de salud)
//si la comida seleccionada es saludable incrementa un poco la salud
//ESTA FUCION ES SUPER BÁSICA Y ES SOLO PARA GUIARME HASTA QUE AGUSTIN HAGA LA PARTE DE ARBOLES
int alimentation(struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData){
    int optAlimentation;
    do{
        printf("\n1. Comida balanceada (aumenta la salud en uno)\n");
        printf("2. Comida chatarra (aumenta la salud en uno)\n");
        printf("Selecciona la comida que quieres darle a %s:", (*ptrAssetsData)->petName);
        scanf("%i", &optAlimentation);
    }while(optAlimentation != 1 && optAlimentation != 2);

    if(optAlimentation == 1){
        (*ptrDataStateBars)->hungry += 10;
        (*ptrDataStateBars)->health += 5;
    }else{
        (*ptrDataStateBars)->hungry += 25;
        (*ptrDataStateBars)->health -= 10;
    }

    //para asegurarse de que la barra no sobrepase los 100
    if((*ptrDataStateBars)->hungry > 100){
        (*ptrDataStateBars)->hungry = 100;
    }

    //MAS ADELANTE SE PUED AGREGAR LA FUNCIÓN SOBREPESO
    return 0;
}

//incrementa ptrDataStateBars->health

//muestra los estados de las barras (por el momento en numeros)
void showAndIncrementerStateBars(struct dataStateBars **ptrDataStateBars){
    printf(GREEN"Salud: %i/100\n", (*ptrDataStateBars)->health);
    printf(BLUE"Animo: %i/100\n", (*ptrDataStateBars)->mood);
    printf(YELLOW"Hambre: %i/100\n", (*ptrDataStateBars)->hungry);
}

// arroja frases random contenidas en el archivo "phrases.txt"
int randomPhrases()
{
    char phrase[M];

    // como c no permite ir hasta una linea especifica de un archivo, creo un indice con un numero random
    // dentro de un rango dado por la cantidad de frases que contiene el archivo y recorro el archivo hasta
    // llegar a la linea que indica el indice random

    srand(time(NULL));
    int indexRandomMax = rand() % (147 - 1);
    int index = 0;

    // printf("\nEl valor aleatorio es: %d", indexRandomMax);

    FILE *filePhrases = fopen("../files/phrases.txt", "r");
    while (index <= indexRandomMax)
    {
        fgets(phrase, M, filePhrases);
        index++;
    }
    fclose(filePhrases);

    printf(GREEN "\n%s\n", phrase);
    printf(RESET);
    return 0;
}

// Muestra una pantalla de bienvenida al usuario
void splashScreen()
{
    char line[M];

    printf(ORANGE);
    FILE *fileSplashScreen = fopen("../files/splashScreen.txt", "r");
    while (!feof(fileSplashScreen))
    {
        fgets(line, M, fileSplashScreen);
        printf("%s", line);
    }
    printf(RESET "\n");
    fclose(fileSplashScreen);
    Sleep(5000);
    system("cls");
}

// muestra el avatar seleccionado por el usuario
void showAvatar(struct AssetsData **ptrAssetsData)
{
    char avatarPath[] = "../files/avatars/avatarX.txt";
    char line[M];

    // para optimizar el codigo, en vez de copiar toda la linea completa, lo que hago es cambiar unicamente
    // el caracter que altera que archivo va a abrirse
    if ((*ptrAssetsData)->gameAvatar == 0)
    {
        avatarPath[23] = '0';
    }
    if ((*ptrAssetsData)->gameAvatar == 1)
    {
        avatarPath[23] = '1';
    }
    if ((*ptrAssetsData)->gameAvatar == 2)
    {
        avatarPath[23] = '2';
    }

    FILE *fileAvatar = fopen(avatarPath, "r");
    printf(MAGENTA "\n");
    while (!feof(fileAvatar))
    {
        fgets(line, M, fileAvatar);
        printf("%s", line);
    }
    printf(RESET "\n");
    fclose(fileAvatar);
}

int main()
{
    system("cls");
    // time_t es un tipo de dato que permite guardar una "marca de tiempo"
    time_t timeNow = time(NULL);

    int timeResult = lastOpenGetterAndSaver(0, timeNow);

    char optMenu;

    struct AssetsData *ptrAssetsData = (struct AssetsData *)malloc(sizeof(struct AssetsData)); // se le asigna un espacio en memoria a la estructura
    struct elpasedTime *ptrElpasedTime = (struct elpasedTime *)malloc(sizeof(struct elpasedTime));
    struct dataStateBars *ptrDataStateBars = (struct dataStateBars *)malloc(sizeof(struct dataStateBars));

    if (assetsLoad(&ptrAssetsData))
    {
        printf(RED "ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        system("pause");
        return 1;
    }

    printf(GREEN "Hola de nuevo %s!", ptrAssetsData->userName);
    printf(RESET);

    // lee y calcula la diferencia con la ultima sesion y lo guarda
    timeConverter(timeResult, &ptrElpasedTime);

    stateBarsGetterAndSaver(0, &ptrDataStateBars);

    randomPhrases();

    /*
    //esteregg "time since 1970"
    time_t since1970 = time(NULL);
    printf("\n%ld\n", since1970);
    */

    /*
    eastergg de sonido "thoy dont know me son"
    */

    splashScreen();

    do
    {
        showAvatar(&ptrAssetsData);
        showAndIncrementerStateBars(&ptrDataStateBars);

        printf(BLUE "\n1. Alimentar\n");
        printf("2. Curar\n");
        printf("3. Jugar\n");
        printf("S. Salir\n");
        printf(RESET);

        fflush(stdin);
        scanf("%c", &optMenu);
        system("cls");

        switch(optMenu){
            case '1':
                alimentation(&ptrDataStateBars, &ptrAssetsData);
                break;
        }
    } while (optMenu != 's' && optMenu != 'S');

    // captura el instante de tiempo de la salida del juego para calcular el tiempo transcurrido cuando el jeugo se vuelve a abrir
    timeNow = time(NULL);

    // guarda la salida de la ultima sesion y los estados de las barras
    lastOpenGetterAndSaver(1, timeNow);
    stateBarsGetterAndSaver(1, &ptrDataStateBars);

    system("pause");
    return 0;
}
