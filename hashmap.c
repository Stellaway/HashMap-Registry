#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "hashmap.h"
#include "jatekosnyilvantarto.h"

//a hasito fuggveny, a hashmap alapja
unsigned long hash(char *str){
    int hash = 7001;
    int a = *str;
    while(*str++){
        hash += hash * 33 + a;
        a=*str;
    }

    return hash;
}

//inicializal egy ures tablat, argumentumkent megadott hellyel
void init_tabla(Tabla *tabla, long n){

    tabla->length = n;


    tabla->players = (Player**) malloc((tabla->length)*sizeof(Player*));
    for(long i=0; i<tabla->length; i++){
        tabla->players[i] = NULL;
    }
}

//segedfuggveny, megnoveli a tabla meretet a ketszeresere, megtartva az osszes elemet
void tabla_grow(Tabla **novelendo){
    Tabla *tabla = *novelendo;
    Tabla *uj=(Tabla*) malloc(sizeof(Tabla));

    init_tabla(uj, tabla->length*2);

    for(long i = 0; i < tabla->length; i++){
        Player *mozgo = tabla->players[i];
        while(mozgo != NULL){
            Player *belsomozgo = uj->players[hash(mozgo->name)%uj->length];

            //ha meg ures az elso lap
            if (belsomozgo == NULL) {

                uj->players[hash(mozgo->name)%uj->length] = mozgo;
                char *name = mozgo->name;
                mozgo = mozgo->kov;
                uj->players[hash(name)%uj->length]->kov = NULL;
                continue;
            }

            while(belsomozgo->kov != NULL){
                belsomozgo = belsomozgo->kov;
            }
            belsomozgo->kov = mozgo;
            belsomozgo=belsomozgo->kov;
            mozgo = mozgo->kov;
            belsomozgo->kov = NULL;
        }

    }
    //free(tabla);  <-- nekem errort okoz ennel az esetnel, az en forditom nem enged egy masik modulban foglalt helyet adott modulbol felszabaditani
    **novelendo = *uj;

}

//segedfuggveny, megmondja hogy szerepel-e a tablan a megadott nevu felhasznalo
bool is_on_list(Tabla *tabla, char key[]){

    Player *searched = tabla->players[hash(key)%tabla->length];

    while(searched!=NULL){

        if(strcmp(searched->name, key)==0){
            return true;
        }
        searched = searched->kov;
    }

    return false;
}

//visszaadja a tabla telitettseget szazalekban
int telitettseg(Tabla *tabla){
    long count=0;
    for (long i=0;i<tabla->length;i++){
        if (tabla->players[i]!=NULL){
            count++;
        }
    }
    return count*100/tabla->length;
}

//hozzaad a tablahoz egy jatekos adatsort
//visszaadja, hogy sikeres volt-e a hozzaadas
bool tabla_add(Tabla *tabla, Player *player){
    char *key = player->name;
    if(is_on_list(tabla,key)){

        return false;
    }
    player->kov = NULL;

    Player *mozgo = tabla->players[hash(key)%tabla->length];

    //ha meg nincs utkozes
    if(mozgo==NULL){
        tabla->players[hash(key)%tabla->length] = player;

        return true;
    }

    while(mozgo->kov != NULL){
        mozgo = mozgo->kov;
    }

    mozgo->kov = player;

    if(telitettseg(tabla)>70){
        tabla_grow(&tabla);
    }
    return true;

}

//visszaadja a nev alapjan keresett jatekost
Player *get_player(Tabla *tabla, char*key){

    Player *searched = tabla->players[hash(key)%tabla->length];

    while(searched!=NULL){

        if(strcmp(searched->name, key)==0){
            return searched;
        }
        searched = searched->kov;
    }
    return NULL;

}

//letorol a tablarol egy jatekost a neve alapjan
//visszaadja, hogy sikeres volt-e a torles
bool delete_player(Tabla *tabla, char*key){
    if (!is_on_list(tabla, key)){
        return false;//NULL;
    }
    Player *searched = tabla->players[hash(key)%tabla->length];


    //ha az elso helyen van
    if(strcmp(searched->name, key)==0){
        tabla->players[hash(key)%tabla->length]=searched->kov;
        searched->kov=NULL;
        free_entry(searched);
        return true;
    }

    Player *lemarado = NULL;


    while(strcmp(searched->name,key)!=0){
        lemarado = searched;
        searched = searched->kov;
    }

    lemarado->kov = searched->kov;
    searched->kov=NULL;
    free_entry(searched);

    return true;


}


//kimenti egy mar *megnyitott* fileba az argumentumkent megadott tablat
void to_file(Tabla *tabla, FILE *f){

    if (f==NULL){
        return;
    }

    fprintf(f,"JNY v1\n");

    for (int i=0; i<tabla->length; i++){

        Player *mozgo = tabla->players[i];
        while(mozgo != NULL){
            fprintf(f,  "%s`"
                        "%s`"
                        "%s`"
                        "%d\n", mozgo->name, mozgo->fave_game, mozgo->aranykopes, *mozgo->regisztralt);

            mozgo=mozgo->kov;
        }
    }

}
