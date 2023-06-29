#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

#define N 15
#define M 100

// contiene la informacion PRINCIPAL en tiempo de ejecucion
struct AssetsData
{
    // tanto el nombre de la mascota como de usuario seran de maximo 12 caracteres
    char userName[N];
    char petName[N];
    int gameAvatar;
    int petInmortality;
    int gameDifficult;
    int gameTryHard; // si esta opcion es verdadera, una vez que la mascota muera no se podra volver a jugar
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

// contiene las monedas del usuario, mas adelante agregare gemas
// utilizo una estructura porque en el futuro se agregar las gemas ya mencionadas
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
    unsigned int price;    // Precio del producto(en monedas)
    struct product *left;  // Puntero al subÃ¡rbol izquierdo
    struct product *right; // Puntero al subÃ¡rbol derecho
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

// Implementacion de arboles Binarios mediante un sistema de gestion de alimentacion
int alimentation(struct dataStateBars **ptrDataStateBars, struct walletData **ptrWalletData)
{
    struct node *root = NULL;
    char option;

    char productName[50];
    int unsigned quantity;
    int unsigned price;
    char foodType[20];

    do
    {
        fflush(stdin);
        system("cls");

        printf("------- MENU -------\n");
        printf("Billetera: %u\n", (*ptrWalletData)->coins);
        printf("1. Comprar producto\n");
        printf("2. Mostrar inventario\n");
        printf("3. Comer producto\n");
        printf("4. Salir\n");
        printf("Ingrese una opcion: ");
        option = getch();
        fflush(stdin);

        switch (option)
        {
        case '1':
            system("cls");

            printf("------- COMPRAR PRODUCTO -------\n");
            printf("Billetera: %u.\n", (*ptrWalletData)->coins);

            printf("Seleccione el tipo de comida:\n");
            printf("1. Chatarra\n");
            printf("2. Saludable\n");
            printf("Ingrese una opcion: ");
            option = getch();
            fflush(stdin);

            printf("\n");

            switch (option)
            {
            case '1':
                printf("------- CHATARRA -------\n");
                printf("Productos disponibles:\n");
                printf("1. Pizza ($10)\n");
                printf("2. Hamburguesa ($7)\n");
                printf("3. Pancho ($5)\n");
                break;

            case '2':
                printf("------- SALUDABLE -------\n");
                printf("Productos disponibles:\n");
                printf("1. Ensalada ($8)\n");
                printf("2. Frutas ($4)\n");
                printf("3. Yogurt ($6)\n");
                break;

            default:
                printf("Opcion invalida.\n");
                break;
            }

            printf("\n");

            printf("Ingrese el nombre del producto: ");
            scanf("%s", productName);
            printf("Ingrese la cantidad: ");
            scanf("%d", &quantity);

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
                    printf("Opcion invalida. \n");
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
                    printf("Opcion invalida. \n");
                    break;
                }
                break;

                break;
            default:
                printf("Opcion invalida.\n");
                break;
            }

            if (price * quantity > (*ptrWalletData)->coins)
            {
                printf("No tienes suficiente dinero para comprar este producto.\n");
                break;
            }

            // Consideracion: No se puede comprar mas productos de los que dispongas en cantidad de efectivo
            int unsigned availableQuantity = (*ptrWalletData)->coins / price;
            if (quantity > availableQuantity)
            {
                printf("No puedes comprar mas de %u productos con la cantidad de efectivo disponible.\n", availableQuantity);
                break;
            }

            root = insertNode(root, productName, quantity, price, foodType);
            (*ptrWalletData)->coins -= price * quantity;
            break;

        case '2':
            system("cls");

            printf("------- MOSTRAR INVENTARIO -------\n");
            showInventory(root);
            printf("\n");
            printf("Presione enter para continuar...");

            break;

        case '3':
            system("cls");
            fflush(stdin);

            printf("------- INVENTARIO -------\n");
            showInventory(root);
            printf("--------------------------\n");

            printf("------- COMER PRODUCTO --------\n");

            printf("Ingrese el nombre del producto a comer: ");
            scanf("%s", productName);
            fflush(stdin);

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
            printf("Saliendo del programa...\n");
            break;
        default:
            printf("Opcion invalida.\n");
            break;
        }

        printf("\n");
        printf("Presione enter para continuar...");
        getchar();
        fflush(stdin);

    } while (option != '4');

    return 0;
}

// IMPLEMENTACION DE ARBOLES BINARIOS A INVENTARIO Y GESTION DE MEDICINAS
// por implementar: incremento o decremento de salud. [Encargado: David]

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
}

