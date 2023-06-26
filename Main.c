#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#include <windows.h>
#include <conio.h>

#define N 15
#define M 100

// contiene la información PRINCIPAL en tiempo de ejecución
struct AssetsData
{
    // tanto el nombre de la mascota como de usuario seran de maximo 12 caracteres
    char userName[N];
    char petName[N];
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

// contiene las monedas del usuario, mas adelante agregaré gemas
struct walletData
{
    int coins;
};

// chequea si es la primera vez que se abre el programa y pide las configuraciones inciales
int firstTime(time_t timeNow)
{
    int firstOpen;

    // cuando se abre por primera vez, el archivo firstOpen solo guardara un entero "1"
    FILE *fileFirstOpen = fopen("../files/firstOpen.txt", "r");
    fscanf(fileFirstOpen, "%i", &firstOpen);
    fclose(fileFirstOpen);

    // si se abre por primera vez, se setea en 0 y se guarda el momento de la primera apertura
    if (firstOpen == 1)
    {
        firstOpen = 0;
        FILE *fileFirstOpen = fopen("../files/firstOpen.txt", "w");
        fprintf(fileFirstOpen, "%i\n%ld", firstOpen, timeNow);
        fclose(fileFirstOpen);

        // se setea el archivo lastStateBars en 100
        FILE *fileStateBars = fopen("../files/lastStateBars.txt", "w");
        fprintf(fileStateBars, "%i %i %i", 100, 100, 100);
        fclose(fileStateBars);

        // se setea el archivo wallet en 250
        FILE *fileWallet = fopen("../files/lastStateBars.txt", "w");
        fprintf(fileWallet, "%i", 250); // cantidad de oro inicial para el jugador
        fclose(fileWallet);

        return 1;
    }

    return 0;
}

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

// incrementa ptrDataStateBars->hungry dependiendo de la comida empleada (si es comida chatarra decrementa la barra de salud)
// si la comida seleccionada es saludable incrementa un poco la salud
// ESTA FUCION ES SUPER BÁSICA Y ES SOLO PARA GUIARME HASTA QUE AGUSTIN HAGA LA PARTE DE ARBOLES
int alimentation(struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData)
{
    int optAlimentation;
    do
    {
        printf("\n1. Comida balanceada (aumenta la salud 5)\n");
        printf("2. Comida chatarra (decrementa la salud en 10)\n");
        printf("Selecciona la comida que quieres darle a %s:", (*ptrAssetsData)->petName);
        scanf("%i", &optAlimentation);
    } while (optAlimentation != 1 && optAlimentation != 2);

    if (optAlimentation == 1)
    {
        (*ptrDataStateBars)->hungry += 10;
        (*ptrDataStateBars)->health += 5;
    }
    else
    {
        (*ptrDataStateBars)->hungry += 25;
        (*ptrDataStateBars)->health -= 10;
    }

    // para asegurarse de que la barra no sobrepase los 100
    if ((*ptrDataStateBars)->hungry > 100)
    {
        (*ptrDataStateBars)->hungry = 100;
    }

    // como los alimentos también incrementan la salud, acá también tengo que verificar que no exceda los 100
    if ((*ptrDataStateBars)->health > 100)
    {
        (*ptrDataStateBars)->health = 100;
    }
    // MAS ADELANTE SE PUEDE AGREGAR LA FUNCIÓN SOBREPESO
    return 0;
}

// incrementa ptrDataStateBars->health dependiendo de la medicina seleccionada
int healing(struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData)
{
    int optHealing;
    do
    {
        printf("\n1.Pastilla (aumenta la salud en 25)\n");
        printf("2.Inyección (aumenta la salud en 50)\n");
        printf("Selecciona la medicina que quieres darle a %s:", (*ptrAssetsData)->petName);
        scanf("%i", &optHealing);
    } while (optHealing != 1 && optHealing != 2);

    if (optHealing == 1)
    {
        (*ptrDataStateBars)->health += 25;
    }
    else
    {
        (*ptrDataStateBars)->health += 50;
    }

    // para asegurarse de que la barra no sobrepase los 100
    if ((*ptrDataStateBars)->health > 100)
    {
        (*ptrDataStateBars)->health = 100;
    }
    return 0;
}

// muestra los estados de las barras (por el momento en numeros)
void showStateBars(struct dataStateBars **ptrDataStateBars)
{
    // caracter de tabla ascii extendida
    char icon = 35; // 35 = #

    // estos for dibujan las barras y los if determinan el color de la barra
    // salud
    if ((*ptrDataStateBars)->health >= 70)
    {
        printf(GREEN);
    }
    else if (((*ptrDataStateBars)->health <= 69) && ((*ptrDataStateBars)->health >= 40))
    {
        printf(YELLOW);
    }
    else
    {
        printf(RED);
    }

    printf("Salud: ");
    for (int i = 0; i < ((*ptrDataStateBars)->health) / 4; i++)
    {
        printf("%c", icon);
    }
    printf(" %i/100\n", (*ptrDataStateBars)->health);

    // animo
    if ((*ptrDataStateBars)->mood >= 70)
    {
        printf(GREEN);
    }
    else if (((*ptrDataStateBars)->mood <= 69) && ((*ptrDataStateBars)->hungry >= 40))
    {
        printf(YELLOW);
    }
    else
    {
        printf(RED);
    }
    printf("Animo: ");
    for (int i = 0; i < ((*ptrDataStateBars)->mood) / 4; i++)
    {
        printf("%c", icon);
    }
    printf(" %i/100\n", (*ptrDataStateBars)->mood);

    // hambre
    if ((*ptrDataStateBars)->hungry >= 70)
    {
        printf(GREEN);
    }
    else if (((*ptrDataStateBars)->hungry <= 69) && ((*ptrDataStateBars)->hungry >= 40))
    {
        printf(YELLOW);
    }
    else
    {
        printf(RED);
    }
    printf("Hambre: ");
    for (int i = 0; i < ((*ptrDataStateBars)->hungry) / 4; i++)
    {
        printf("%c", icon);
    }
    printf(" %i/100\n", (*ptrDataStateBars)->hungry);

    printf(RESET);
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
    if ((*ptrAssetsData)->gameAvatar == 3)
    {
        avatarPath[23] = '3';
    }
    if ((*ptrAssetsData)->gameAvatar == 4)
    {
        avatarPath[23] = '4';
    }
    if ((*ptrAssetsData)->gameAvatar == 5)
    {
        avatarPath[23] = '5';
    }

    FILE *fileAvatar = fopen(avatarPath, "r");
    printf(MAGENTA "\n");
    while (!feof(fileAvatar)) // recorre el archivo hasta el final
    {
        fgets(line, M, fileAvatar);
        printf("%s", line);
    }
    printf(RESET "\n");
    fclose(fileAvatar);
}

// permite modificar los ajustes del archivos assets, luego de ejecutada la funcion debe recargar la estructura "AssetsData"
// tiene dos modos, mode = 1 permite modificiar los valores / mode = 0 es para cargar los valores iniciales
int settings(int mode, struct AssetsData **ptrAssetsData)
{
    // almacenan temporalmente las preferencias actualizadas
    char petName[N];
    char userName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;
    int gameTryHard;

    char optChange;

    if (mode)
    {

        FILE *fileAssets = fopen("../files/assets.txt", "rb");
        if (fileAssets == NULL)
        {
            return 1;
        }

        fflush(stdin);
        printf("Los valores actuales son: \n");
        printf("Nombre de usuario: %s\n", (*ptrAssetsData)->userName);
        printf("Nombre de la mascota: %s\n", (*ptrAssetsData)->petName);
        printf("Avatar seleccionado: %i\n", (*ptrAssetsData)->gameAvatar);
        printf("Inmortalidad de la inmortalidad: %i\n", (*ptrAssetsData)->petInmortality);
        printf("Dicifultad del juego: %i\n", (*ptrAssetsData)->gameDifficult);
        printf("Modo TryHard: %i\n", (*ptrAssetsData)->gameTryHard);

        fclose(fileAssets);

        do
        {
            printf(RED "Desea cambiar estos valores?(S/N): ");
            scanf("%c", &optChange);
        } while (optChange != 's' && optChange != 'n' && optChange != 'S' && optChange != 'N');
        printf(RESET);
    }

    // AGREGAR LA COMPROBACION DE DATOS
    if (optChange == 's' || optChange == 'S' || mode == 0)
    {
        if (mode)
        {
            printf("Ingrese los nuevos valores que desea agregar:\n");
        }
        else
        {
            printf("Ingrese los datos que desea:\n");
        }

        printf("Nombre del usuario (no puede ser mayor a 12 caracteres): ");
        scanf("%s", userName);
        printf("Nombre de la mascota (no puede ser mayor a 12 caracteres): ");
        scanf("%s", petName);
        printf("Avatar seleccionado (0/1/2): ");
        scanf("%i", &gameAvatar);
        printf("Inmortalidad de la mascota: (1/0): ");
        scanf("%i", &petInmortality);
        printf("Dicifultad del juego (0/1/2): ");
        scanf("%i", &gameDifficult);
        printf("Modo TryHard (0/1):");
        scanf("%i", &gameTryHard);

        fflush(stdin);
        printf(BLUE "SEGURO QUE DESEAS APLICAR LOS CAMBIOS?: ");
        scanf("%c", &optChange);

        // se guardan las opciones
        if (optChange == 's' || optChange == 'S' || mode == 0)
        {
            FILE *fileAssets = fopen("../files/assets.txt", "w");
            if (fileAssets == NULL)
            {
                return 1;
            }
            fprintf(fileAssets, "%s\n%s\n%i\n%i\n%i\n%i", userName, petName, gameAvatar, petInmortality, gameDifficult, gameTryHard);
            fclose(fileAssets);

            printf(GREEN "\nDatos guardados...\n");
        }
        else
        {
            printf(YELLOW "\nDatos descartados...\n");
        }
        printf(RESET);
    }

    return 0;
}

// guarda o carga la cantidad de monedas del usuario
// mode = 1 guarda la informacion en el archivo wallet.txt, mode = 0 lee el contenido del archivo
int walletGetterAndSaver(int mode, struct walletData **ptrWalletData)
{
    if (mode)
    {
        // guarda la cantidad de monedas del usuario al momento de cerrar la sesión
        FILE *fileWallet = fopen("../files/wallet.txt", "w");
        fprintf(fileWallet, "%i", (*ptrWalletData)->coins);
        fclose(fileWallet);
    }
    else
    {
        // carga la cantidad de monedas del usuario de la sesion anterior
        FILE *fileWallet = fopen("../files/wallet.txt", "r");
        fscanf(fileWallet, "%i", &(*ptrWalletData)->coins);
        fclose(fileWallet);

        // printf("\nLa cantidad de monedas de la última sesión es: %i\n", (*ptrWalletData)->coins);
    }
}

// determina en cuanto se deben decrementar las barras dependiendo del tiempo transcurrido y de
// la dificultad seleccionada por el usuario
void stateBarsDecrement(struct elpasedTime **ptrElpasedTime, struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData)
{
    int decrement;

    // por cada hora transcurrida se decrementa un porcentaje dado a cada barra de estado

    if ((*ptrElpasedTime)->hours >= 1)
    { // para que comience a decrementar debe transcurrir minimo una hora
        if ((*ptrAssetsData)->gameDifficult == 0)
        { // facil
            decrement = 2;
        }
        else if ((*ptrAssetsData)->gameDifficult == 1)
        { // medio
            decrement = 5;
        }
        else
        { // dificl
            decrement = 10;
        }

        // decrementa dependiendo de la dificultad que ha seleccionado el usuario
        // printf("\nLa cantidad de horas transcurridas fueron: %i\n", (*ptrElpasedTime)->hours);
        for (int i = 0; i < (*ptrElpasedTime)->hours; i++)
        {
            (*ptrDataStateBars)->health = (*ptrDataStateBars)->health - decrement;
            (*ptrDataStateBars)->mood = (*ptrDataStateBars)->mood - decrement;
            (*ptrDataStateBars)->hungry = (*ptrDataStateBars)->hungry - decrement;
        }
    }

    // AREA DE MUESTRA PARA LA EXPOSICION
}

// muestra una pantalla de meurte cuado la salud es <= 0
void deathScreen(struct AssetsData *ptrAssetsData)
{
    printf("                       ______\n");
    printf("                    .-\"      \"-.\n");
    printf("                   /            \\\n");
    printf("       _          |              |          _\n");
    printf("      ( \\         |,  .-.  .-.  ,|         / )\n");
    printf("       > \"=._     | )(__/  \\__)( |     _.=\" <\n");
    printf("      (_/\"=._\"=._ |/     /\\     \\| _.=\"_.=\"\\_)\n");
    printf("             \"=._ (_     ^^     _)\"_.=\"\n");
    printf("                 \"=\\__|IIIIII|__/=\"\n");
    printf("                _.=\"| \\IIIIII/ |\"=._\n");
    printf("      _     _.=\"_.=\"\\          /\"=._\"=._     _\n");
    printf("     ( \\_.=\"_.=\"     `--------`     \"=._\"=._/ )\n");
    printf("      > _.=\"                            \"=._ <\n");
    printf("     (_/                                    \\_)\n");

    printf(RED "%s eres un IRRESPONSABLE!!! Dejaste que al pobre %s le diera un cortocircuito :(\n", ptrAssetsData->userName, ptrAssetsData->petName);
    printf(WHITE "%s ahora descansa en el");
    printf(GREEN " paraiso electronico...\n", ptrAssetsData->petName);
    printf(RESET);
    system("pause");

    // aqui debe ir el ascii de una calavera
}

// explica como usar y cuidar a la mascota
void tutorial()
{
}

int main()
{
    system("cls");
    // time_t es un tipo de dato que permite guardar una "marca de tiempo"
    time_t timeNow = time(NULL);

    struct AssetsData *ptrAssetsData = (struct AssetsData *)malloc(sizeof(struct AssetsData)); // se le asigna un espacio en memoria a la estructura
    struct elpasedTime *ptrElpasedTime = (struct elpasedTime *)malloc(sizeof(struct elpasedTime));
    struct dataStateBars *ptrDataStateBars = (struct dataStateBars *)malloc(sizeof(struct dataStateBars));
    struct walletData *ptrWalletData = (struct walletData *)malloc(sizeof(struct walletData));

    int timeResult = lastOpenGetterAndSaver(0, timeNow);

    char keyOptMenu;

    // si es la primera vez que se abre el programa, pedira las confg iniciales
    if (firstTime(timeNow) != 0)
    {
        settings(0, &ptrAssetsData);
    }

    if (assetsLoad(&ptrAssetsData))
    {
        printf(RED "ERROR FATAL, NO SE CARGARON LOS DATOS PRINCIPALES...");
        system("pause");
        return 1;
    }

    // lee y calcula la diferencia con la ultima sesion y lo guarda
    timeConverter(timeResult, &ptrElpasedTime);

    stateBarsGetterAndSaver(0, &ptrDataStateBars);

    stateBarsDecrement(&ptrElpasedTime, &ptrDataStateBars, &ptrAssetsData);

    walletGetterAndSaver(0, &ptrWalletData);

    /*
    //esteregg "time since 1970"
    time_t since1970 = time(NULL);
    printf("\n%ld\n", since1970);
    */

    /*
    eastergg de sonido "thoy dont know me son"
    */

    splashScreen();

    printf(GREEN "Hola de nuevo %s!\n", ptrAssetsData->userName);
    printf(RESET);

    do
    {

        // si en las configuraciones la inmortalidad esta activada, la mascota no puede morir
        if (ptrAssetsData->petInmortality != 1)
        {
            if (ptrDataStateBars->health <= 0)
            {
                // printf("\n La salud es: %i\n", ptrDataStateBars->health);
                deathScreen(ptrAssetsData);
                return 1;
            }
        }

        // printf("\n%s\n", ptrAssetsData->petName); //para probar si los datos se cargaron correctamente
        showAvatar(&ptrAssetsData);
        randomPhrases();
        showStateBars(&ptrDataStateBars);

        printf(CYAN "\n1. Alimentar\n");
        printf("2. Curar\n");
        printf("3. Juegos\n");
        printf("4. Configuraciones\n");
        printf("0. Como cuidar la mascota\n");
        printf("S. Salir\n");
        printf(RESET);

        fflush(stdin);
        printf("\nPresione la tecla de la opción que desea realizar..");
        keyOptMenu = getch();
        system("cls");

        if (keyOptMenu != 's' || keyOptMenu != 'S')
        {
            switch (keyOptMenu)
            {
            case 0:
                tutorial();
                break;
            case '1':
                alimentation(&ptrDataStateBars, &ptrAssetsData);
                break;
            case '2':
                healing(&ptrDataStateBars, &ptrAssetsData);
                break;
            case '3':
                break;
            case '4':
                settings(1, &ptrAssetsData);
                assetsLoad(&ptrAssetsData);
                break;
            case 's':
            case 'S':
                break;
            default:
                printf(YELLOW "\nEscoje una opción válida!!\n");
                printf(RESET);
            }
        }

    } while (keyOptMenu != 's' && keyOptMenu != 'S');

    // captura el instante de tiempo de la salida del juego para calcular el tiempo transcurrido cuando el jeugo se vuelve a abrir
    timeNow = time(NULL);

    // guarda la salida de la ultima sesion y los estados de las barras
    lastOpenGetterAndSaver(1, timeNow);
    stateBarsGetterAndSaver(1, &ptrDataStateBars);
    walletGetterAndSaver(1, &ptrWalletData);

    system("pause");
    return 0;
}
