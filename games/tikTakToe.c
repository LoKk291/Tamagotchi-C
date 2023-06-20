#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <time.h>

#define USER 'X'
#define PET 'O'

#define R 3
#define C 3

int main(){
    //el campo del juego será una matriz
    char board[R][C];
    char optAgain;

    do{
        printf("Desea volver a jugar: ");
        scanf("%c", &optAgain);
    }while(optAgain != 'S' && optAgain != 's');
}
