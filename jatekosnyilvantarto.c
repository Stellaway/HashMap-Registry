#include "debugmalloc.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "hashmap.h"

#include "jatekosnyilvantarto.h"


void ui_start(Tabla *tabla) {

    while (true)
    {

        clearscreen();
        printf("Udv a Jatekosnyilvantarto v1.0 verzioju megvalositasaban. Mi osszegyujtjuk a jatekosat!\n\n");

        printf("\n\nKerjuk valasszon egy muveletet a hozzarendelt szammal:\n");
        printf("\t1. Jatekos hozzaadasa\n"
               "\t2. Jatekos megtekintese\n"
               "\t3. Jatekos torlese\n"
               "\t4. Jatekostabla betoltese\n"
               "\t5. Jatekostabla mentese\n"
               "\t6. Kilepes\n"
               "...");



        int command;
        if(scanf(" %d", &command)==0) {
            printf("Baj");
            char*c;
            while((c = getchar()) != '\n' && c != EOF);
            continue;
        }

        getchar();

        switch (command)
        {

        case 1:
            add_player(tabla);
            break;

        case 2:
            view_player(tabla);
            break;

        case 3:
            remove_player(tabla);
            break;

        case 4:
            load_tabla(tabla);
            break;

        case 5:
            save_tabla(tabla);
            wait4menu();
            break;

        case 6:
            if (program_exit(tabla)){
                return;
            }
            break;


        //rossz szamot irt be
        default:
            printf("Ismeretlen vezerlokod, %d", command);
            wait4menu();
        }
    }
}

void add_player(Tabla *tabla){

    Player *player=(Player *) malloc(sizeof(Player));
    if(player==NULL){
        print_hiba();
        return;
    }
    //fflush(stdin);
    printf("\n\nKerem adja meg a jatekosnevet... ");

    char *name = sort_beolvas();

    if(name==NULL){
        print_hiba();
        wait4menu();
        return;
    }
    trim(&name);
    if (is_on_list(tabla, name)){
        printf("Mar szerepel a megadott jatekos a nyilvantartoban..\n");
        wait4menu();
        free(player);
        free(name);
        return;
    }

    //fflush(stdin);
    printf("\nKerem adja meg a megadott jatekos kedvenc jatekat...");
    char *fave = sort_beolvas();

    //fflush(stdin);
    printf("\nKerem adja meg a jatekos egy aranykopeset, szellemes felszolalasat, eletfilozofiajat, esetleg kedvenc Friends idezetet...");
    char *quote = sort_beolvas();


    time_t *t = (time_t*)malloc(sizeof(time_t));
    if (t==NULL){
        print_hiba();
        wait4menu();
        return;
    }
    time(t);

    if(fave == NULL || quote == NULL){
        print_hiba();
        wait4menu();
        return;
    }

    player->name = name;
    player->fave_game = fave;
    player->aranykopes = quote;
    player->regisztralt = t;
    player->kov=NULL;

    tabla_add(tabla, player);

    printf("Sikeresen hozzaadva!\n  \n\n");
    wait4menu();

}

void view_player(Tabla *tabla){
    //fflush(stdin);
    printf("Kerem adja meg a megtekinteni kivant jatekos felhasznalonevet...\n");
    char *name = sort_beolvas();
    if (name==NULL){
        print_hiba();
        wait4menu();
    }
    trim(&name);

    Player *keresett = get_player(tabla, name);
    free(name);
    if(keresett==NULL){
        printf("%s", "Nem letezik ilyen nevu jatekos.\n");
        wait4menu();
        return;
    }

    printf("A jatekos adatai:\n\n"
           "\tFelhasznalo neve: %s\n"
           "\tKedvenc jateka: %s\n"
           "\tAranykopese/kedvenc idezete: %s\n"
           "\tAdatbazisunkba regisztralt: %s", keresett->name, keresett->fave_game, keresett->aranykopes, ctime(keresett->regisztralt));


    wait4menu();


}