// Funcion principal de inventario y gestion de medicamentos
void healing(struct dataStateBars **ptrDataStateBars, struct walletData **ptrWalletData)
{
    struct product *root = NULL;
    // monedas de prueba hasta que david y yo implementemos el enlace de la variable wallet al archivo
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
        printf("3. Consumir medicament\n");
        printf("4. Salir\n");
        printf("\nPresione la tecla de la opcion que desea realizar..");
        printf(RESET);

        option = getchar();
        fflush(stdin);

        switch (option)
        {
        case '1':
            system("cls");

            printf("------- TIENDA DE MEDICAMENTOS -------\n");
            printf("Billetera: %d. \n", (*ptrWalletData)->coins);

            printf("Seleccione el tipo de medicamentos:\n");
            printf("1. Curitas (6 monedas)\n");
            printf("2. Pastillas (10 monedas)\n");
            printf("3. Inyeccion (50 monedas\n");
            printf("\nPresione la tecla de la opcion que desea realizar..");
            subOption = getchar();
            fflush(stdin);

            switch (subOption)
            {
            case '1':
                printf("------- CURITAS -------\n");
                printf("Ingrese la cantidad de curitas a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if (quantity * 6 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas.\n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 6;
                    root = insertNodePr(root, "Curitas", quantity, 6);
                    printf(GREEN "Compra realizada con exito.\n");
                    printf(RESET);
                    Sleep(1000);
                }

                break;
            case '2':
                printf("------- PASTILLAS -------\n");
                printf("Ingrese la cantidad de pastillas a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if (quantity * 10 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas.\n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 10;
                    root = insertNodePr(root, "Pastillas", quantity, 10);
                    printf(GREEN "Compra realizada con exito.\n");
                    printf(RESET);
                    Sleep(1000);
                }

                break;
            case '3':
                printf("------- INYECCION -------\n");
                printf("Ingrese la cantidad de inyecciones a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if (quantity * 50 > (*ptrWalletData)->coins)
                {
                    printf(YELLOW "No tienes suficientes monedas.\n");
                    printf(RESET);
                    Sleep(1000);
                }
                else
                {
                    (*ptrWalletData)->coins -= quantity * 50;
                    root = insertNodePr(root, "Inyeccion", quantity, 50);
                    printf(GREEN "Compra realizada con exito.\n");
                    printf(RESET);
                    Sleep(1000);
                }
                break;
            default:
                printf(YELLOW "Opcion invalida.\n");
                printf(RESET);
                Sleep(1000);
                break;
            }

            break;
        case '2':
            system("cls");

            printf("------- INVENTARIO -------\n");
            showInventoryPr(root);
            printf("--------------------------\n");
            printf("Presione enter para continuar...");
            getchar();
            fflush(stdin);

            break;
        case '3':
            system("cls");

            printf("------- INVENTARIO -------\n");
            showInventoryPr(root);
            printf("--------------------------\n");

            printf("------- CONSUMIR MEDICAMENTO -------\n");
            printf("Ingrese el nombre del medicamento: \n");
            fgets(productName, sizeof(productName), stdin);
            productName[strcspn(productName, "\n")] = '\0';

            printf("Ingrese la cantidad a consumir: ");
            scanf("%u", &quantity);

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
                }

                // no permite que la barra exceda los 100 puntos
                if ((*ptrDataStateBars)->health > 100)
                {
                    (*ptrDataStateBars)->health = 100;
                }
            }
            else
            {
                printf(YELLOW "No se encontro el medicamento en el inventario o la cantidad ingresada es invalida.\n");
                printf(RESET);
            }

            Sleep(1000);

            break;
        case '4':
            printf("Saliendo de la tienda de medicamentos...\n");
            break;
        default:
            printf(YELLOW "Opcion invalida.\n");
            printf(RESET);
            Sleep(1000);
            break;
        }

    } while (option != '4');
}

// FIN DE LOS ARBOLES BINARIOS

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

// MODIFICAR PARA QUE SE VEA MAS ESTETICO
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

