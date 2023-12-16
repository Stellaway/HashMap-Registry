#ifndef HASHMAP_H
#define HASHMAP_H

#include <stdbool.h>
//a vodor egy lapja, mint Player struktúra
typedef struct Player{
    char *name, *fave_game, *aranykopes; time_t *regisztralt;
    struct Player *kov;

}Player;

//az egesz hashtabla, tartalmazva a jelenlegi hosszat
typedef struct Tabla{
    long length;
    Player **players;
}Tabla;

unsigned long hash(char *str);

void init_tabla(Tabla *tabla, long n);

bool is_on_list(Tabla *tabla, char key[]);

void free_tabla(Tabla*tabla);

void to_file(Tabla *tabla, FILE *f);

#endif
