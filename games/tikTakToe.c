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
        resetBoard(board);
        playerMove(board);
        printBoard(board);
        
        fflush(stdin);
        printf("Desea volver a jugar: ");
        scanf("%c", &optAgain);
    }while(optAgain != 'N' && optAgain != 'n');
}

//muestra el tablero en pantalla
void printBoard(char board[][C]){
	printf(" %c | %c | %c ", board[0][0], board[0][1], board[0][2]);
	printf("\n---|---|---\n");
	printf(" %c | %c | %c ", board[1][0], board[1][1], board[1][2]);
	printf("\n---|---|---\n");
	printf(" %c | %c | %c ", board[2][0], board[2][1], board[2][2]);
	printf("\n");
    fflush(stdin);
}

//permite al jugador poner su "ficha"
void playerMove(char board[][C]){
	int i;
	int j;
	int band;

	do{
        band = 0;
        do{
            printf("Ingrese la fila: (1-3)");
		    scanf("%i", &i);
            if(i>3){
                printf("Valor inválido, ingrese un valor entre 1 y 3");
            }
        }while(i>3);
		i--;

        do{
            printf("Ingrese la columna (1-3)");
		    scanf("%i", &j);
            if(j>3){
                printf("Valor inválido, ingrese un valor entre 1 y 3");
            }
        }while(j>3);
		j--;
		
        //error de bucle infinito
		if(board[i][j] != ' '){
			printf("La casilla ya está ocupada\n");
		}else{
			board[i][j] = USER;
            band = 1;
		}
	} while (band == 0);
	fflush(stdin);
}

//reinicia todo el tablero
void resetBoard(char board[][C]){
	for(int i = 0; i < R; i++){
		for(int j = 0; j < C; j++){
			board[i][j] = ' ';
		}
	}
}