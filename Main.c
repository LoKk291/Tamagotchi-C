#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "libraries/colors.h"
#include <windows.h>
#include <conio.h>
#include <stdbool.h>

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

// nodo del arbol binario
struct node
{
    char itemName[50];     // Nombre del artículo
    int unsigned quantity; // Cantidad del artículo
    int unsigned price;    // precio del artículo
    char foodType[20];     // Tipo de comida
    struct node *left;     // Puntero al subárbol izquierdo
    struct node *right;    // Puntero al subárbol derecho
};

//Estructura para representar los nodos del Ã¡rbol binario
struct product{
    char productName[50];   //Nombre del producto
    unsigned int quantity;  //Cantidad de unidades
    unsigned int price;     //Precio del producto(en monedas)
    struct product *left;   //Puntero al subÃ¡rbol izquierdo
    struct product *right;  //Puntero al subÃ¡rbol derecho
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

// la antigua funcion de alimentacion, se encuentra comentada, para así poder hacer uso de ciertas partes de su codigo, en la nueva funcion de alimentacion.
//  incrementa ptrDataStateBars->hungry dependiendo de la comida empleada (si es comida chatarra decrementa la barra de salud)
//  si la comida seleccionada es saludable incrementa un poco la salud
/*int alimentation(struct dataStateBars **ptrDataStateBars, struct AssetsData **ptrAssetsData)
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
}*/

// Funciones necesarias para la implementacion de arboles Binarios

// crea el nuevo nodo del arbol
struct node *createNode(char itemName[], int unsigned quantity, int unsigned price, char foodType[])
{
    // asignar memoria para el nuevo nodo
    struct node *node = (struct node *)malloc(sizeof(struct node));

    // copiar los valores al nodo
    strcpy(node->itemName, itemName);

    // Inicializar los punteros izquierdo y derecho como NULL
    node->left = NULL;
    node->right = NULL;

    // Devolver el nodo creado
    return node;
}

// Funcion para insertar un nodo en el árbol
struct node *insertNode(struct node *root, char itemName[], int quantity, int price, char foodType[])
{
    // si el árbol está vacío, crear un nuevo nodo y establecerlo como raíz
    if (root == NULL)
    {
        return createNode(itemName, quantity, price, foodType);
    }

    // Comparar el nombre del artículo con el nombre en el nodo actual
    if (strcmp(itemName, root->itemName) < 0)
    {
        // Si el nombre es menor, insertar el nodo en el subárbol izquierdo
        root->left = insertNode(root->left, itemName, quantity, price, foodType);
    }
    else if (strcmp(itemName, root->itemName) > 0)
    {
        // Si el nombre es mayor, insertar el nodo en el subárbol derecho
        root->right = insertNode(root->right, itemName, quantity, price, foodType);
    }
    else
    {
        // si el nombre es igual, incrementar la cantidad en el nodo actual
        root->quantity += quantity;
    }

    // devolver el nodo raíz modificado
    return root;
}

// Funcion para buscar un nodo en el árbol
struct node *searchNode(struct node *root, char itemName[])
{
    // Si el árbol está vacio o el nombre del artículo coincide, devolver el nodo actual
    if (root == NULL || strcmp(root->itemName, itemName) == 0)
    {
        return root;
    }

