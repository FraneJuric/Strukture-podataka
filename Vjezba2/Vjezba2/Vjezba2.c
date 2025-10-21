/*Definirati strukturu osoba (ime, prezime, godina rođenja) i napisati program koji:
A. dinamički dodaje novi element na početak liste,
B. ispisuje listu,
C. dinamički dodaje novi element na kraj liste,
D. pronalazi element u listi (po prezimenu),
E. briše određeni element iz liste,
U zadatku se ne smiju koristiti globalne varijable.*/

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

int main() {
    Person* head = NULL;

    head = addToBeginning(head, "Ivan", "Ivic", 2005);
    head = addToEnd(head, "Pero", "Peric", 2004);

    printf("List after adding elements:\n");
    printList(head);

    Person* found = findByLastName(head, "Ivic");
    if (found)
        printf("Found: %s %s, born %d\n", found->firstName, found->lastName, found->birthYear);
    else
        printf("Person not found.\n");

    head = deleteByLastName(head, "Peric");

    printf("List after deletion:\n");
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

    if (head == NULL)
        return newPerson;

    Person* temp = head;
    while (temp->next != NULL)
        temp = temp->next;

    temp->next = newPerson;
    return head;
}

//printing every person in the list
void printList(Person* head) {
    Person* temp = head;
    while (temp != NULL) {
        printf("%s %s, born %d\n", temp->firstName, temp->lastName, temp->birthYear);
        temp = temp->next;
    }
}

//finding a person by given last name
Person* findByLastName(Person* head, const char* lastName) {
    Person* temp = head;
    while (temp != NULL) {
        if (strcmp(temp->lastName, lastName) == 0)
            return temp;
        temp = temp->next;
    }
    return NULL;
}

//deleting a person by given last name
Person* deleteByLastName(Person* head, char* lastName) {
    Person* temp = head;
    Person* prev = NULL;

    while (temp != NULL && strcmp(temp->lastName, lastName) != 0) {
        prev = temp;
        temp = temp->next;
    }

    if (temp == NULL) {
        printf("Person with last name '%s' not found.\n", lastName);
        return head;
    }

    if (prev == NULL) {
        head = temp->next;
    }
    else {
        prev->next = temp->next;
    }

    free(temp);
    printf("Person '%s' deleted successfully.\n", lastName);
    return head;
}

//freeing the memory
void freeList(Person* head) {
    Person* current = head;
    while (current != NULL) {
        Person* temp = current;
        current = current->next;
        free(temp);
    }
}