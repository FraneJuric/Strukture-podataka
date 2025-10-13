/*Napisati program koji prvo procita koliko redaka ima datoteka, tj.koliko ima studenata
zapisanih u datoteci.Nakon toga potrebno je dinamicki alocirati prostor za niz struktura
studenata(ime, prezime, bodovi) i ucitati iz datoteke sve zapise.Na ekran ispisati ime,
prezime, apsolutni i relativni broj bodova.
Napomena: Svaki redak datoteke sadrži ime i prezime studenta, te broj bodova na kolokviju.
relatvan_br_bodova = br_bodova / max_br_bodova * 100*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

#define MAX_POINTS 50

//defining a structure Student
typedef struct {
    char name[15];
    char surname[15];
    int points;
} Student;

//declaring functions
int lineNumber(char* fileName);
int allocateMemory(Student** students, int numberOfStudents);
int loadStudents(char* fileName, Student* students, int numberOfStudents);
int printStudents(Student* students, int numberOfStudents);

int main() {
    char fileName[50];
    int numberOfStudents = 0;
    Student* students = NULL;

    //user input of file name
    printf("Enter the file name: ");
    scanf("%s", fileName);

    numberOfStudents = lineNumber(fileName);
    if (numberOfStudents <= 0) {
        printf("Error.\n");
        return -1;
    }

    if (allocateMemory(&students, numberOfStudents) != 0) {
        printf("Error.\n");
        return -1;
    }

    if (loadStudents(fileName, students, numberOfStudents) != 0) {
        printf("Error.\n");
        free(students);
        return -1;
    }

    if (printStudents(students, numberOfStudents) != 0) {
        printf("Error.\n");
        free(students);
        return -1;
    }

    free(students);
    return 0;
}

//counting the number of lines (students)
int lineNumber(char* fileName) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
        return -1;

    int counter = 0;
    char buffer[40];

    while (fgets(buffer, sizeof(buffer), file) != NULL)
        counter++;

    fclose(file);
    return counter;
}

//allocating memory for all students
int allocateMemory(Student** students, int numberOfStudents) {
    *students = (Student*)malloc(numberOfStudents * sizeof(Student));
    if (*students == NULL)
        return -1;

    return 0;
}

//loading all students from the file
int loadStudents(char* fileName, Student* students, int numberOfStudents) {
    FILE* file = fopen(fileName, "r");
    if (file == NULL)
        return -1;

    for (int i = 0; i < numberOfStudents; i++) {
        int result = fscanf(file, "%s %s %d",
            students[i].name,
            students[i].surname,
            &students[i].points);
        if (result != 3) {
            fclose(file);
            return -1;
        }
    }

    fclose(file);
    return 0;
}

//printing all students and their points
int printStudents(Student* students, int numberOfStudents) {
    if (students == NULL || numberOfStudents <= 0)
        return -1;

    for (int i = 0; i < numberOfStudents; i++) {
        double relative = (double)students[i].points / MAX_POINTS * 100.0;
        printf("%-15s %-15s %-10d %-15.2f\n",
            students[i].name,
            students[i].surname,
            students[i].points,
            relative);
    }

    return 0;
}