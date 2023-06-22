#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGHT 100
#define CANT 10

//estructura donde guardar de forma individual, la pregunta actual y su respuesta (0= falso, 1=verdadero)
struct question{
    char question[MAX_LINE_LENGHT];
    int answer;
}

/*con esta funcion, separo en cada linea, las preguntas de las respuestas de cada linea leida; con la funcion strtok separo una linea usando un delimitador*/
void separateQuestionsAnswers(struct question* question){
    char *token =strtok(linea,";")
    if(token !=NULL){
        strcpy(question->question,token);
        token=strtok(NULL,",");
        if(token!=NULL){
            question->answer=atoi(token);
        }
    }

}


int main(void){
FILE *archive =fopen();//aca va la direccion del archivo

if(archive==NULL){
    printf("Error, no se pudo abrir el archivo seleccionado...\n");
    return(1);
}

//generacion de cantidad de preguntas
struct question question[CANT];

printf("---------------Trivia Mind-------------\n");



return 0;
}
