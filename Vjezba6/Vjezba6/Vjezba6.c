#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Artikl {
    char naziv[50];
    int kolicina;
    float cijena;
    struct Artikl* next;
} Artikl;

typedef struct Racun {
    char datum[11];
    Artikl* artikli;
    struct Racun* next;
} Racun;

Racun* umetniRacunSortirano(Racun* glava, Racun* novi) {
    if (glava == NULL || strcmp(novi->datum, glava->datum) < 0) {
        novi->next = glava;
        return novi;
    }
    Racun* p = glava;
    while (p->next != NULL && strcmp(novi->datum, p->next->datum) > 0)
        p = p->next;

    novi->next = p->next;
    p->next = novi;
    return glava;
}

Artikl* umetniArtiklSortirano(Artikl* glava, Artikl* novi) {
    if (glava == NULL || strcmp(novi->naziv, glava->naziv) < 0) {
        novi->next = glava;
        return novi;
    }
    Artikl* p = glava;
    while (p->next != NULL && strcmp(novi->naziv, p->next->naziv) > 0)
        p = p->next;

    novi->next = p->next;
    p->next = novi;
    return glava;
}

Artikl* ucitajArtikle(char* imeDatoteke) {
    FILE* fp = fopen(imeDatoteke, "r");
    if (!fp) {
        return NULL;
    }

    char buffer[128];
    fgets(buffer, sizeof(buffer), fp);

    Artikl* glava = NULL;

    while (fgets(buffer, sizeof(buffer), fp)) {
        Artikl* a = (Artikl*)malloc(sizeof(Artikl));
        a->next = NULL;

        char naziv[50];
        int kolicina;
        float cijena;

        sscanf(buffer, "%[^,], %d, %f", naziv, &kolicina, &cijena);

        strcpy(a->naziv, naziv);
        a->kolicina = kolicina;
        a->cijena = cijena;

        glava = umetniArtiklSortirano(glava, a);
    }

    fclose(fp);
    return glava;
}

Racun* ucitajRacune() {
    FILE* fp = fopen("racuni.txt", "r");
    if (!fp) {
        return NULL;
    }

    Racun* glava = NULL;
    char imeDatoteke[50];

    while (fgets(imeDatoteke, sizeof(imeDatoteke), fp)) {
        imeDatoteke[strcspn(imeDatoteke, "\n")] = 0;

        FILE* fr = fopen(imeDatoteke, "r");
        if (!fr) {
            continue;
        }

        char datum[11];
        fgets(datum, sizeof(datum), fr);
        datum[strcspn(datum, "\n")] = 0;

        Racun* r = (Racun*)malloc(sizeof(Racun));
        strcpy(r->datum, datum);
        r->next = NULL;

        fclose(fr);

        r->artikli = ucitajArtikle(imeDatoteke);

        glava = umetniRacunSortirano(glava, r);
    }

    fclose(fp);
    return glava;
}

void upit(Racun* glava, char* artikl, char* odDatuma, char* doDatuma) {
    float ukupno = 0;
    int komada = 0;

    Racun* r = glava;
    while (r != NULL) {
        if (strcmp(r->datum, odDatuma) >= 0 && strcmp(r->datum, doDatuma) <= 0) {
            Artikl* a = r->artikli;
            while (a != NULL) {
                if (strcmp(a->naziv, artikl) == 0) {
                    ukupno += a->cijena * a->kolicina;
                    komada += a->kolicina;
                }
                a = a->next;
            }
        }
        r = r->next;
    }

    printf("\nRezultat:\n");
    printf("Artikl: %s\n", artikl);
    printf("Ukupno potroseno: %.2f\n", ukupno);
    printf("Ukupno kupljeno: %d komada\n", komada);
}

int main() {
    Racun* racuni = ucitajRacune();

    char artikl[50];
    char odDatum[11], doDatum[11];

    printf("Unesi naziv artikla: ");
    scanf(" %[^\n]", artikl);

    printf("Unesi pocetni datum (YYYY-MM-DD): ");
    scanf("%s", odDatum);

    printf("Unesi krajnji datum (YYYY-MM-DD): ");
    scanf("%s", doDatum);

    upit(racuni, artikl, odDatum, doDatum);

    return 0;
}