void remove_player(Tabla *tabla){
    //fflush(stdin);
    printf("Kerem adja meg a torolni kivant jatekos felhasznalonevet...\n");
    char *name = sort_beolvas();
    if (name==NULL){
        print_hiba();
        wait4menu();
        return;
    }
    trim(&name);

    if(strcmp(tabla->players[hash(name)%tabla->length]->name,name)==0){
        Player *torol = tabla->players[hash(name)%tabla->length];
        tabla->players[hash(name)%tabla->length]=torol->kov;
        free_entry(torol);
        free(name);
        printf("Jatekos torolve.\n");
        wait4menu();
        return;
    }

    Player *torlendo = get_player(tabla,name);
    *name = "lol";
    if (torlendo==NULL){
        printf("Nincs ilyen nevu jatekos..\n");
        free(name);
        wait4menu();
        return;
    }

    free_entry(torlendo);
    free(name);
    printf("Jatekos torolve.\n");
    wait4menu();
}

void load_tabla(Tabla *tabla){
    printf("\nBetoltes.. Mi a betoltendo file neve? (kiterjesztessel egyutt, a program mappajaban kell lennie.)\n...");
    char *fname=sort_beolvas();
    if (fname==NULL){
        print_hiba();
        wait4menu();
        return;
    }

    FILE *f;
    f = fopen(fname, "r");

    if(f==NULL){
        printf("Nem sikerult a file megnyitasa.");
        wait4menu();
        return;
    }

    bool sikerult = from_file(tabla, f);
    printf("%s", sikerult ? "Sikerult a beolvasas." : "Nem sikerult a file beolvasasa.");



    free(fname);
    wait4menu();
}

void save_tabla(Tabla *tabla){
    printf("\nMentes.. Mi legyen a file neve? (kiterjesztessel egyutt, vigyazat, meglevo file-t felulirja, a program mappajaba ment.)\n...");
    char *fname=sort_beolvas();
    if (fname==NULL){
        print_hiba();
        wait4menu();
        return;
    }
    FILE *f;
    f=fopen(fname, "w");

    if(f==NULL){
        printf("A file megnyitasa nem sikerult");
        wait4menu();
        return;
    }
    to_file(tabla,f);
    free(fname);
    fclose(f);

}

bool program_exit(Tabla *tabla){
    printf("A nem mentett adatok elveszhetnek. Biztos benne, hogy kilep?\nY ha igen ... ");
    char c = getc(stdin);
    if (!(c=='y'|| c=='Y')){
        printf("\nRendben, nincs kilepes.");getchar();
        wait4menu();
        return false;
    }
    //fflush(stdin);
    printf("Kilepes..\n\n");
    free_tabla(tabla);
    return true;

}

//kepernyo torlest imital azzal, hogy 75 uj sort hoz letre, ez 1080p magas monitornal mukodik biztosan
void clearscreen(){
    for (int i = 0; i<75; i++){
        printf("\n");
    }
}

//var a legelso enterre amit hivas utan adunk az inputba, ezutan torli a console tartalmat
//nem ugrik sehova, csak menutomb vegen, visszaugras elotti varakozasra hasznalatos
void wait4menu(){
    printf("\nNyomja meg az entert a menube valo visszatereshez...");
    char*c;
    while((c = getchar()) != '\n' && c != EOF);
    clearscreen();
}

void print_hiba(void){
    printf("\nMemoriafoglalasi hiba...");
}

//felszabadit egy jatekost

void free_entry(Player *player){
    free(player->name);
    free(player->aranykopes);
    free(player->fave_game);
    free(player->regisztralt);
    free(player);
}

//felszabaditja a az egesz tablat
void free_tabla(Tabla *tabla){
    for (int i=0;i<tabla->length;i++){
        if(tabla->players[i]==NULL){
            continue;
        }

        Player *curr = tabla->players[i];
        Player *next = curr->kov;

        free_entry(curr);
        while(next!=NULL){
            curr=next;
            next=curr->kov;
            free_entry(curr);
        }

    }
}

//segedfuggveny a sorbeolvaso fuggvenyhez
char* seged_sort_beolvas(int n){
    char  c;
    if ((scanf("%c", &c) == EOF) || (c=='\n')){
        char* end = malloc((n+2)*sizeof(char));
        end[n] = '\0';

        return end;
    }
    char* sor = seged_sort_beolvas(n+1);
    sor[n]=c;
    return sor;

}