    // comparar el nombre del artículo con el nombre en el nodo actual
    if (strcmp(itemName, root->itemName) < 0)
    {
        // Si el nombre es menor, buscar en el subárbol izquierdo
        return searchNode(root->left, itemName);
    }
    else
    {
        // Si el nombre es mayor, buscar en el subárbol derecho
        return searchNode(root->right, itemName);
    }
}

// Funcion para mostrar el inventario en orden
void showInventory(struct node *root)
{
    if (root != NULL)
    {
        // Recorrer el subárbol izquierdo
        showInventory(root->left);

        // Mostrar los detalles del artículo en el nodo actual
        printf("Nombre: %s, Cantidad: %u, Precio: %u, Tipo: %s\n",
               root->itemName, root->quantity, root->price, root->foodType);

        // Recorrer el subárbol derecho
        showInventory(root->right);
    }
}

// Funcion auxiliar para encontrar el nodo con el valor mínimo
struct node *findMinimum(struct node *root)
{
    struct node *current = root;
    while (current && current->left != NULL)
    {
        current = current->left;
    }
    return current;
}

// Funcion para eliminar un nodo del árbol
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

// Implementacion de Árboles Binarios mediante un sistema de gestion de alimentacion
int alimentation()
{
    struct node *root = NULL;
    char option;

    // TIENE QUE RECIBIR COMO PARAMETRO LA VARIABLE COINS DE LA ESTRUCTURA WALLETS Y OPERAR CON ELLA, POSTERIORMENTE
    // EN CADA FUNCION QUE SE OCUPE WALLETS SE DEBE GUARDAR LAS MONEDAS QUE QUEDARON
    int unsigned wallet = 250;
    char productName[50];
    int unsigned quantity;
    int unsigned price;
    char foodType[20];

    do
    {
        system("cls");

        printf("------- MENU -------\n");
        printf("Billetera: %u\n", wallet);
        printf("1. Comprar producto\n");
        printf("2. Mostrar inventario\n");
        printf("3. COmer producto\n");
        printf("4. Salir\n");
        printf("Ingrese una opcion: ");
        option = getch();

        switch (option)
        {
        case '1':
            system("cls");

            printf("------- COMPRAR PRODUCTO -------\n");
            printf("Billetera: %u.\n", wallet);

            printf("Seleccione el tipo de comida:\n");
            printf("1. Chatarra\n");
            printf("2. Saludable\n");
            printf("Ingrese una opcion: ");
            option = getch();

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

            if (price * quantity > wallet)
            {
                printf("No tienes suficiente dinero para comprar este producto.\n");
                break;
            }

            // Consideracion: No se puede comprar mas productos de los que dispongas en cantidad de efectivo
            int unsigned availableQuantity = wallet / price;
            if (quantity > availableQuantity)
            {
                printf("No puedes comprar mas de %u productos con la cantidad de efectivo disponible.\n", availableQuantity);
                break;
            }

            root = insertNode(root, productName, quantity, price, foodType);
            wallet -= price * quantity;
            break;

        case '2':
            system("cls");

            printf("------- MOSTRAR INVENTARIO -------\n");
            showInventory(root);
            printf("\n");
            printf("Presione enter para continuar...");
            getchar();
            getchar();
            break;

        case '3':
            system("cls");

            printf("------- INVENTARIO -------\n");
            showInventory(root);
            printf("--------------------------\n");

            printf("------- COMER PRODUCTO --------\n");

            printf("Ingrese el nombre del producto a comer: ");
            scanf("%s", productName);

            struct node *node = searchNode(root, productName);
            if (node != NULL)
            {
                if (node->quantity > 1)
                {
                    node->quantity--;
                }
                else
                {
                    root = deleteNode(root, productName);
                }
            }
            else
            {
                printf("No tienes ese producto en tu inventario.\n");
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
        getchar();

    } while (option != '4');

    return 0;
}

//IMPLEMENTACION DE ARBOLES BINARIOS A INVENTARIO Y GESTION DE MEDICINAS
//por implementar: incremento o decremento de salud. [Encargado: David]

//Funcion para crear un nuevo nodo del árbol
struct product *createNodePr(char productName[], unsigned int quantity, unsigned int price){
    //Asignar memoria al nuevo nodo
    struct product *node =(struct product*)malloc(sizeof(struct product));

    //copiar los valores al nodo
    strcpy(node->productName, productName);
    node->quantity = quantity;
    node->price = price;

    //Inicialzar los punteros izquierdo y derecho como NULL
    node->left = NULL;
    node->right = NULL;

    //Devolver el nodo creado
    return node;

}

//Funcion para insertar un nodo en el árbol
struct product *insertNodePr(struct product *root, char productName[], unsigned int quantity, unsigned int price) {
    // Si el árbol está vacío, crear un nuevo nodo y establecerlo como raíz
    if(root ==NULL){
        return createNodePr( productName, quantity, price);
    }

    //comparar el nombre del product con el nombre en el nodo actual
    int comparison = strcmp(productName, root->productName);
    if (comparison < 0){
        //Si el nombre es menor, insertar el nodo en el subárbol izquierdo
        root->left = insertNodePr(root->left, productName, quantity, price);
    } else if (comparison >0){
        //Si el nombre es mayor, insertar el nodo en el subárbol derecho
        root->right = insertNodePr(root->right, productName, quantity, price);
    } else{
        //Si el nombre es igual, incrementar la cantidad en el nodo actual
        root->quantity += quantity;
    }

    //Devolver el nodo raíz modificado
    return root;
}


//Funcion para buscar un nodo en el árbol
struct product *searchNodePr(struct product *root, char productName[]){
    //Si el arbol está vacío o el nombre del producto coincide, devolver el nodo actual
    if (root == NULL || strcmp(root->productName, productName) == 0){
        return root;
    }

    //Comparar el nombre del producto con el nombre en el nodo actual
    int comparison = strcmp(productName, root->productName);
    if (comparison < 0){
        //Si el nombre es menor, buscar en el subárbol izquierdo
        return searchNodePr (root->left, productName);
    } else {
        //Si el nombre es mayor buscar en el subárbol derecho
        return searchNodePr (root->right, productName);
    }

}

//Funcion para mostrar el inventario en orden
void ShowInventoryPr(struct product *root) {
    if (root != NULL) {
        //recorrer el subárbol izquierdo
        ShowInventoryPr(root->left);

        //Mostrar los detalles del artículo en el nodo actual
        printf("Nombre: %s, Cantidad: %u, Precio: %u\n",
                root->productName,root->quantity, root->price);

        //Recorrer el subárbol derecho
        showInventoryPr(root->right);
    }
}

//Función auxiliar para encontrar el nodo con el valor mínimo
struct product *findMinimumPr(struct product *root){
    struct product *current = root;
    while (current && current ->left !=NULL){
        current = current->left;
    }
    return current;
}

//Funcion para eliminar un nodo del árbol
struct product *deleteNodePr(struct product *root, char productName[]) {
    if(root == NULL) {
        return root;
    }

    int comparison = strcmp (productName, root->productName);

    if(comparison < 0){
        root->left = deleteNodepr(root->left, productName);

    }else if (comparison > 0 ){
        root->right = deleteNodePr(root->right, productName);

    } else {
        
        if(root->quantity > 1){
            root->quantity--;

        } else {

            if (root->left == NULL){
                struct product *temp = root->right;
                free(root);
                return(temp); 

            } else if (root->right == NULL) {
                struct product *temp = root->left;
                free(root);
                return(temp);

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

//Funcion para consumir medicamentos del inventario
int consumeMedicine(struct product *root, const char *productName, unsigned int quantity ) {
    if(root == NULL){
        return 0;
    }

    struct product *current = root;
    while(current !=NULL){
        int cmp= strcmp (productName, current->productName);

        if(cmp == 0){
            if(quantity <= current->quantity){
                current->quantity -=quantity;

                if(current->quantity == 0){
                    root = deleteNodePr(root, current->productName);
                }

                return 1;

            }else{
                return 0;
            }
        
        } else if (cmp < 0){
            current = current ->left;
        } else {
            current = current -> right;
        }

    }

}

//Funcion principal de inventario y gestion de medicamentos
void healing(){
    struct product *root = NULL;
    //monedas de prueba hasta que david y yo implementemos el enlace de la variable wallet al archivo
    struct walletData myWallet;
    myWallet.coins = 100;
    char option;
    char subOption;
    char productName[50];
    unsigned int quantity;

    do{
        fflush(stdin);
        system("cls");

        printf("------- MENÚ -------\n");
        printf("Billetera: %d\n", myWallet.coins);
        printf("1.Comprar medicamento\n");
        printf("2.Mostrar inventario\n");
        printf("3. COnsumir medicament\n");
        printf("4. Salir\n");
        printf("Ingrese una opción\n");
        option = getchar();
        fflush(stdin);

        switch (option) {
        case '1':
            system ("cls");

            printf("------- TIENDA DE MEDICAMENTOS -------\n");
            printf("Billetera: %d. \n", myWallet.coins);

            printf("Seleccione el tipo de medicamentos:\n");
            printf("1. Curitas (6 monedas)\n");
            printf("2 Pastillas (10 monedas)\n");
            printf("3. Inyeccion (50 monedas\n");
            printf("Ingrese una opción: ");
            subOption = getchar();
            fflush(stdin);

            switch (subOption) {
            case '1':
                printf("------- CURITAS -------\n");
                printf("Ingrese la cantidad de curitas a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if(quantity *6 > myWallet.coins){
                    printf("No tienes suficientes monedas.\n");
                    Sleep(1000);
                } else {
                    myWallet.coins -= quantity *6;
                    root = insertNodePr(root,"Curitas",quantity,6);
                    printf("Compra realizada con éxito.\n");
                    Sleep(1000);
                }


                break;
            case '2':
                printf("------- PASTILLAS -------\n");
                printf("Ingrese la cantidad de pastillas a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if(quantity *10 > myWallet.coins){
                    printf("No tienes suficientes monedas.\n");
                    Sleep(1000);
                } else {
                    myWallet.coins -= quantity *10;
                    root = insertNodePr(root,"Pastillas",quantity,10);
                    printf("Compra realizada con éxito.\n");
                    Sleep(1000);
                }

                break;
            case '3':
                 printf("------- INYECCIÓN -------\n");
                printf("Ingrese la cantidad de inyecciones a comprar: ");
                scanf("%u", &quantity);
                fflush(stdin);

                if(quantity *50 > myWallet.coins){
                    printf("No tienes suficientes monedas.\n");
                    Sleep(1000);
                } else {
                    myWallet.coins -= quantity *50;
                    root = insertNodePr(root,"Pastillas",quantity,50);
                    printf("Compra realizada con éxito.\n");
                    Sleep(1000);
                }

                break;
            default:
                printf("Opción inválida.\n");
                Sleep(1000);
                break;
            }

            break;
        case '2':
            system("cls");

            printf("------- INVENTARIO -------\n");
            showInventoryPr(root);
            printf("--------------------------\n");
            print("Presione enter para continuar...");
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

            if (consumeMedicine(root, productName, quantity)) {
                printf("Medicamento consumido con éxito.\n");
            } else {
                printf("No se encontró el medicamento en el inventario o la cantidad ingresada es inválida.\n");
            }

            Sleep(1000);

        break;
        case '4':
            printf("Saliendo de la tienda de medicamentos...\n");
            break;
        default:
            printf("Opción invalida.\n");
            Sleep(1000);
            break;
        }

    }while (option != '4');


}


/*
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
*/

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
        if(i != rest){
            printf("#");
        }
    }
    colorBar(valueBar); // se vuelve a llamar esta funcion para que se pueda ver la estadistica con el mismo color
    printf(" %i", valueBar);
    printf(GRAY"/100\n");

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
    // mas sasciedad, menos hambre

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

// ejecuta los juegos seleccionados
// NO FUNCIONA
void gameExecute()
{
    char optGame;

    do
    {
        printf("Ingerese el juego que desea jugar: \n");
        optGame = getch();
    } while (optGame != 1 && optGame != 2);

    if (optGame == '1')
    {
        system("../games/output/triviaMind.exe");
    }
}

int main()
{
    SetConsoleCP(1252);
    SetConsoleOutputCP(1252); // posible solucion para que se reconozca el ascii extendido

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
                alimentation();
                break;
            case '2':
                healing(&ptrDataStateBars, &ptrAssetsData);
                break;
            case '3':
                gameExecute();
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
