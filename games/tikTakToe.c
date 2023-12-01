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
    printf(ORANGE);
    printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
    printf("\n---|---|---\n");
    printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
    printf("\n");
    fflush(stdin);
    printf(RESET);
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

        // para que el juego sea más amigable con el usuario, se puede elegir las casillas empezando desde el 1 en vez del 0
        // validador de filas
        do
        {
            printf("Ingrese la fila donde quiere colocar la ficha (1-3): ");
            scanf("%i", &i);
            if (i < 1 || i > 3)
            {
                printf(YELLOW "Fila invalida, ingrese una casilla (1-3): ");
                printf(RESET);
            }
        } while (i < 1 && i > 3);
        i--;

        // validador de columnas
        do
        {
            printf("Ingrese la columna donde quiere colocar la ficha (1-3): ");
            scanf("%i", &j);
            if (j < 1 || j > 3)
            {
                printf(YELLOW "Columna invalida, ingrese una casilla (1-3): ");
                printf(RESET);
            }
        } while (j < 1 && j > 3);
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

    // para comenzar con la verificacion, primero se debe ver que la cantidad de espacios
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
    int freeSpaces = 9;
    int userMoves = 0;
    int coins = 0;

    printf(GREEN "TU eres X y la MASCOTA es O\n");
    printf(RESET);

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
    Sleep(3000);
    fflush(stdin);

    return coins;
}