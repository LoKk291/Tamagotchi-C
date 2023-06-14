#include <stdio.h>    
#include "libraries/colors.h"

int assetsLoad(){
    FILE* fileAssets = fopen("files/assets.txt", "r");
    if(fileAssets == NULL){
        printf("ERROR FATAL, no se ha podido abrir el archivo 'assets'\n");
        return 1;
    }
    return 0;
}

int main(){
    if(assetsLoad() != 0){
        return 1;
    }
    return 0;
}