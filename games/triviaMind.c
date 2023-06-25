#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGTH 100
#define AMOUNT 10

// estructura donde guardar de forma individual, la pregunta actual y su respuesta (0= falso, 1=verdadero)
struct question
{
    char question[MAX_LINE_LENGTH];
    int answer;
};

/*con esta funcion, separo en cada linea, las preguntas de las respuestas de cada linea leida;
con la funcion strtok separo una linea usando un delimitador*/
void separateQuestionsAnswers(char *line, struct question *question)
{
    char *token = strtok(line, ";");
    if (token != NULL)
    {
        strcpy(question->question, token);
        token = strtok(NULL, ",");
        if (token != NULL)
        {
            question->answer = atoi(token);
        }
    }
}

/*Funcion que genera un numero aleatorio dentro de un rango determinado*/
int generateRandomNumber(int min,int max){
return min + rand() % (max - min + 1);
}

int main(void)
{
    printf("---------------Trivia Mind-------------\n");    
    FILE *archive = fopen("../../files/triviaMind.csv","r"); // aca va la direccion del archivo

    if (archive == NULL)
    {
        printf("Error, no se pudo abrir el archivo seleccionado...\n");
        return 1;
    }

    // generacion de variables necesarias para la implementacion del conteo de puntos y preguntas
    struct question question[AMOUNT];
    int totalquestions = 0;
    char line[MAX_LINE_LENGTH];

    // separar linea actual y sumar 1 pregunta
    while (fgets(line, sizeof(line), archive))
    {
        separateQuestionsAnswers(line, &question[totalquestions]);
        totalquestions++;
    }

    fclose(archive);

    //permutar las preguntas aleatoriamente
    for (int i = totalquestions-1; i>0 ; i--)
    {
        int j= generateRandomNumber(0,i);
        struct question temp= question[i];
        question[i]=question[j];
        question[j]=temp;
    }


    //mostrar solo las primeras 10 preguntas permutadas
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
            points++;
        }else
        {
            printf("Respuesta incorrecta.\n");
        }
        printf("\n");

    }

    printf("\nPuntaje final: %d/%d\n", points, numQuestionsToShow);

    return 0;
}