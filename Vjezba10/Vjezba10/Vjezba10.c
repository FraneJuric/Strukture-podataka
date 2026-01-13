/*Napisati program koji cita datoteku drzave.txt u kojoj su zapisani nazivi pojedinih drzava. Uz
ime drzave u datoteci se nalazi i ime dodatne datoteke u kojoj se nalaze gradovi pojedine
drzave. Svaka datoteka koja predstavlja drzavu sadrži popis gradova u formatu naziv_grada,
broj_stanovnika.
a) Potrebno je formirati sortiranu vezanu listu drzava po nazivu. Svaki cvor vezane liste
sadrzi stablo gradova sortirano po broju stanovnika, zatim po nazivu grada.
b) Potrebno je formirati stablo drzava sortirano po nazivu. Svaki cvor stabla sadrzi vezanu
listu gradova sortiranu po broju stanovnika, zatim po nazivu grada.
Nakon formiranja podataka potrebno je ispisati drzave i gradove te omoguciti korisniku putem
tastature pretragu gradova odredene drzave koji imaju broj stanovnika veci od unosa na
tastaturi.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 100

typedef struct CityTree {
    char name[MAX_NAME];
    int population;
    struct CityTree* left;
    struct CityTree* right;
} CityTree;

typedef struct CityList {
    char name[MAX_NAME];
    int population;
    struct CityList* next;
} CityList;

typedef struct CountryWithTree {
    char name[MAX_NAME];
    char filename[MAX_NAME];
    CityTree* cityTree;
    struct CountryWithTree* next;
} CountryWithTree;

typedef struct CountryTree {
    char name[MAX_NAME];
    char filename[MAX_NAME];
    CityList* cityList;
    struct CountryTree* left;
    struct CountryTree* right;
} CountryTree;

CityTree* createCityTreeNode(const char* name, int population) {
    CityTree* newNode = (CityTree*)malloc(sizeof(CityTree));
    strcpy(newNode->name, name);
    newNode->population = population;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

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
        if (strcmp(name, root->name) < 0) {
            root->left = insertCityIntoTree(root->left, name, population);
        }
        else {
            root->right = insertCityIntoTree(root->right, name, population);
        }
    }

    return root;
}

CountryWithTree* createCountryWithTree(const char* name, const char* filename) {
    CountryWithTree* newNode = (CountryWithTree*)malloc(sizeof(CountryWithTree));
    strcpy(newNode->name, name);
    strcpy(newNode->filename, filename);
    newNode->cityTree = NULL;
    newNode->next = NULL;
    return newNode;
}

CountryWithTree* insertCountryIntoList(CountryWithTree* head, const char* name, const char* filename) {
    CountryWithTree* newNode = createCountryWithTree(name, filename);

    if (head == NULL || strcmp(name, head->name) < 0) {
        newNode->next = head;
        return newNode;
    }

    CountryWithTree* current = head;
    while (current->next != NULL && strcmp(current->next->name, name) < 0) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

void loadCitiesIntoTree(CountryWithTree* country) {
    FILE* file = fopen(country->filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", country->filename);
        return;
    }

    char cityName[MAX_NAME];
    int population;

    while (fscanf(file, " %[^,], %d\n", cityName, &population) == 2) {
        country->cityTree = insertCityIntoTree(country->cityTree, cityName, population);
    }

    fclose(file);
}

void printCityTree(CityTree* root) {
    if (root == NULL) {
        return;
    }

    printCityTree(root->left);
    printf("    %s, %d\n", root->name, root->population);
    printCityTree(root->right);
}

void printCountryList(CountryWithTree* head) {

    CountryWithTree* current = head;
    while (current != NULL) {
        printf("Country: %s\n", current->name);
        printf("  Cities:\n");
        printCityTree(current->cityTree);
        printf("\n");
        current = current->next;
    }
}

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

CityList* createCityListNode(const char* name, int population) {
    CityList* newNode = (CityList*)malloc(sizeof(CityList));
    strcpy(newNode->name, name);
    newNode->population = population;
    newNode->next = NULL;
    return newNode;
}

CityList* insertCityIntoList(CityList* head, const char* name, int population) {
    CityList* newNode = createCityListNode(name, population);

    if (head == NULL || population < head->population ||
        (population == head->population && strcmp(name, head->name) < 0)) {
        newNode->next = head;
        return newNode;
    }

    CityList* current = head;
    while (current->next != NULL &&
        (current->next->population < population ||
            (current->next->population == population && strcmp(current->next->name, name) < 0))) {
        current = current->next;
    }

    newNode->next = current->next;
    current->next = newNode;

    return head;
}

CountryTree* createCountryTreeNode(const char* name, const char* filename) {
    CountryTree* newNode = (CountryTree*)malloc(sizeof(CountryTree));
    strcpy(newNode->name, name);
    strcpy(newNode->filename, filename);
    newNode->cityList = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

CountryTree* insertCountryIntoTree(CountryTree* root, const char* name, const char* filename) {
    if (root == NULL) {
        return createCountryTreeNode(name, filename);
    }

    int cmp = strcmp(name, root->name);
    if (cmp < 0) {
        root->left = insertCountryIntoTree(root->left, name, filename);
    }
    else if (cmp > 0) {
        root->right = insertCountryIntoTree(root->right, name, filename);
    }

    return root;
}

void loadCitiesIntoList(CountryTree* country) {
    FILE* file = fopen(country->filename, "r");
    if (file == NULL) {
        printf("Error: Cannot open file %s\n", country->filename);
        return;
    }

    char cityName[MAX_NAME];
    int population;

    while (fscanf(file, " %[^,], %d\n", cityName, &population) == 2) {
        country->cityList = insertCityIntoList(country->cityList, cityName, population);
    }

    fclose(file);
}

void printCityList(CityList* head) {
    CityList* current = head;
    while (current != NULL) {
        printf("    %s, %d\n", current->name, current->population);
        current = current->next;
    }
}

void printCountryTree(CountryTree* root) {
    if (root == NULL) {
        return;
    }

    printCountryTree(root->left);
    printf("Country: %s\n", root->name);
    printf("  Cities:\n");
    printCityList(root->cityList);
    printf("\n");
    printCountryTree(root->right);
}

void searchCitiesInList(CityList* head, int minPopulation) {
    CityList* current = head;
    while (current != NULL) {
        if (current->population > minPopulation) {
            printf("    %s, %d\n", current->name, current->population);
        }
        current = current->next;
    }
}

CountryTree* findCountryInTree(CountryTree* root, const char* name) {
    if (root == NULL) {
        return NULL;
    }

    int cmp = strcmp(name, root->name);
    if (cmp == 0) {
        return root;
    }
    else if (cmp < 0) {
        return findCountryInTree(root->left, name);
    }
    else {
        return findCountryInTree(root->right, name);
    }
}

CountryWithTree* findCountryInList(CountryWithTree* head, const char* name) {
    CountryWithTree* current = head;
    while (current != NULL) {
        if (strcmp(current->name, name) == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

void loadAllCitiesInCountryTree(CountryTree* root) {
    if (root == NULL) {
        return;
    }
    loadAllCitiesInCountryTree(root->left);
    loadCitiesIntoList(root);
    loadAllCitiesInCountryTree(root->right);
}

void freeCityTree(CityTree* root) {
    if (root == NULL) return;
    freeCityTree(root->left);
    freeCityTree(root->right);
    free(root);
}

void freeCountryList(CountryWithTree* head) {
    while (head != NULL) {
        CountryWithTree* temp = head;
        head = head->next;
        freeCityTree(temp->cityTree);
        free(temp);
    }
}

void freeCityList(CityList* head) {
    while (head != NULL) {
        CityList* temp = head;
        head = head->next;
        free(temp);
    }
}

void freeCountryTree(CountryTree* root) {
    if (root == NULL) return;
    freeCountryTree(root->left);
    freeCountryTree(root->right);
    freeCityList(root->cityList);
    free(root);
}

int main() {
    FILE* file = fopen("drzave.txt", "r");
    if (file == NULL) {
        return 1;
    }

    CountryWithTree* countryList = NULL;

    CountryTree* countryTreeRoot = NULL;

    char countryName[MAX_NAME];
    char filename[MAX_NAME];

    while (fscanf(file, " %s %s", countryName, filename) == 2) {
        countryList = insertCountryIntoList(countryList, countryName, filename);

        countryTreeRoot = insertCountryIntoTree(countryTreeRoot, countryName, filename);
    }

    fclose(file);

    CountryWithTree* current = countryList;
    while (current != NULL) {
        loadCitiesIntoTree(current);
        current = current->next;
    }

    loadAllCitiesInCountryTree(countryTreeRoot);

    printCountryList(countryList);

    printCountryTree(countryTreeRoot);

    char searchCountry[MAX_NAME];
    int minPopulation;
    char choice;

    do {
        printf("Enter country name: ");
        scanf("%s", searchCountry);

        printf("Enter minimum population: ");
        scanf("%d", &minPopulation);

        CountryWithTree* foundCountryA = findCountryInList(countryList, searchCountry);
        if (foundCountryA != NULL) {
            printf("Cities in %s with population > %d:\n", searchCountry, minPopulation);
            searchCitiesInTree(foundCountryA->cityTree, minPopulation);
        }
        else {
            printf("Country not found.\n");
        }

        printf("\nSearch again? (y/n): ");
        scanf(" %c", &choice);

    } while (choice == 'y' || choice == 'Y');

    freeCountryList(countryList);
    freeCountryTree(countryTreeRoot);

    return 0;
}