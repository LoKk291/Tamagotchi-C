#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
MAX_LINE_LENGTH:represnta la longitud máxima de una linea en el archivo 
.csv que contiene las preguntas y respuestas
AMOUNT:indica la cantidad de preguntas que se mostraran en cada sesión.
*/
#define MAX_LINE_LENGTH 500
#define AMOUNT 10

// estructura donde guardar de forma individual, la pregunta actual y su respuesta (0= falso, 1=verdadero)
struct question
{
    char question[MAX_LINE_LENGTH];
    int answer;
};

/*con esta funcion, separo en cada linea, las preguntas de las respuestas de cada linea leida;
La funcion strlok divide la cadena en subcadenas utilizando un delimitaodr específico*/
void separateQuestionsAnswers(char *line, struct question *question)
{
    char *token = strtok((char *)line, ";");
    if (token != NULL)
    {
        strcpy(question->question, token);
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            //copio la pregunta en la estructura y convierto la respuesta a un entero utilizando atoi();
            question->answer = atoi(token);
        }
    }
}

/*Funcion que genera un numero aleatorio dentro de un rango determinado entre min y max*/
int generateRandomNumber(int min,int max){
return min + rand() % (max - min + 1);
}

int main(void)
{
    //utilizo la funcion time dentro de srand asi defino una semilla aleatoria distinta en cada ejecucion del programa
    srand(time(NULL));

    printf("---------------Trivia Mind-------------\n");    
   
    FILE *archive = fopen("../../files/triviaMind.csv","r"); // aca va la direccion del archivo

    if (archive == NULL)
    {
        printf("Error, no se pudo abrir el archivo seleccionado...\n");
        return 1;
    }

    // generacion de estructuras y variables necesarias para la implementacion del conteo de puntos y preguntas
    struct question question[AMOUNT];
    int totalquestions = 0;
    char line[MAX_LINE_LENGTH];

    // Leo las preguntas y respuestas del archivo y las almaceno en el arreglo 'question'
   
    while (fgets(line, sizeof(line), archive))
    {
        separateQuestionsAnswers(line, &question[totalquestions]);
        totalquestions++;
    }

    fclose(archive);

    //permutar las preguntas aleatoriamente 
    /*utilizo el algoritmo de Fisher-Yates:Comienzo en la ultima posicion del arreglo y en cada iteracion
    intercambio la pregunta actual con una pregunta aleatoria anterior a ella.*/
    for (int i = totalquestions-1; i>0 ; i--)
    {
        int j= generateRandomNumber(0,i);
        struct question temp= question[i];
        question[i]=question[j];
        question[j]=temp;
    }


    //mostrar solo las primeras 10 preguntas permutadas y solicitamos al usuario que ingrese sus respuestas
   
     /*operacio ternaria que asegura de que no se muestren mas preguntas de las disponibles en el archivo .csv;
    o, en todo caso, si hay menos preguntas, muestre todas las disponibles*/
    int numQuestionsToShow = (totalquestions<AMOUNT) ? totalquestions : AMOUNT;
    int points=0;
    for(int i =0;i<numQuestionsToShow;i++)
    {
        printf("Pregunta %d: %s\n", i+1, question[i].question);

        int answer;
        
        printf("Respuesta (0 = Falso, i = Verdadero): ");
        scanf("%d",&answer);

        if(answer == question[i].answer)
        {
            printf("¡Respuesta correcta!\n");
            points = points + 5;
        }else
        {
            printf("Respuesta incorrecta.\n");
            points = points - 5;
        }
        printf("\n");

    }

    //muuestra de puntaje final
    printf("\nPuntaje final: %d/%d\n", points, numQuestionsToShow);

    return 0;
}