// genera un nombre aleatorio para la mascota
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
            printf(RED "Desea cambiar estos valores?(S/N): \n");
            optChange = getch();
        } while (optChange != 's' && optChange != 'n' && optChange != 'S' && optChange != 'N');
        printf(RESET);
    }

    // AGREGAR LA COMPROBACION DE DATOS
    if (optChange == 's' || optChange == 'S' || mode == 0)
    {
        if (mode)
        {
            printf("\nIngrese los nuevos valores que desea agregar:\n");
        }
        else
        {
            printf("\nIngrese los datos que desea:\n");
        }

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
            printf("Avatar seleccionado (0/1/2/3/4/5/6): ");
            scanf("%i", &gameAvatar);
        } while (gameAvatar != 0 && gameAvatar != 1 && gameAvatar != 2 && gameAvatar != 3 && gameAvatar != 4 && gameAvatar != 5 && gameAvatar != 6);

        do
        {
            printf("Inmortalidad de la mascota: (0 = no / 1 = si): ");
            scanf("%i", &petInmortality);
        } while (petInmortality != 0 && petInmortality != 1);

        do
        {
            printf("Dicifultad del juego (0 = facil / 1 = medio / 2= dificil): ");
            scanf("%i", &gameDifficult);
        } while (gameDifficult != 0 && gameDifficult != 1 && gameDifficult != 2);

        do
        {
            printf("Modo TryHard (0 = no / 1= si):");
            scanf("%i", &gameTryHard);
        } while (gameTryHard != 0 && gameTryHard != 1);

        fflush(stdin);
        printf(BLUE "SEGURO QUE DESEAS APLICAR LOS CAMBIOS? (s/n): \n");
        optChange = getch();

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
            Sleep(200);
            system("cls");
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
}

// determina en cuanto se deben decrementar las barras dependiendo del tiempo transcurrido y de
// la dificultad seleccionada por el usuario
void stateBarsDecrement(struct elpasedTime **ptrElpasedTime, struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData)
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
        }
        else if ((*ptrAssetsData)->gameDifficult == 1)
        { // medio
            decrementHealth = 5;
            decrementMood = 10;
            decrementHungry = 15;
        }
        else
        { // dificl
            decrementHealth = 10;
            decrementMood = 15;
            decrementHungry = 25;
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

// ejecuta los juegos seleccionados
// NO FUNCIONA
void gameExecute()
{
    /*char optGame;

    do
    {
        printf("ingrese el juego que desea jugar: \n");
        optGame = getch();
    } while (optGame != 1 && optGame != 2);

    if (optGame == '1')
    {
        system("../games/output/triviaMind.exe");
    }*/
}

/* dependiendo del numero generado, la mascota se enferma o no, si se enferma, se guarda uno en el archivo sick
ademas la funcion se encarga de leer el estado desde el archivo sickPet (si ya estaba enferma o no) y
guardar el nuevo valor en caso de que se enferme cuando se ejecute la enfermedad o que se cure con una vacuna
como el valor no es importante en todo momento de la ejecucion, no se crea una estructura para empaquetar el dato
si no que solo se crea un puntero en el main.
mode 0 = lee mode 1 = guarda*/
void sickPet(int mode, int *sickPetStatus)
{
    srand(time(NULL));
    int randNum = rand() % (99999);

    // si el numero generado, multiplicado por 2 y restandole 7, es divisible por 3, entonces la mascota se enferma
    randNum = (randNum * 2) - 7;
    // printf("\nEl numero random es: %i\n", randNum); //PRUEBAS
    // randNum = 123;

    // si el modo es 0 lee, si el modo es 1 guarda
    if (!mode)
    { // mode 0
        if (randNum % 3 == 0)
        {
            // printf("\nLa mascota se enferma\n");
            sickPetStatus = 1;
            FILE *fileSickPet = fopen("../files/sickPet.txt", "w");
            fprintf(fileSickPet, "%i", sickPetStatus);
            fclose(fileSickPet);
        }
    }
    else
    { // mode 1
        FILE *fileSickPet = fopen("../files/sickPet.txt", "r");
        fscanf(fileSickPet, "%i", sickPetStatus); // no uso el & porque sickPetStatus ya es un puntero
        fclose(fileSickPet);
    }
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

    sickPet(0, ptrSickPetStatus);
    // printf("\nEl valor de la variable sickPetStatus es %i\n", sickPetStatus);

    /*
    //esteregg "time since 1970"
    time_t since1970 = time(NULL);
    printf("\n%ld\n", since1970);
    */

    /*
    eastergg de sonido "thoy dont know me son"
    */

    // splashScreen();

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

        // cada vez que se ejecuta el bucle, se guardan los datos en los archivos
        timeNow = time(NULL);

        lastOpenGetterAndSaver(1, timeNow);
        stateBarsGetterAndSaver(1, &ptrDataStateBars);
        walletGetterAndSaver(1, &ptrWalletData);

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
        printf("\nPresione la tecla de la opcion que desea realizar..");
        keyOptMenu = getch();
        system("cls");

        if (keyOptMenu != 's' || keyOptMenu != 'S')
        {
            switch (keyOptMenu)
            {
            case 0:
                tutorial();
                system("cls");
                break;
            case '1':
                alimentation(&ptrDataStateBars, &ptrWalletData);
                system("cls");
                break;
            case '2':
                healing(&ptrDataStateBars, &ptrWalletData);
                system("cls");
                break;
            case '3':
                gameExecute();
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
