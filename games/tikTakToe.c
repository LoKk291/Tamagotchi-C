#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>
#include "../libraries/colors.h"
#include <windows.h>

#define USER 'X'
#define PET 'O'

#define R 3
#define C 3

// muestra el tablero en pantalla
void printBoard(char board[][C])
{
    printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
    printf("\n");
    fflush(stdin);
}

// permite al jugador poner su "ficha"
void playerMove(char board[][C])
{
    int i;
    int j;
    int flag;

    do
    {
        flag = 0;
        do
        {
            printf("Ingrese la fila: (1-3): ");
            scanf("%i", &i);
            if (i > 3)
            {
                printf("Casilla invalida, ingrese una casilla entre 1 y 3");
            }
        } while (i > 3);
        i--;

        do
        {
            printf("Ingrese la columna (1-3): ");
            scanf("%i", &j);
            if (j > 3)
            {
                printf("Casilla inválida, ingrese una casilla entre 1 y 3");
            }
        } while (j > 3);
        j--;

        if (board[i][j] != ' ')
        {
            printf("La casilla ya esta ocupada\n");
        }
        else
        {
            board[i][j] = USER;
            flag = 1;
        }
    } while (flag == 0);
    fflush(stdin);
}

// permite a la computadora rellenar una casilla vacia, generando un numero aleatorio de casilla (i;j)
void computerMove(char board[][C], int freeSpaces)
{
    srand(time(NULL));
    int i;
    int j;

    // solo ejecuta el movimiento si hay algun espacio disponible
    if (freeSpaces > 0)
    {
        do
        {
            i = rand() % 3;
            j = rand() % 3;
        } while (board[i][j] != ' ');

        board[i][j] = PET;
    }
}

// reinicia todo el tablero (vacía las casillas)
void resetBoard(char board[][C])
{
    for (int i = 0; i < R; i++)
    {
        for (int j = 0; j < C; j++)
        {
            board[i][j] = ' ';
        }
    }
}

// chequea si alguna fila, columna, o diagonal fue relenada con el mismo
// caracter para determinar si  existe un ganador y quien gano
int checkWinner(char board[][C], int freeSpaces)
{
    int flagWinner = 0;
    char winner;

    // identifica quien es el ganador
    int whosWhinner = 0;

    // para coemnzar con la verificacion, primero se debe ver que la cantidad de espacios
    // sea <= 4, ya que esta es la cantidad de espacios minima que se ocupan para que haya un ganador
    if (freeSpaces <= 4)
    {
        // filas
        for (int i = 0; i < R; i++)
        {
            if (board[i][0] == board[i][1] && board[i][0] == board[i][2])
            {
                winner = board[i][0];
                flagWinner = 1;
            }
        }
        // columnas
        for (int i = 0; i < C; i++)
        {
            if (board[0][i] == board[1][i] && board[0][i] == board[2][i])
            {
                winner = board[0][i];
                flagWinner = 1;
            }
        }
        // diagonales
        if (board[0][0] == board[1][1] && board[0][0] == board[2][2])
        {
            winner = board[0][0];
            flagWinner = 1;
        }
        if (board[0][2] == board[1][1] && board[0][2] == board[2][0])
        {
            winner = board[0][2];
            flagWinner = 1;
        }
    }

    if (flagWinner)
    {
        if (winner == 'X')
        {
            whosWhinner = 1;
        }
        else
        {
            whosWhinner = 2;
        }
        // printf("\nHay un ganador, es %i\n", whosWhinner);
    }

    return whosWhinner;
}

// permite cargar 25 monedas al archivo wallet por cada partida ganada
int rewardCoins()
{
    int coins = 0;

    // abre el archivo y lee la cantidad de monedas
    FILE *fileWallet = fopen("../../files/wallet.txt", "r");
    fscanf(fileWallet, "%i", &coins);
    fclose(fileWallet);
    // printf("\nLa cantidad de monedas es: %i\n", coins);

    coins += 25; // si el jugador gana, se le dan 25 monedas como recompensa

    FILE *fileWalletW = fopen("../../files/wallet.txt", "w");
    fprintf(fileWalletW, "%i", coins);
    fclose(fileWalletW);

    printf(GREEN "\nFelicidades, ganaste 25 monedas, no la uses en tonterias!!!\n");
    printf(RESET);
    return coins;
}

// le coloco este nombre para poder ejecutarlo desde el main sin que ocurran problemas
int mainTikTakToe()
{
    // el campo del juego será una matriz
    char board[R][C];
    char optAgain;
    int freeSpaces;
    int userMoves;
    int coins;

    freeSpaces = 9;
    userMoves = 0;
    resetBoard(board);
    while (freeSpaces != 0 && (checkWinner(board, freeSpaces) == 0))
    {
        playerMove(board);
        userMoves++;
        freeSpaces--;
        if (!(userMoves > 3))
        {
            computerMove(board, freeSpaces);
            freeSpaces--;
        }

        printBoard(board);
    }

    if (checkWinner(board, freeSpaces) == 1)
    {
        printf(GREEN "\nHAS GANADO!!!\n");
        printf(RESET);
        coins = rewardCoins();
    }
    else
    {
        coins = 0;
        printf(YELLOW "\nHas perdido, suerte la proxima..\n");
        printf(RESET);
    }
    Sleep(1000);
    fflush(stdin);

    // ELIMINAR EL BUCLE QEU PERMITE VOLER A JUGAR PARA EVITAR BUGS

    return coins;
}