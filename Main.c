#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#include <windows.h>
#include <conio.h>
#include <stdbool.h>
#include "games/tikTakToe.c"  // incluye el juego tikTakToe en el archivo, asi se pueden llamar directamente las funciones
#include "games/triviaMind.c" // incluye el juego triviaMind en el archivo, asi se pueden llamar directamente las funciones

#define N 15
#define M 100

// contiene la informacion PRINCIPAL (contenida en el archivo assets) en tiempo de ejecucion
struct AssetsData
{
    // tanto el nombre de la mascota como de usuario seran de maximo 14 caracteres
    char userName[N];
    char petName[N];
    int gameAvatar;

    int petInmortality;
    int gameDifficult;
};

// almacena los datos de las barras de estado
struct dataStateBars
{
    int health;
    int mood;
    int hungry;
};

// almacena las horas/minutos/segundos que transcurrieron desde la ultima sesion de juego
struct elpasedTime
{
    int seconds;
    int minutes;
    int hours;
};

// contiene las monedas del usuario
struct walletData
{
    int unsigned coins;
};

// nodo del arbol binario
struct node
{
    char itemName[50];     // Nombre del articulo
    int unsigned quantity; // Cantidad del articulo
    int unsigned price;    // precio del articulo
    char foodType[20];     // Tipo de comida
    struct node *left;     // Puntero al subarbol izquierdo
    struct node *right;    // Puntero al subarbol derecho
};

// Estructura para representar los nodos del Arbol binario
struct product
{
    char productName[50];  // Nombre del producto
    unsigned int quantity; // Cantidad de unidades
    unsigned int price;    // Precio del producto
    struct product *left;  // Puntero al subarbol izquierdo
    struct product *right; // Puntero al subarbol derecho
};

// chequea si es la primera vez que se abre el programa y pide las configuraciones inciales
int firstTime(time_t timeNow)
{
    int firstOpen;

    // cuando se abre por primera vez, el archivo firstOpen solo guardara un entero "1"
    FILE *fileFirstOpen = fopen("../files/firstOpen.txt", "r");
    fscanf(fileFirstOpen, "%i", &firstOpen);
    fclose(fileFirstOpen);

    // si se abre por primera vez EL PROGRAMA, se setea en 0 y se guarda el momento de la primera apertura
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

        // se setea el archivo wallet en 250 coins
        FILE *fileWallet = fopen("../files/wallet.txt", "w");
        fprintf(fileWallet, "%i", 250); // cantidad de oro inicial para el jugador
        fclose(fileWallet);

        // se setea el archivo sickPet en 0
        FILE *fileSickPet = fopen("../files/sickPet.txt", "w");
        fprintf(fileSickPet, "%i", 0);
        fclose(fileSickPet);

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
    // Se coloca el [0] para solucionar los warnings ocasionados
    fscanf(fileAssets, "%s%s%i%i%i", &(*ptrAssetsData)->userName[0], &(*ptrAssetsData)->petName[0], &(*ptrAssetsData)->gameAvatar, &(*ptrAssetsData)->petInmortality, &(*ptrAssetsData)->gameDifficult);
    fclose(fileAssets);
    return 0;
}

/* convierte los segundos de la variable timeResult de la funcion lastOpenGetterAndSaver en horas/minutos/segundos
 y almacena los datos procesados en struct elapsedTime, para que no se tenga que abrir una y otra vez el archivo*/
int timeConverter(double timeResult, struct elpasedTime **ptrElpasedTime)
{
    int hours = 0;
    int minutes = 0;
    int seconds = timeResult;

    hours = seconds / 3600;
    minutes = (seconds - hours * 3600) / 60;
    seconds = seconds - (hours * 3600 + minutes * 60);
    // printf("\nla cantidad de segundos es: %i\n", seconds); //PRUEBAS
    // printf("\n%i %i %i\n", hours, minutes, seconds);

    (*ptrElpasedTime)->hours = hours;
    (*ptrElpasedTime)->minutes = minutes;
    (*ptrElpasedTime)->seconds = seconds;

    // printf("\n%i %i %i\n", (*ptrElpasedTime)->hours, (*ptrElpasedTime)->minutes, (*ptrElpasedTime)->seconds);
    return 0;
}

// esta funcion abrira el archivo "lastOpen" y dependiendo del modo, escribira en el u obtendra su contenido
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

// carga los estados de la barra anterior y calcula el valor real en base al tiempo transcurrido o gaurda los estados actuales
// de las barras en el archivo lastSatateBars.txt (mode 1 = guarda | mode 0 = lee)
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

        // no permite que las barras tengan numeros negativos (no utilizamos un int unsigned para evitar problemas)
        // no utilizo if anidados ya que necesito que las 3 condiciones se evaluen
        if ((*ptrDataStateBars)->health < 0)
        {
            (*ptrDataStateBars)->health = 0;
        }

        if ((*ptrDataStateBars)->mood < 0)
        {
            (*ptrDataStateBars)->mood = 0;
        }

        if ((*ptrDataStateBars)->hungry < 0)
        {
            (*ptrDataStateBars)->hungry = 0;
        }
        // printf("\n%i%i%i\n", (*ptrDataStateBars)->health, (*ptrDataStateBars)->mood, (*ptrDataStateBars)->hungry); //PRUEBAS
        // printf("\nCarga finalizada..\n"); //PRUEBAS
    }
    return 0;
}

