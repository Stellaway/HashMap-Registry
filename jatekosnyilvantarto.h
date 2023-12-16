#ifndef JATEKOSNYILVANTARTO_H
#define JATEKOSNYILVANTARTO_H

#include <time.h>
#include "hashmap.h"

void ui_start(Tabla *tabla);
void add_player(Tabla *tabla);
void view_player(Tabla *tabla);
void remove_player(Tabla *tabla);
void load_tabla(Tabla *tabla);
void save_tabla(Tabla *tabla);
bool program_exit(tabla);
bool from_file(Tabla *tabla, FILE *f);
void print_hiba();
void clearscreen();
void free_entry(Player *player);
void trim(char **s);
char *file_sort_beolvas(FILE *f);
char *sort_beolvas();


#endif
