#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <string.h>
#include "jatekosnyilvantarto.h"


int main(){

    Tabla *tabla= (Tabla*)malloc(sizeof(Tabla));
    if (tabla == NULL){
        print_hiba();
        printf("Kerjuk inditsa ujra a programot.");
        return 0;
    }
    init_tabla(tabla,32);

    ui_start(tabla);

    free(tabla);

    return 0;

}