/* Dependiendo del numero generado (si es multiplo de 3), la mascota se enferma. Cuando se enferma guarda 1 en el
archivo sickPet. La funcion tambien lee el estado del archivo para saber si estaba o no enferma. Esto se produce cada
vez que se abre nuevamente la aplicacion

Como el valor no es importante en todo momento de la ejecucion, no se crea una estructura para empaquetar el dato
si no que solo se crea un puntero en el main.
mode 0 = guarda mode 1 = lee mode = 2 guarda cuando se sana*/
void sickPet(int mode, int *sickPetStatus)
{
    srand(time(NULL));
    int randNum = rand() % (99999);

    // si el numero generado, multiplicado por 2 y restandole 7, es divisible por 3, entonces la mascota se enferma
    randNum = (randNum * 2) - 7;
    // printf("\nEl numero random es: %i\n", randNum); //PRUEBAS
    // randNum = 123;

    // si el modo es 0 lee, si el modo es 1 guarda
    if (mode == 1)
    { // mode 1
        if (randNum % 3 == 0)
        {
            // printf("\nLa mascota se enferma\n");
            sickPetStatus = 1;
            FILE *fileSickPet = fopen("../files/sickPet.txt", "w");
            fprintf(fileSickPet, "%i", sickPetStatus);
            fclose(fileSickPet);
        }
    }
    else if (mode == 2)
    {
        sickPetStatus = 0;
        FILE *fileSickPet = fopen("../files/sickPet.txt", "w");
        fprintf(fileSickPet, "%i", sickPetStatus);
        fclose(fileSickPet);
    }
    else
    { // mode 0
        FILE *fileSickPet = fopen("../files/sickPet.txt", "r");
        fscanf(fileSickPet, "%i", &sickPetStatus);
        fclose(fileSickPet);
    }
}

// FUNCIONES NECESARIAS PARA LA IMPLEMENTACION DE ARBOLES BINARIOSS

// crea el nuevo nodo del arbol
struct node *createNode(char itemName[], int unsigned quantity, int unsigned price, char foodType[])
{
    // asignar memoria para el nuevo nodo
    struct node *node = (struct node *)malloc(sizeof(struct node));

    // copiar los valores al nodo

    strcpy(node->itemName, itemName);
    node->quantity = quantity;
    node->price = price;
    strcpy(node->foodType, foodType);

    // Inicializar los punteros izquierdo y derecho como NULL
    node->left = NULL;
    node->right = NULL;

    // Devolver el nodo creado
    return node;
}

// Funcion para insertar un nodo en el arbol
struct node *insertNode(struct node *root, char itemName[], int quantity, int price, char foodType[])
{
    // si el arbol esta vacio, crear un nuevo nodo y establecerlo como raiz
    if (root == NULL)
    {
        return createNode(itemName, quantity, price, foodType);
    }

    // Comparar el nombre del articulo con el nombre en el nodo actual
    if (strcmp(itemName, root->itemName) < 0)
    {
        // Si el nombre es menor, insertar el nodo en el subarbol izquierdo
        root->left = insertNode(root->left, itemName, quantity, price, foodType);
    }
    else if (strcmp(itemName, root->itemName) > 0)
    {
        // Si el nombre es mayor, insertar el nodo en el subarbol derecho
        root->right = insertNode(root->right, itemName, quantity, price, foodType);
    }
    else
    {

        // si el nombre es igual, incrementar la cantidad en el nodo actual
        root->quantity += quantity;
    }

    // devolver el nodo raiz modificado
    return root;
}

// Funcion para buscar un nodo en el arbol
struct node *searchNode(struct node *root, char itemName[])
{
    // Si el arbol esta vacio o el nombre del articulo coincide, devolver el nodo actual
    if (root == NULL || strcmp(root->itemName, itemName) == 0)
    {
        return root;
    }

    // comparar el nombre del articulo con el nombre en el nodo actual
    if (strcmp(itemName, root->itemName) < 0)
    {
        // Si el nombre es menor, buscar en el subarbol izquierdo
        return searchNode(root->left, itemName);
    }
    else
    {
        // Si el nombre es mayor, buscar en el subarbol derecho
        return searchNode(root->right, itemName);
    }
}

// Funcion para mostrar el inventario en orden
void showInventory(struct node *root)
{
    if (root != NULL)
    {
        // Recorrer el subarbol izquierdo
        showInventory(root->left);

        // Mostrar los detalles del articulo en el nodo actual
        printf("Nombre: %s, Cantidad: %u, Precio: %u, Tipo: %s\n",
               root->itemName, root->quantity, root->price, root->foodType);

        // Recorrer el subarbol derecho
        showInventory(root->right);
    }
}

