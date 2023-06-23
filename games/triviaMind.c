#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_LENGHT 100
#define CANT 10

// estructura donde guardar de forma individual, la pregunta actual y su respuesta (0= falso, 1=verdadero)
struct question
{
    char question[MAX_LINE_LENGHT];
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

int main(void)
{
    printf("---------------Trivia Mind-------------\n");
    FILE *archive = fopen(); // aca va la direccion del archivo

    if (archive == NULL)
    {
        printf("Error, no se pudo abrir el archivo seleccionado...\n");
        return 1;
    }

    // generacion de variables necesarias para la implementacion del conteo de puntos y preguntas
    struct question question[CANT];
    int totalquestions = 0;
    char line[MAX_LINE_LENGHT];

    // separar linea actual y sumar 1 pregunta
    while (fgets(line, sizeof(line), archive))
    {
        separateQuestionsAnswers(line, &question[totalquestions]);
        totalquestions++;
    }

    fclose(archive);

    int points = 0;
    for (int i = 0; i < totalquestions; i++)
    {
        int answer;
        scanf("&d", &answer);

        if (answer == question[i].answer)
        {
            printf("¡Respuesta correcta!.\n");
        }
        else
        {
            printf("Respuesta incorrecta.\n");
        }
        printf("\n");
    }

    printf("\nPuntaje final: %d/%d\n", points, totalquestions);

    return 0;
}