/*Prethodnom zadatku dodati funkcije :
A.dinamicki dodaje novi element iza odredenog elementa,
B.dinamicki dodaje novi element ispred odredenog elementa,
C.sortira listu po prezimenima osoba,
D.upisuje listu u datoteku,
E.cita listu iz datoteke.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Person {
    char firstName[50];
    char lastName[50];
    int birthYear;
    struct Person* next;
} Person;

//declaring functions
Person* createPerson(char* firstName, char* lastName, int birthYear);
Person* addToBeginning(Person* head, char* firstName, char* lastName, int birthYear);
Person* addToEnd(Person* head, char* firstName, char* lastName, int birthYear);
void printList(Person* head);
Person* findByLastName(Person* head, char* lastName);
Person* deleteByLastName(Person* head, char* lastName);
void freeList(Person* head);
Person* addAfter(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear);
Person* addBefore(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear);
Person* sortListByLastName(Person* head);
void saveToFile(Person* head, char* filename);
Person* loadFromFile(char* filename);

int main() {
    Person* head = NULL;

    head = addToBeginning(head, "Ivan", "Ivic", 2005);
    head = addToEnd(head, "Pero", "Peric", 2004);
    head = addAfter(head, "Ivic", "Ana", "Anic", 2006);
    head = addBefore(head, "Peric", "Marko", "Markic", 2003);

    printf("List after additions:\n");
    printList(head);

    head = sortListByLastName(head);
    printf("\nList after sorting by last name:\n");
    printList(head);

    saveToFile(head, "people.txt");

    freeList(head);
    head = loadFromFile("people.txt");
    printf("\nList loaded from file:\n");
    printList(head);

    freeList(head);
    return 0;
}

//creating a new person
Person* createPerson(char* firstName, char* lastName, int birthYear) {
    Person* newPerson = (Person*)malloc(sizeof(Person));
    if (!newPerson) {
        printf("Memory allocation failed!\n");
        exit(1);
    }
    strcpy(newPerson->firstName, firstName);
    strcpy(newPerson->lastName, lastName);
    newPerson->birthYear = birthYear;
    newPerson->next = NULL;
    return newPerson;
}

//adding a person to the beginning
Person* addToBeginning(Person* head, char* firstName, char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = head;
    return newPerson;
}

//adding a person to the end
Person* addToEnd(Person* head, char* firstName, char* lastName, int birthYear) {
    Person* newPerson = createPerson(firstName, lastName, birthYear);
    if (!head) return newPerson;

    Person* temp = head;
    while (temp->next) temp = temp->next;
    temp->next = newPerson;
    return head;
}

//printing every person in the list
void printList(Person* head) {
    if (!head) {
        printf("The list is empty.\n");
        return;
    }
    Person* temp = head;
    while (temp) {
        printf("%s %s, born %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

//finding a person by given last name
Person* findByLastName(Person* head, char* lastName) {
    Person* temp = head;
    while (temp) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

//deleting a person by given last namee
Person* deleteByLastName(Person* head, char* lastName) {
    Person* temp = head;
    Person* prev = NULL;

    while (temp && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (!temp) {
        printf("Person with last name '%s' not found.\n", lastName);
        return head;
    }

    if (!prev) head = temp->next;
    else prev->next = temp->next;

    free(temp);
    printf("Person '%s' deleted successfully.\n", lastName);
    return head;
}

//freeing the memory
void freeList(Person* head) {
    Person* current = head;
    while (current) {
        Person* temp = current;
        current = current->next;
        free(temp);
    }
}

//adding a person after a given person
Person* addAfter(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear) {
    Person* target = findByLastName(head, targetLastName);
    if (!target) {
        printf("Person with last name '%s' not found.\n", targetLastName);
        return head;
    }

    Person* newPerson = createPerson(firstName, lastName, birthYear);
    newPerson->next = target->next;
    target->next = newPerson;
    return head;
}

//adding a person before a given person
Person* addBefore(Person* head, char* targetLastName, char* firstName, char* lastName, int birthYear) {
    if (!head) return NULL;

    if (strcmp(head->lastName, targetLastName) == 0)
        return addToBeginning(head, firstName, lastName, birthYear);

    Person* prev = NULL;
    Person* current = head;

    while (current && strcmp(current->lastName, targetLastName) != 0) {
        prev = current;
        current = current->next;
    }

    if (!current) {
        printf("Person with last name '%s' not found.\n", targetLastName);
        return head;
    }

    Person* newPerson = createPerson(firstName, lastName, birthYear);
    prev->next = newPerson;
    newPerson->next = current;
    return head;
}

//sorting list by last name
Person* sortListByLastName(Person* head) {
    if (!head || !head->next) return head;

    int swapped;
    do {
        swapped = 0;
        Person* current = head;
        Person* prev = NULL;

        while (current->next) {
            if (strcmp(current->lastName, current->next->lastName) > 0) {
                swapped = 1;
                Person* tmp = current->next;
                current->next = tmp->next;
                tmp->next = current;

                if (!prev) head = tmp;
                else prev->next = tmp;

                prev = tmp;
            }
            else {
                prev = current;
                current = current->next;
            }
        }
    } while (swapped);

    return head;
}

//saving the list to a file
void saveToFile(Person* head, char* filename) {
    FILE* fp = fopen(filename, "w");
    if (!fp) {
        printf("Cannot open file '%s' for writing.\n", filename);
        return;
    }

    Person* temp = head;
    while (temp) {
        fprintf(fp, "%s %s %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
    fclose(fp);
    printf("List saved to file '%s'.\n", filename);
}

//loading a list from a file
Person* loadFromFile(char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Cannot open file '%s' for reading.\n", filename);
        return NULL;
    }

    Person* head = NULL;
    char firstName[50], lastName[50];
    int birthYear;

    while (fscanf(fp, "%49s %49s %d", firstName, lastName, &birthYear) == 3) {
        head = addToEnd(head, firstName, lastName, birthYear);
    }

    fclose(fp);
    return head;
}