// Funcion auxiliar para encontrar el nodo con el valor minimo
struct node *findMinimum(struct node *root)
{
    struct node *current = root;
    while (current && current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

// Funcion para eliminar un nodo del arbol
struct node *deleteNode(struct node *root, char itemName[])
{
    if (root == NULL)
    {
        return root;
    }

    if (strcmp(itemName, root->itemName) < 0)
    {
        root->left = deleteNode(root->left, itemName);
    }
    else if (strcmp(itemName, root->itemName) > 0)
    {
        root->right = deleteNode(root->right, itemName);
    }
    else
    {
        if (root->quantity > 1)
        {
            root->quantity--;
        }
        else
        {

            if (root->left == NULL)
            {
                struct node *temp = root->right;
                free(root);
                return temp;
            }
            else if (root->right == NULL)
            {
                struct node *temp = root->left;
                free(root);
                return temp;
            }

            struct node *temp = findMinimum(root->right);
            strcpy(root->itemName, temp->itemName);
            root->quantity = temp->quantity;
            root->price = temp->price;
            strcpy(root->foodType, temp->foodType);
            root->right = deleteNode(root->right, temp->itemName);
        }
    }

    return root;
}

// administra el contenido del archivo de inventario de la tienda de alimentos
// si el modo = 0: guarda los elementos del arbol. si el modo = 1: carga los elementos del archivo al arbol
// modo = 2: borra todo el contenido del archivo, se usa este modo luego de cargar el contenido del archivo al arbol
// (como la funcion usa recursividad es necesario un modo aparte para limpiar el archivo)
void saveReadInventoryA(int mode, struct node *root)
{
    // carga cada una de las ramas en el archivo
    if (mode == 0)
    {
        FILE *fileAlimentationInventory = fopen("../files/alimentationInventory.txt", "a");
        if (root != NULL)
        {
            // Recorrer el subarbol izquierdo
            saveReadInventoryA(0, root->left);

            fprintf(fileAlimentationInventory, "%s %i %i %s\n",
                    root->itemName, root->quantity, root->price, root->foodType);

            // Recorrer el subarbol derecho
            saveReadInventoryA(0, root->right);
        }
        fclose(fileAlimentationInventory);
    }
    else if (mode == 1)
    {
        
        FILE *fileAlimentationInventory = fopen("../files/alimentationInventory.txt", "r");

        if(fileAlimentationInventory == NULL){
            printf(RED, "No se pude abrir el archivo");
        }

        char linea[50]; // Buffer para almacenar la línea leída
       
        // Leer y procesar todas las líneas del archivo
        fflush(stdin);
        while (fgets(linea, sizeof(linea), fileAlimentationInventory) != NULL)
        {
            fflush(stdin);
            char itemName[20], foodType[20];
            int quantity, price;
            
            // Utilizando sscanf para extraer palabras individuales de la línea
            sscanf(linea, "%s %u %u %s", itemName, &quantity, &price, foodType);
            root = insertNode(root, itemName, quantity, price, foodType);
        }
    }
    else
    {
        // elimina todo lo anterior del archivo
        FILE *fileAlimentationInventory = fopen("../files/alimentationInventory.txt", "w");
        fclose(fileAlimentationInventory);
    }
}

// Implementacion de arboles Binarios mediante un sistema de gestion de alimentacion
void alimentation(struct dataStateBars **ptrDataStateBars, struct walletData **ptrWalletData)
{
    struct node *root = NULL; // se debe cambiar la ubicacion de la declaracion del nodo
    char option;

    char productName[50];
    int unsigned quantity;
    int unsigned price;
    char foodType[20];

    saveReadInventoryA(1, root); // carga el arbol desde el archivo

    do
    {
        fflush(stdin);
        system("cls");

        printf(CYAN "------- MENU -------\n");
        printf("Billetera: %u\n", (*ptrWalletData)->coins);
        printf("1. Comprar producto\n");
        printf("2. Mostrar inventario\n");
        printf("3. Comer producto\n");
        printf("4. Salir\n");
        printf("\nPresione la tecla de la opcion que desea realizar..");
        printf(RESET);
        option = getch();
        fflush(stdin);

        switch (option)
        {
        case '1':
            system("cls");

            printf(CYAN "------- COMPRAR PRODUCTO -------\n");
            printf("Billetera: %u.\n", (*ptrWalletData)->coins);

            printf("Seleccione el tipo de comida:\n");
            printf("1. Chatarra\n");
            printf("2. Saludable\n");
            printf("\nPresione la tecla de la opcion que desea realizar..");
            printf(RESET);
            option = getch();
            fflush(stdin);

            printf("\n");

            switch (option)
            {
            case '1':
                printf(CYAN "------- CHATARRA -------\n");
                printf("Productos disponibles:\n");
                printf("1. Pizza ($10)\n");
                printf("2. Hamburguesa ($7)\n");
                printf("3. Pancho ($5)\n");
                printf(RESET);
                break;

            case '2':
                printf(CYAN "------- SALUDABLE -------\n");
                printf("Productos disponibles:\n");
                printf("1. Ensalada ($8)\n");
                printf("2. Frutas ($4)\n");
                printf("3. Yogurt ($6)\n");
                printf(RESET);
                break;

            default:
                printf(YELLOW "Opcion invalida.\n");
                printf(RESET);
                break;
            }

            printf("\n");

            printf(CYAN "Ingrese el numero del producto: ");
            scanf("%s", productName);
            printf("Ingrese la cantidad que desea comprar: ");
            scanf("%d", &quantity);
            printf(RESET);

            switch (option)
            {
            case '1':
                switch (productName[0])
                {
                case '1':
                    strcpy(productName, "Pizza");
                    price = 10;
                    strcpy(foodType, "Chatarra");
                    break;

                case '2':
                    strcpy(productName, "Hamburguesa");
                    price = 7;
                    strcpy(foodType, "Chatarra");
                    break;

                case '3':
                    strcpy(productName, "Pancho");
                    price = 5;
                    strcpy(foodType, "Chatarra");
                    break;

                default:
                    printf(YELLOW "Opcion invalida. \n");
                    printf(RESET);
                    break;
                }
                break;

            case '2':
                switch (productName[0])
                {
                case '1':
                    strcpy(productName, "Ensalada");
                    price = 8;
                    strcpy(foodType, "Saludable");
                    break;

                case '2':
                    strcpy(productName, "Frutas");
                    price = 4;
                    strcpy(foodType, "Saludable");
                    break;

                case '3':
                    strcpy(productName, "Yogurt");
                    price = 6;
                    strcpy(foodType, "Saludable");
                    break;

                default:
                    printf(YELLOW "Opcion invalida. \n");
                    printf(RESET);
                    break;
                }
                break;

                break;
            default:
                printf(YELLOW "Opcion invalida.\n");
                printf(RESET);
                break;
            }

            if (price * quantity > (*ptrWalletData)->coins)
            {
                printf(YELLOW "No tienes suficiente dinero para comprar este producto.\n");
                printf(RESET);
                break;
            }

            int unsigned availableQuantity = (*ptrWalletData)->coins / price;
            if (quantity > availableQuantity)
            {
                printf(YELLOW "No puedes comprar mas de %u productos con la cantidad de efectivo disponible.\n", availableQuantity);
                printf(RESET);
                break;
            }

            root = insertNode(root, productName, quantity, price, foodType);
            (*ptrWalletData)->coins -= price * quantity;
            break;

        case '2':
            system("cls");

            printf(CYAN "------- MOSTRAR INVENTARIO -------\n");
            showInventory(root);
            printf("\n");
            system("pause");
            printf(RESET);

            break;

        case '3':
            system("cls");
            fflush(stdin);

            printf(CYAN "------- INVENTARIO -------\n");
            showInventory(root);
            printf("--------------------------\n");

            printf("------- COMER PRODUCTO --------\n");

            printf("Ingrese el nombre del producto a comer: ");
            scanf("%s", productName);
            fflush(stdin);
            printf(RESET);

            struct node *node = searchNode(root, productName);
            if (node != NULL)
            {
                if (node->quantity > 1)
                {
                    node->quantity--;
                    Sleep(1000);
                }
                else
                {
                    // aca elimina la linea del producto
                    root = deleteNode(root, productName);
                    Sleep(1000);
                }

                // incremento y decremento de las barras
                if (strcmp(productName, "Ensalada") == 0)
                {
                    // ensalada h 15 s 3
                    (*ptrDataStateBars)->hungry += 15 * quantity;
                    (*ptrDataStateBars)->health += 3 * quantity;
                }
                else if (strcmp(productName, "Yogurt") == 0)
                {
                    // yogurt h 10 s 2
                    (*ptrDataStateBars)->hungry += 10 * quantity;
                    (*ptrDataStateBars)->health += 2 * quantity;
                }
                else if (strcmp(productName, "Frutas") == 0)
                {
                    // frutas h 6 s 1
                    (*ptrDataStateBars)->hungry += 6 * quantity;
                    (*ptrDataStateBars)->health += 1 * quantity;
                }
                else if (strcmp(productName, "Pizza") == 0)
                {
                    // Pizza h 20 s -1
                    (*ptrDataStateBars)->hungry += 20 * quantity;
                    (*ptrDataStateBars)->health -= 1 * quantity;
                }
                else if (strcmp(productName, "Hamburguesa") == 0)
                {
                    // Hamburguesa h 30 s -3
                    (*ptrDataStateBars)->hungry += 30 * quantity;
                    (*ptrDataStateBars)->health -= 3 * quantity;
                }
                else if (strcmp(productName, "Pancho") == 0)
                {
                    // Pancho h 25 s -2
                    (*ptrDataStateBars)->hungry += 25 * quantity;
                    (*ptrDataStateBars)->health -= 2 * quantity;
                }

                // no permite que la barra exceda los 100 puntos
                if ((*ptrDataStateBars)->health > 100)
                {
                    (*ptrDataStateBars)->health = 100;
                }

                if ((*ptrDataStateBars)->hungry > 100)
                {
                    (*ptrDataStateBars)->hungry = 100;
                }
            }
            else
            {
                printf("No tienes ese producto en tu inventario.\n");
                Sleep(1000);
            }

            break;

        case '4':
            printf(YELLOW "Saliendo del programa...\n");
            printf(RESET);
            break;
        default:
            printf(YELLOW "Opcion invalida.\n");
            printf(RESET);
            break;
        }

        printf("\n");
        system("pause");
        fflush(stdin);

    } while (option != '4');

    saveReadInventoryA(0, root);
}

// IMPLEMENTACION DE ARBOLES BINARIOS A INVENTARIO Y GESTION DE MEDICINAS

// Funcion para crear un nuevo nodo del arbol
struct product *createNodePr(char productName[], unsigned int quantity, unsigned int price)
{
    // Asignar memoria al nuevo nodo
    struct product *node = (struct product *)malloc(sizeof(struct product));

    // copiar los valores al nodo

    strcpy(node->productName, productName);
    node->quantity = quantity;
    node->price = price;

    // Inicialzar los punteros izquierdo y derecho como NULL
    node->left = NULL;
    node->right = NULL;

    // Devolver el nodo creado
    return node;
}

// Funcion para insertar un nodo en el arbol
struct product *insertNodePr(struct product *root, char productName[], unsigned int quantity, unsigned int price)
{
    // Si el arbol esta vacio, crear un nuevo nodo y establecerlo como raiz
    if (root == NULL)
    {
        return createNodePr(productName, quantity, price);
    }

    // comparar el nombre del product con el nombre en el nodo actual
    int comparison = strcmp(productName, root->productName);
    if (comparison < 0)
    {
        // Si el nombre es menor, insertar el nodo en el subarbol izquierdo
        root->left = insertNodePr(root->left, productName, quantity, price);
    }
    else if (comparison > 0)
    {
        // Si el nombre es mayor, insertar el nodo en el subarbol derecho
        root->right = insertNodePr(root->right, productName, quantity, price);
    }
    else
    {
        // Si el nombre es igual, incrementar la cantidad en el nodo actual

        root->quantity += quantity;
    }

    // Devolver el nodo raiz modificado
    return root;
}

// Funcion para buscar un nodo en el arbol
struct product *searchNodePr(struct product *root, char productName[])
{
    // Si el arbol esta vacio o el nombre del producto coincide, devolver el nodo actual
    if (root == NULL || strcmp(root->productName, productName) == 0)
    {
        return root;
    }

    // Comparar el nombre del producto con el nombre en el nodo actual
    int comparison = strcmp(productName, root->productName);
    if (comparison < 0)
    {
        // Si el nombre es menor, buscar en el subarbol izquierdo
        return searchNodePr(root->left, productName);
    }
    else
    {
        // Si el nombre es mayor buscar en el subarbol derecho
        return searchNodePr(root->right, productName);
    }
}

// Funcion para mostrar el inventario en orden
void showInventoryPr(struct product *root)
{
    if (root != NULL)
    {
        // recorrer el subarbol izquierdo
        showInventoryPr(root->left);

        // Mostrar los detalles del articulo en el nodo actual
        printf("Nombre: %s, Cantidad: %u, Precio: %u\n",
               root->productName, root->quantity, root->price);

        // Recorrer el subarbol derecho
        showInventoryPr(root->right);
    }
}

// Funcion auxiliar para encontrar el nodo con el valor minimo
struct product *findMinimumPr(struct product *root)
{
    struct product *current = root;
    while (current && current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

// Funcion para eliminar un nodo del arbol
struct product *deleteNodePr(struct product *root, char productName[])
{
    if (root == NULL)
    {
        return root;
    }

    int comparison = strcmp(productName, root->productName);

    if (comparison < 0)
    {
        root->left = deleteNodePr(root->left, productName);
    }
    else if (comparison > 0)
    {
        root->right = deleteNodePr(root->right, productName);
    }
    else
    {

        if (root->quantity > 1)
        {

            root->quantity--;
        }
        else
        {

            if (root->left == NULL)
            {
                struct product *temp = root->right;
                free(root);
                return (temp);
            }
            else if (root->right == NULL)
            {
                struct product *temp = root->left;
                free(root);
                return (temp);
            }

            struct product *temp = findMinimumPr(root->right);
            strcpy(root->productName, temp->productName);
            root->quantity = temp->quantity;
            root->price = temp->price;
            root->right = deleteNodePr(root->right, temp->productName);
        }
    }
    return root;
}

// Funcion para consumir medicamentos del inventario
int consumeMedicine(struct product *root, const char *productName, unsigned int quantity)
{
    if (root == NULL)
    {
        return 0;
    }

    struct product *current = root;
    while (current != NULL)
    {
        int cmp = strcmp(productName, current->productName); // se declara aqui por optimizacippon, asi si no se cumple la condicion
                                                             // no se usa memoria innecesariamente
        if (cmp == 0)
        {
            if (quantity <= current->quantity)
            {
                current->quantity -= quantity;

                if (current->quantity == 0)
                {
                    root = deleteNodePr(root, current->productName);
                }

                return 1;
            }
            else
            {
                return 0;
            }
        }
        else if (cmp < 0)
        {
            current = current->left;
        }
        else
        {
            current = current->right;
        }
    }
    return 7; // warning
}

// Funcion principal de inventario y gestion de medicamentos
void healing(struct dataStateBars **ptrDataStateBars, struct walletData **ptrWalletData, int **ptrSickPetStatus)
{
    struct product *root = NULL;
    char option;
    char subOption;
    char productName[50];
    unsigned int quantity;

    do
    {
        fflush(stdin);
        system("cls");

        printf(CYAN "------- MENU -------\n");
        printf("Billetera: %d\n", (*ptrWalletData)->coins);
        printf("1. Comprar medicamento\n");
        printf("2. Mostrar inventario\n");
        printf("3. Consumir medicamento\n");
        printf("4. Salir\n");
        printf("\nPresione la tecla de la opcion que desea realizar..");

        option = getchar();
        fflush(stdin);
        printf(RESET);

        switch (option)
        {
        case '1':
            system("cls");

            printf(CYAN "------- TIENDA DE MEDICAMENTOS -------\n");
            printf("Billetera: %d. \n", (*ptrWalletData)->coins);

            printf("Seleccione el tipo de medicamentos:\n");
            printf("1. Curitas (6 monedas)\n");
            printf("2. Pastillas (10 monedas)\n");
            printf("3. Inyeccion (50 monedas)\n");
            printf("\nSeleccione el medicamento: ");

            subOption = getchar();
            fflush(stdin);
            printf(RESET);

            switch (subOption)
            {
            case '1':
                printf(CYAN "------- CURITAS -------\n");
                printf("Ingrese la cantidad de curitas a comprar: ");
                scanf("%u", &quantity);
                printf(RESET);
                fflush(stdin);

                if (quantity * 6 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas\n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 6;
                    root = insertNodePr(root, "Curitas", quantity, 6);
                    printf(GREEN "Compra realizada con exito \n");
                    printf(RESET);
                    Sleep(1000);
                }

                break;
            case '2':
                printf(CYAN "------- PASTILLAS -------\n");
                printf("Ingrese la cantidad de pastillas a comprar: ");
                scanf("%u", &quantity);
                printf(RESET);
                fflush(stdin);

                if (quantity * 10 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas \n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 10;
                    root = insertNodePr(root, "Pastillas", quantity, 10);
                    printf(GREEN "Compra realizada con exito \n");
                    printf(RESET);
                    Sleep(1000);
                }

                break;
            case '3':
                printf(CYAN "------- INYECCION -------\n");
                printf("Ingrese la cantidad de inyecciones a comprar: ");
                scanf("%u", &quantity);
                printf(RESET);
                fflush(stdin);

                if (quantity * 50 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas \n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 50;
                    root = insertNodePr(root, "Inyeccion", quantity, 50);
                    printf(GREEN "Compra realizada con exito \n");
                    printf(RESET);
                    Sleep(1000);
                }
                break;
            default:
                printf(YELLOW "Opcion invalida\n");
                printf(RESET);
                Sleep(1000);
                break;
            }

            break;
        case '2':
            system("cls");

            printf(CYAN "------- INVENTARIO -------\n");
            showInventoryPr(root);
            printf("--------------------------\n");
            system("pause");
            printf(RESET);
            fflush(stdin);

            break;
        case '3':
            system("cls");

            printf(CYAN "------- INVENTARIO -------\n");
            showInventoryPr(root);
            printf("--------------------------\n");

            printf("------- CONSUMIR MEDICAMENTO -------\n");
            printf("Ingrese el nombre del medicamento: \n");
            fgets(productName, sizeof(productName), stdin);
            productName[strcspn(productName, "\n")] = '\0';

            printf("Ingrese la cantidad a consumir: ");
            scanf("%u", &quantity);
            printf(RESET);

            if (consumeMedicine(root, productName, quantity))
            {
                printf(GREEN "Medicamento consumido con exito.\n");

                printf(RESET);

                // hago el aumento de la barra aquí ya que es más fácil pasar los datos de la estructura
                // cuando se encuentra el elemento, dependiendo de que elemento sea y la cantidad consumida
                // se incrementa la barra de salud, SOLO FUNCIONA CON EL NOMBRE DEL PRODUCTO, NO CON EL CODIGO

                if (strcmp(productName, "Curitas") == 0)
                { // las curitas curan 5 por unidad
                    (*ptrDataStateBars)->health += 5 * quantity;
                }
                else if (strcmp(productName, "Pastillas") == 0)
                { // las pastillas curan 10 por unidad
                    (*ptrDataStateBars)->health += 10 * quantity;
                }
                else if (strcmp(productName, "Inyeccion") == 0)
                { // las inyecciones curan 25 por unidad y decrementan en 20 el animo
                    (*ptrDataStateBars)->health += 25 * quantity;
                    (*ptrDataStateBars)->mood -= 20 * quantity;
                    sickPet(2, &ptrSickPetStatus);
                }

                // no permite que la barra exceda los 100 puntos
                if ((*ptrDataStateBars)->health > 100)
                {
                    (*ptrDataStateBars)->health = 100;
                }
            }
            else
            {
                printf(YELLOW "No se encontro el medicamento en el inventario o la cantidad ingresada es invalida \n");
                printf(RESET);
            }

            Sleep(1000);

            break;
        case '4':
            printf(YELLOW "Saliendo de la tienda de medicamentos...\n");
            printf(RESET);
            break;
        default:
            printf(YELLOW "Opcion invalida.\n");
            printf(RESET);
            Sleep(1000);
            break;
        }

    } while (option != '4');
}

// determina el color de las barras dependiendo de su estado
// se pasa solo la PROPIEDAD del puntero. Ej: la propiedad ptrDataStateBars->health es un entero
void colorBar(int propety)
{
    if (propety >= 70)
    {
        printf(GREEN);
    }
    else if (propety <= 69 && propety >= 40)
    {
        printf(YELLOW);
    }
    else
    {
        printf(RED);
    }
}

//  dibuja las barras completas, lo que quiere decir que dibuja la barra en si y tambien su parte restante, como parametro
// recibe el valor de la barra que se quiere dibujar
void drawBars(int valueBar)
{
    int rest = 100;

    /* Ya que no se pudo agregar caracteres assci extendidos, lo que se hace es rellenar las barras con el
    caracter # en un determinado color dado por la funcion colorBar() y luego el resto de la barra se dibuja
    con el mismo caracter pero de color gris, asi el usuario entiende que la barra no esta llena*/

    // este for dibuja la barra
    colorBar(valueBar);
    for (int i = 0; i < (valueBar / 4); i++)
    {
        printf("#");
    }

    rest = (rest - valueBar) / 4; // calculo del excedente
    printf(GRAY);
    // este for dibuja el excedente de la barra
    for (int i = 0; i <= rest; i++)
    {
        // si rest es 100, se imprime una almohadilla de mas, por lo que se evita ese problema con el if
        if (i != rest)
        {
            printf("#");
        }
    }
    colorBar(valueBar); // se vuelve a llamar esta funcion para que se pueda ver la estadistica con el mismo color
    printf(" %i", valueBar);
    printf(GRAY "/100\n");

    printf(RESET);
}

// muestra finalmente las barras
void showStateBars(struct dataStateBars **ptrDataStateBars)
{
    // salud
    printf("Salud:    |");
    drawBars((*ptrDataStateBars)->health);

    // animo
    printf("Animo:    |");
    drawBars((*ptrDataStateBars)->mood);

    // hambre: para que sea mas correcto, el hambre realmente se entiende como saciedad, es decir, mientras
    // mas sasciedad, menos hambre, por ende lo que aumenta cuando se alimenta a la mascota no es el hambre,
    // si no la saciedad
    printf("Saciedad: |");
    drawBars((*ptrDataStateBars)->hungry);
}

// arroja frases random contenidas en el archivo "phrases.txt"
int randomPhrases(struct AssetsData *ptrAssetsData)
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

    printf(MAGENTA "\n%s dice: ", ptrAssetsData->petName);
    printf(GREEN "%s\n", phrase);
    printf(RESET);
    return 0;
}

// Muestra una pantalla de bienvenida al usuario cada vez que se inicia el programa
void splashScreen(struct AssetsData *ptrAssetsData)
{
    char line[M];

    printf(ORANGE);
    FILE *fileSplashScreen = fopen("../files/splashScreen.txt", "r");
    while (!feof(fileSplashScreen))
    {
        fgets(line, M, fileSplashScreen);
        printf("%s", line);
    }
    fclose(fileSplashScreen);
    printf(RESET "\n\n");
    printf(GREEN "%s esta muy feliz de verte de nuevo!!!\n", ptrAssetsData->petName);
    Sleep(5000);
    system("cls");
}

// muestra el avatar seleccionado por el usuario (informacion contenida en assets.txt)
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

// genera un nombre aleatorio para la mascota
// el numero aleatorio generado, sirve para establecer hasta que linea se recoore y por ende que frase dice la mascota
void randomPetName(char petName[])
{
    FILE *filePetNames = fopen("../files/petNames.txt", "r");
    int indexRandomMax;
    int index = 0;
    char optGenerate;

    do
    {
        srand(time(NULL));
        indexRandomMax = rand() % (162 - 1);
        // printf("\nEl valor aleatorio es: %d", indexRandomMax);

        while (index <= indexRandomMax)
        {
            fgets(petName, N, filePetNames);
            index++;
        }
        printf("El nombre generado aleatoriamente es: %s\n", petName);
        printf("Desea generar otro nombre? (s/n):\n");
        optGenerate = getch();
    } while (optGenerate != 'n' && optGenerate != 'N');

    fclose(filePetNames);
}

// permite modificar los ajustes del archivos assets, luego de ejecutada la funcion debe recargar la estructura "AssetsData"
// tiene dos modos, mode = 1 permite modificiar los valores / mode = 0 es para cargar los valores iniciales
int settings(int mode, struct AssetsData **ptrAssetsData)
{
    // almacenan temporalmente las preferencias actualizadas para posteriormente guardarlas definitivamente
    char petName[N];
    char userName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;

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
        printf("Dificultad del juego: %i\n", (*ptrAssetsData)->gameDifficult);

        fclose(fileAssets);

        do
        {
            printf(RED "Desea cambiar estos valores?(S/N): \n");
            optChange = getch();
        } while (optChange != 's' && optChange != 'n' && optChange != 'S' && optChange != 'N');
        printf(RESET);
    }

    // AGREGAR LA COMPROBACION DE DATOS
    if (optChange == 's' || optChange == 'S' || mode == 0)
    {
        do
        {

            printf(GREEN "Por favor, rellena los siguientes datos para tu mascota: \n");

            do
            {
                printf("Nombre del usuario (no puede ser mayor a 14 caracteres): ");
                scanf("%s", userName);
            } while (strlen(userName) > 14);

            do
            {
                printf("Nombre de la mascota (no puede ser mayor a 14 caracteres) (1 = Generar nombre de forma aleatoria): ");
                scanf("%s", petName);
            } while (strlen(petName) > 14 && petName[0] != '1');

            if (petName[0] == '1')
            {
                // se genera el nombre de forma aleatoria
                randomPetName(petName);
            }

            do
            {
                printf("Avatar seleccionado (0/1/2/3/4/5): ");
                scanf("%i", &gameAvatar);
            } while (gameAvatar != 0 && gameAvatar != 1 && gameAvatar != 2 && gameAvatar != 3 && gameAvatar != 4 && gameAvatar != 5 && gameAvatar != 6);

            do
            {
                printf("Inmortalidad de la mascota: (0 = no / 1 = si): ");
                scanf("%i", &petInmortality);
            } while (petInmortality != 0 && petInmortality != 1);

            do
            {
                printf("Dificultad del juego (0 = facil / 1 = medio / 2= dificil): ");
                scanf("%i", &gameDifficult);
            } while (gameDifficult != 0 && gameDifficult != 1 && gameDifficult != 2);

            fflush(stdin);
            printf(RED "SEGURO QUE DESEAS APLICAR LOS CAMBIOS? (S/N): \n");
            optChange = getch();
            printf(RESET);
        } while (optChange == 'n' || optChange == 'N');

        // se guardan las opciones
        if (optChange == 's' || optChange == 'S' || mode == 0)
        {
            FILE *fileAssets = fopen("../files/assets.txt", "w");
            if (fileAssets == NULL)
            {
                return 1;
            }
            fprintf(fileAssets, "%s\n%s\n%i\n%i\n%i", userName, petName, gameAvatar, petInmortality, gameDifficult);
            fclose(fileAssets);

            printf(GREEN "\nDatos guardados...\n");
            Sleep(200);
            system("cls");
        }
        else
        {
            printf(YELLOW "\nDatos descartados...\n");
        }
        printf(RESET);
    }
    printf(RESET);
    return 0;
}

// guarda o carga la cantidad de monedas del usuario
// mode = 1 guarda la informacion en el archivo wallet.txt, mode = 0 lee el contenido del archivo
int walletGetterAndSaver(int mode, struct walletData **ptrWalletData)
{
    if (mode)
    {
        // guarda la cantidad de monedas del usuario al momento de cerrar la sesion
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

        // printf("\nLa cantidad de monedas de la ultima sesion es: %i\n", (*ptrWalletData)->coins);
    }
    return 7; // warning
}

// determina en cuanto se deben decrementar las barras dependiendo del tiempo transcurrido y de
// la dificultad seleccionada por el usuario
void stateBarsDecrement(struct elpasedTime **ptrElpasedTime, struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData, int **ptrSickPetStatus)
{
    int decrementHealth;
    int decrementMood;
    int decrementHungry;

    // por cada hora transcurrida se decrementa un porcentaje dado a cada barra de estado

    if ((*ptrElpasedTime)->hours >= 1)
    { // para que comience a decrementar debe transcurrir minimo una hora
        if ((*ptrAssetsData)->gameDifficult == 0)
        { // facil
            decrementHealth = 2;
            decrementMood = 5;
            decrementHungry = 10;

            if (*ptrSickPetStatus == 1)
            {
                decrementHealth = 5;
            }
        }
        else if ((*ptrAssetsData)->gameDifficult == 1)
        { // medio
            decrementHealth = 5;
            decrementMood = 10;
            decrementHungry = 15;

            if (*ptrSickPetStatus == 1)
            {
                decrementHealth = 10;
            }
        }
        else
        { // dificl
            decrementHealth = 10;
            decrementMood = 15;
            decrementHungry = 25;

            if (*ptrSickPetStatus == 1)
            {
                decrementHealth = 20;
            }
        }

        // decrementa dependiendo de la dificultad que ha seleccionado el usuario
        // printf("\nLa cantidad de horas transcurridas fueron: %i\n", (*ptrElpasedTime)->hours);
        for (int i = 0; i < (*ptrElpasedTime)->hours; i++)
        {
            (*ptrDataStateBars)->health = (*ptrDataStateBars)->health - decrementHealth;
            (*ptrDataStateBars)->mood = (*ptrDataStateBars)->mood - decrementMood;
            (*ptrDataStateBars)->hungry = (*ptrDataStateBars)->hungry - decrementHungry;
        }
    }

}

// muestra una pantalla de muerte cuado la salud es <= 0
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
    printf("     (_/                                    \\_)\n\n");

    printf(RED "%s eres un irresponsable!! Dejaste que al pobre %s le diera un cortocircuito :(\n", ptrAssetsData->userName, ptrAssetsData->petName);
    printf(WHITE "%s ahora descansa en el", ptrAssetsData->petName);
    printf(GREEN " paraiso electronico...\n");
    printf(ORANGE "Como la mascota ha muerto, se reiniciaran todos los datos....\n");
    printf(RESET);
    system("pause");
}

// explica como usar y cuidar a la mascota en un texto desde un archivo txt externo
void tutorial()
{
    char line[200];
    FILE *fileTutorial = fopen("../files/tutorial.txt", "r");
    printf(GREEN);
    while (fgets(line, 200, fileTutorial) != NULL)
    {
        printf("%s", line); // Imprime la línea leída
    }
    printf(RESET);

    fclose(fileTutorial);

    system("pause");
}

// ejecuta los juegos seleccionados
void gameExecute(struct walletData **ptrWalletData, struct dataStateBars **ptrDataStateBars)
{
    char optGame;
    int reward = 0;
    do
    {
        system("cls");
        printf(CYAN "1. Tateti\n");
        printf("2. Trivia\n");
        printf("S. Salir\n");
        printf("ingrese el juego que desea jugar: \n");
        optGame = getch();
    } while (optGame != '1' && optGame != '2' && optGame != 's' && optGame != 'S');

    switch (optGame)
    {
    case '1':
        // Si se retorna un numero distinto de 0 entonces se actualiza el puntero de la barra de diversion
        reward = mainTikTakToe();

        if (reward != 0)
        {
            (*ptrDataStateBars)->mood += 10;
            if ((*ptrDataStateBars)->mood > 100)
            { // limita a  la barra
                (*ptrDataStateBars)->mood = 100;
            }
        }

        (*ptrWalletData)->coins += reward; // el juego retorna las monedas guardadas, por lo que luego solo se
                                           // suma lo ganado a la billetera
        break;
    case '2':
        reward = triviaMindmain();

        if (reward != 0)
        {
            (*ptrDataStateBars)->mood += 20;
            if ((*ptrDataStateBars)->mood > 100)
            {
                (*ptrDataStateBars)->mood = 100;
            }
        }

        (*ptrWalletData)->coins += reward/10;
        break;
    }

    reward = 0;
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

    // almacena el estado de enfermedad de la mascota
    int sickPetStatus = 0;
    int *ptrSickPetStatus = &sickPetStatus;

    int timeResult = lastOpenGetterAndSaver(0, timeNow);

    char keyOptMenu;

    // si es la primera vez que se abre el programa, pedira las confg iniciales
    if (firstTime(timeNow) != 0)
    {
        printf(GREEN, "Bienvenido al centro de adopción de mascotas virtuales.\n");
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

    stateBarsDecrement(&ptrElpasedTime, &ptrDataStateBars, &ptrAssetsData, &ptrSickPetStatus);

    walletGetterAndSaver(0, &ptrWalletData);

    sickPet(1, ptrSickPetStatus);
    // printf("\nEl valor de la variable sickPetStatus es %i\n", sickPetStatus);

    splashScreen(ptrAssetsData);

    printf(GREEN "Hola de nuevo %s!\n", ptrAssetsData->userName);
    printf(RESET);

    do
    {

        // si en las configuraciones la inmortalidad esta activada, la mascota no puede morir
        if (ptrDataStateBars->health <= 0)
        {
            if (ptrAssetsData->petInmortality != 1)
            {
                deathScreen(ptrAssetsData);

                // Cuando la mascota muere se setea en 1 el archivo firstOpen, asi la proxima que se abra el programa se
                // reescriben los datos
                FILE *fileFirstOpen = fopen("../files/firstOpen.txt", "w");
                fprintf(fileFirstOpen, "%i", 1);
                fclose(fileFirstOpen);

                lastOpenGetterAndSaver(1, timeNow); // cada vez
                return 1;
            }
            else
            {
                printf(RED "Tienes suerte de que %s sea inmortal...", ptrAssetsData->petName);
                system("pause");
            }
        }

        // cada vez que se ejecuta el bucle, se guardan los datos en los archivos
        timeNow = time(NULL);

        lastOpenGetterAndSaver(1, timeNow);
        stateBarsGetterAndSaver(1, &ptrDataStateBars);
        walletGetterAndSaver(1, &ptrWalletData);

        // printf("\n%s\n", ptrAssetsData->petName); //para probar si los datos se cargaron correctamente
        showAvatar(&ptrAssetsData);
        randomPhrases(ptrAssetsData);
        showStateBars(&ptrDataStateBars);

        printf(CYAN "\n1. Alimentar\n");
        printf("2. Curar\n");
        printf("3. Juegos\n");
        printf("4. Configuraciones\n");
        printf("0. Como cuidar la mascota\n");
        printf("S. Salir\n");
        printf(RESET);

        fflush(stdin);
        printf("\nPresione la tecla de la opcion que desea realizar..");
        keyOptMenu = getch();
        system("cls");

        if (keyOptMenu != 's' || keyOptMenu != 'S')
        {
            switch (keyOptMenu)
            {
            case '0':
                tutorial();
                system("cls");
                break;
            case '1':
                alimentation(&ptrDataStateBars, &ptrWalletData);
                system("cls");
                break;
            case '2':
                healing(&ptrDataStateBars, &ptrWalletData, &ptrSickPetStatus);
                system("cls");
                break;
            case '3':
                gameExecute(&ptrWalletData, &ptrDataStateBars);
                system("cls");
                break;
            case '4':
                settings(1, &ptrAssetsData);
                assetsLoad(&ptrAssetsData);
                system("cls");
                break;
            case 's':
            case 'S':
                break;
            default:
                printf(YELLOW "\nEscoje una opcion valida!!\n");
                printf(RESET);
            }
        }

    } while (keyOptMenu != 's' && keyOptMenu != 'S');

    // captura el instante de tiempo
    timeNow = time(NULL);

    lastOpenGetterAndSaver(1, timeNow);
    stateBarsGetterAndSaver(1, &ptrDataStateBars);
    walletGetterAndSaver(1, &ptrWalletData);

    system("pause");
    return 0;
}