//az elozo fuggveny segitsegevel visszaad egy dinamikusan foglalt helyen mentett sor hosszú sztringet amit a konzolrol olvasott be enterig (de azt nem beleveve) vagy EOF-ig, a hivo dolga felszabaditani
//ha ures stringet olvasott be, kiterjeszti egy kotojelle
char *sort_beolvas(){
    char*sor= seged_sort_beolvas(0);
    if (sor[0]=='\0'){
        free(sor);
        sor = (char*)malloc(2*sizeof(char));
        sor[0]='-';
        sor[1]='\0';
    }
    return sor;

}

//ezt a trim fuggvenyt az infoc oldalarol vettem
void seged_trim(char *forras, char *cel) {
    int eleje = 0, vege = 0;

    /* elején a szóközök? */
    while (forras[eleje] != '\0' && forras[eleje] == ' ')
        eleje++;
    /* hol a vége? utána visszafelé a szóközökön */
    while (forras[vege] != '\0')
        vege++;
    vege--;
    while (vege >= 0 && forras[vege] == ' ')
        vege--;
    /* másolás az elejére, és lezárás */
    int i;
    for (i = 0; i <= vege-eleje; i++)
        cel[i] = forras[eleje+i];
    cel[i] = '\0';
}

//levagja a szokozoket a stringbol, letrehozva egy uj stringet kozben
void trim(char **s){
    char *uj =(char*) malloc((strlen(*s)+1)*sizeof(char));
    seged_trim(*s, uj);
    free(*s);
    *s=uj;
}

//beolvas es hozzaadja a megadott tablaba a megadott file-bol kiolvasott adatcsoportokat,
//miutan ellenorizte az elso sor alapjan, hogy a programhoz illo-e a file
//igazsagerteket ad vissza ez alapjan
bool from_file(Tabla *tabla, FILE *f){
    char s[7];
    fgets(s,7,f);fgetc(f);
    if (strcmp(s,"JNY v1")!=0){
        return false;
    }
    char *sor;
    while(sor = file_sort_beolvas(f)){
        Player *player = (Player*) malloc(sizeof(Player));

        //megszamolja, hogy mekkora stringekre lesz szukseg, es lefoglalja
        int n_nev, n_game, n_aranykopes;
        vesszokereso(sor,&n_nev,&n_game,&n_aranykopes);
        char*name = (char*) malloc(n_nev*sizeof(char));
        char*game = (char*) malloc(n_game*sizeof(char));
        char*aranykopes = (char*) malloc(n_aranykopes*sizeof(char));
        time_t *reg = (time_t*) malloc(sizeof(time_t));
        if(player==NULL||name==NULL||game==NULL||aranykopes==NULL||reg==NULL){
            continue;
        }
        long ido;
        sscanf(sor, "%[^`]`%[^`]`%[^`]`%d", name, game, aranykopes, &ido);
        trim(&name); trim(&game); trim(&aranykopes);
        *reg=ido;
        player->name=name;
        player->fave_game=game;
        player->aranykopes=aranykopes;
        player->regisztralt=reg;

        free(sor);

        if(!(tabla_add(tabla, player))){
            free(name); free(game); free(aranykopes); free(reg); free(player);
        }

    }
    return true;

}

//seged fuggveny file sor beolvasashoz
char* seged_file_sort_beolvas(FILE *f, int n){
    char  c;
    if ((fscanf(f,"%c", &c) == EOF) || (c=='\n')){
        if(n==0){
            return NULL;
        }
        char* end = malloc((n+1)*sizeof(char));
        end[n] = '\0';
        return end;
    }
    char* sor = seged_file_sort_beolvas(f,n+1);
    sor[n]=c;
    return sor;

}

//megadott filebol pontosan egy sort beolvas
char *file_sort_beolvas(FILE *f){
    char*sor= seged_file_sort_beolvas(f,0);
    return sor;

}

//megkeresi a repulo ekezeteket egy, a fileban levo sorban
void vesszokereso(char *str, int *elso, int *masodik, int *harmadik){
    int n=1;
    while((*str)!='`'){
        n++;
        str++;
    }
    *elso = n;
    str++; n=1;
    while(*str!='`'){
        n++;
        str++;
    }
    *masodik = n;
    str++; n=1;
    while(*str!='`'){
        n++;
        str++;
    }
    *harmadik=n;

}


