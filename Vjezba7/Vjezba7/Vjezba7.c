/*Napisati program koji pomocu vezanih listi(stabala) predstavlja strukturu direktorija.
Omoguciti unos novih direktorija i pod - direktorija, ispis sadrzaja direktorija i
povratak u prethodni direktorij.Tocnije program treba preko menija simulirati
koristenje DOS naredbi : 1 - "md", 2 - "cd dir", 3 - "cd..", 4 - "dir" i 5 – izlaz.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NAME 50

typedef struct Directory {
    char name[MAX_NAME];
    struct Directory* firstChild;
    struct Directory* nextSibling;
    struct Directory* parent;
} Directory;

//pomocna funkcija za stvaranje direktorija
Directory* createDirectory(const char* name, Directory* parent) {
    Directory* newDir = (Directory*)malloc(sizeof(Directory));
    if (newDir == NULL) {
        printf("Greska pri alokaciji memorije!\n");
        return NULL;
    }

    strcpy(newDir->name, name);
    newDir->firstChild = NULL;
    newDir->nextSibling = NULL;
    newDir->parent = parent;

    return newDir;
}

//funkcija za dodavanje novog direktorija - 1
void addDirectory(Directory* parent, const char* name) {
    if (parent == NULL) {
        printf("Roditelj ne postoji!\n");
        return;
    }

    Directory* newDir = createDirectory(name, parent);
    if (newDir == NULL) return;

    if (parent->firstChild == NULL) {
        parent->firstChild = newDir;
    }
    else {
        Directory* current = parent->firstChild;
        while (current->nextSibling != NULL) {
            if (strcmp(current->name, name) == 0) {
                printf("Direktorij '%s' vec postoji!\n", name);
                free(newDir);
                return;
            }
            current = current->nextSibling;
        }
        if (strcmp(current->name, name) == 0) {
            printf("Direktorij '%s' vec postoji!\n", name);
            free(newDir);
            return;
        }
        current->nextSibling = newDir;
    }

    printf("Direktorij '%s' uspjesno kreiran.\n", name);
}

//funkcija za mijenjanje direktorija - 2
Directory* changeDirectory(Directory* current, const char* name) {
    if (current == NULL || current->firstChild == NULL) {
        printf("Direktorij '%s' ne postoji!\n", name);
        return current;
    }

    Directory* temp = current->firstChild;
    while (temp != NULL) {
        if (strcmp(temp->name, name) == 0) {
            return temp;
        }
        temp = temp->nextSibling;
    }

    printf("Direktorij '%s' ne postoji!\n", name);
    return current;
}

//funkcija za vracanje u roditeljski direktorij - 3
Directory* goToParent(Directory* current) {
    if (current == NULL) {
        printf("Vec ste u root direktoriju!\n");
        return current;
    }

    if (current->parent == NULL) {
        printf("Vec ste u root direktoriju!\n");
        return current;
    }

    return current->parent;
}

//funkcija za ispisivanje svih poddirektorija u direktoriju - 4
void listDirectory(Directory* current) {
    if (current == NULL) {
        printf("Direktorij ne postoji!\n");
        return;
    }

    printf("\nSadrzaj direktorija '%s':\n", current->name);

    if (current->firstChild == NULL) {
        printf("Prazan direktorij.\n");
    }
    else {
        Directory* temp = current->firstChild;
        while (temp != NULL) {
            printf("%s\n", temp->name);
            temp = temp->nextSibling;
        }
    }
}

//funkcija za ispisivanje trenutne putanje
void printPath(Directory* current) {
    if (current == NULL) return;

    if (current->parent != NULL) {
        printPath(current->parent);
    }
    printf("%s ", current->name);
}

//funkcija za izlaz iz svega - 5
void freeDirectoryTree(Directory* dir) {
    if (dir == NULL) return;

    freeDirectoryTree(dir->firstChild);
    freeDirectoryTree(dir->nextSibling);
    free(dir);
}

int main() {
    Directory* root = createDirectory("C:", NULL);
    Directory* current = root;
    int choice;
    char dirName[MAX_NAME];

    if (root == NULL) {
        printf("Greska pri inicijalizaciji root direktorija!\n");
        return 1;
    }

    while (1) {
        printf("\nTrenutna putanja: ");
        printPath(current);
        printf("\n");

        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Unesite ime novog direktorija: ");
            scanf("%s", dirName);
            addDirectory(current, dirName);
            break;

        case 2:
            printf("Unesite ime direktorija: ");
            scanf("%s", dirName);
            current = changeDirectory(current, dirName);
            break;

        case 3:
            current = goToParent(current);
            break;

        case 4:
            listDirectory(current);
            break;

        case 5:
            printf("Izlazim iz programa...\n");
            freeDirectoryTree(root);
            return 0;

        default:
            printf("Nepostojeca opcija! Pokusajte ponovno.\n");
        }
    }

    return 0;
}