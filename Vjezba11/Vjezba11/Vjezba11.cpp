/*Prepraviti zadatak 10 na nacin da se formira hash tablica drzava.Tablica ima 11 mjesta, a
funkcija za preslikavanje kljuc racuna da se zbraja ASCII vrijednost prvih pet slova drzave zatim
racuna ostatak cjelobrojnog dijeljenja te vrijednosti s velicinom tablice.Drzave s istim kljucem se
pohranjuju u vezanu listu sortiranu po nazivu drzave.Svaki cvor vezane liste sadrzi stablo
gradova sortirano po broju stanovnika, zatim po nazivu grada.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100
#define HASH_TABLE_SIZE 11

//struktura za grad u stablu
typedef struct CityTree {
    char name[MAX_NAME];
    int population;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

//struktura za državu u vezanoj listi
typedef struct CountryNode {
    char name[MAX_NAME];
    char filename[MAX_NAME];
    CityTree* cityTree;
    struct CountryNode* next;
} CountryNode;

//hash tablica drzava
typedef struct {
    CountryNode* buckets[HASH_TABLE_SIZE];
} HashTable;

//funkcija za izracun hash kljuca
int hashFunction(const char* name) {
    int sum = 0;
    int length = strlen(name);
    int limit = (length < 5) ? length : 5;

    for (int i = 0; i < limit; i++) {
        sum += (int)name[i];
    }

    return sum % HASH_TABLE_SIZE;
}

//stvaranje novog cvora grada
CityTree* createCityTreeNode(const char* name, int population) {
    CityTree* newNode = (CityTree*)malloc(sizeof(CityTree));
    strcpy(newNode->name, name);
    newNode->population = population;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//umetanje grada u stablo
CityTree* insertCityIntoTree(CityTree* root, const char* name, int population) {
    if (root == NULL) {
        return createCityTreeNode(name, population);
    }

    if (population < root->population) {
        root->left = insertCityIntoTree(root->left, name, population);
    }
    else if (population > root->population) {
        root->right = insertCityIntoTree(root->right, name, population);
    }
    else {
        //ako je populacija ista, sortira po nazivu
        if (strcmp(name, root->name) < 0) {
            root->left = insertCityIntoTree(root->left, name, population);
        }
        else {
            root->right = insertCityIntoTree(root->right, name, population);
        }
    }

    return root;
}

//stvaranje novog cvora drzave
CountryNode* createCountryNode(const char* name, const char* filename) {
    CountryNode* newNode = (CountryNode*)malloc(sizeof(CountryNode));
    strcpy(newNode->name, name);
    strcpy(newNode->filename, filename);
    newNode->cityTree = NULL;
    newNode->next = NULL;
    return newNode;
}

//umetanje drzave u vezanu listu
CountryNode* insertCountryIntoList(CountryNode* head, const char* name, const char* filename) {
    CountryNode* newNode = createCountryNode(name, filename);

    //umetanje na pocetak liste
    if (head == NULL || strcmp(name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    CountryNode* current = head;
    while (current->next != NULL && strcmp(current->next->name, name) < 0) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

//inicijalizacija hash tablice
void initHashTable(HashTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        table->buckets[i] = NULL;
    }
}

//umetanje drzave u hash tablicu
void insertCountryIntoHashTable(HashTable* table, const char* name, const char* filename) {
    int index = hashFunction(name);
    table->buckets[index] = insertCountryIntoList(table->buckets[index], name, filename);
}

//ucitavanje gradova iz datoteke u stablo drzave
void loadCitiesIntoTree(CountryNode* country) {
    FILE* file = fopen(country->filename, "r");
    if (file == NULL) {
        printf("Greska: Ne mogu otvoriti datoteku %s\n", country->filename);
        return;
    }

    char cityName[MAX_NAME];
    int population;

    while (fscanf(file, " %[^,], %d\n", cityName, &population) == 2) {
        country->cityTree = insertCityIntoTree(country->cityTree, cityName, population);
    }

    fclose(file);
}

//ucitavanje svih gradova za sve drzave u hash tablici
void loadAllCities(HashTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        CountryNode* current = table->buckets[i];
        while (current != NULL) {
            loadCitiesIntoTree(current);
            current = current->next;
        }
    }
}

//ispis stabla gradova
void printCityTree(CityTree* root) {
    if (root == NULL) {
        return;
    }

    printCityTree(root->left);
    printf("    %s, %d\n", root->name, root->population);
    printCityTree(root->right);
}

//ispis vezane liste drzava
void printCountryList(CountryNode* head) {
    CountryNode* current = head;
    while (current != NULL) {
        printf("  Drzava: %s\n", current->name);
        printf("    Gradovi:\n");
        printCityTree(current->cityTree);
        current = current->next;
    }
}

//ispis cijele hash tablice
void printHashTable(HashTable* table) {
    printf("HASH TABLICA DRZAVA\n\n");
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        printf("Bucket %d:\n", i);
        if (table->buckets[i] == NULL) {
            printf("  (prazno)\n");
        }
        else {
            printCountryList(table->buckets[i]);
        }
        printf("\n");
    }
}

//pretrazivanje gradova po minimalnoj populaciji
void searchCitiesInTree(CityTree* root, int minPopulation) {
    if (root == NULL) {
        return;
    }

    searchCitiesInTree(root->left, minPopulation);

    if (root->population > minPopulation) {
        printf("    %s, %d\n", root->name, root->population);
    }

    searchCitiesInTree(root->right, minPopulation);
}

//pronalaženje drzave u hash tablici
CountryNode* findCountryInHashTable(HashTable* table, const char* name) {
    int index = hashFunction(name);
    CountryNode* current = table->buckets[index];

    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }

    return NULL;
}

//oslobadanje memorije za stablo gradova
void freeCityTree(CityTree* root) {
    if (root == NULL) return;
    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);
}

//oslobadanje memorije za vezanu listu drzava
void freeCountryList(CountryNode* head) {
    while (head != NULL) {
        CountryNode* temp = head;
        head = head->next;
        freeCityTree(temp->cityTree);
        free(temp);
    }
}

//oslobadanje memorije za hash tablicu
void freeHashTable(HashTable* table) {
    for (int i = 0; i < HASH_TABLE_SIZE; i++) {
        freeCountryList(table->buckets[i]);
    }
}

int main() {
    FILE* file = fopen("drzave.txt", "r");
    if (file == NULL) {
        printf("Greska: Ne mogu otvoriti datoteku drzave.txt\n");
        return 1;
    }

    //inicijalizacija hash tablice
    HashTable hashTable;
    initHashTable(&hashTable);

    char countryName[MAX_NAME];
    char filename[MAX_NAME];

    //ucitavanje drzava iz datoteke u hash tablicu
    while (fscanf(file, " %s %s", countryName, filename) == 2) {
        insertCountryIntoHashTable(&hashTable, countryName, filename);
    }

    fclose(file);

    //ucitavanje svih gradova za sve drzave
    loadAllCities(&hashTable);

    //ispis hash tablice
    printHashTable(&hashTable);

    //pretrazivanje
    char searchCountry[MAX_NAME];
    int minPopulation;
    char choice;

    do {
        printf("Unesite naziv drzave: ");
        scanf("%s", searchCountry);

        printf("Unesite minimalnu populaciju: ");
        scanf("%d", &minPopulation);

        CountryNode* foundCountry = findCountryInHashTable(&hashTable, searchCountry);
        if (foundCountry != NULL) {
            printf("Gradovi u drzavi %s s populacijom > %d:\n", searchCountry, minPopulation);
            searchCitiesInTree(foundCountry->cityTree, minPopulation);
        }
        else {
            printf("Drzava nije pronadena.\n");
        }

        printf("\nPretrazivanje ponovno? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    //oslobadanje memorije
    freeHashTable(&hashTable);

    return 0;
}
