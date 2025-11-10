/*Napisati program koji iz datoteke cita postfiks izraz i zatim koristenjem stoga racuna
rezultat.Stog je potrebno realizirati preko vezane liste.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node {
    double value;
    struct Node* next;
} Node;

Node* createNode(double value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    if (!newNode) {
        printf("Memory allocation error!\n");
        exit(1);
    }
    newNode->value = value;
    newNode->next = NULL;
    return newNode;
}

Node* push(Node* top, double value) {
    Node* newNode = createNode(value);
    newNode->next = top;
    return newNode;
}

Node* pop(Node* top, double* value) {
    if (top == NULL) {
        printf("Error!\n");
        exit(1);
    }
    Node* temp = top;
    *value = top->value;
    top = top->next;
    free(temp);
    return top;
}

int isOperator(char* token) {
    return (strcmp(token, "+") == 0 ||
        strcmp(token, "-") == 0 ||
        strcmp(token, "*") == 0 ||
        strcmp(token, "/") == 0);
}

double evaluatePostfix(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error!'\n", filename);
        exit(1);
    }

    Node* stack = NULL;
    char token[50];

    while (fscanf(file, "%s", token) == 1) {
        if (isOperator(token)) {
            double a, b, result;

            stack = pop(stack, &b);
            stack = pop(stack, &a);

            if (strcmp(token, "+") == 0)
                result = a + b;
            else if (strcmp(token, "-") == 0)
                result = a - b;
            else if (strcmp(token, "*") == 0)
                result = a * b;
            else if (strcmp(token, "/") == 0) {
                if (b == 0) {
                    printf("Error!\n");
                    exit(1);
                }
                result = a / b;
            }

            stack = push(stack, result);
        }
        else {
            double num = atof(token);
            stack = push(stack, num);
        }
    }

    fclose(file);

    double finalResult;
    stack = pop(stack, &finalResult);

    if (stack != NULL) {
        printf("Error!\n");
    }

    return finalResult;
}

int main() {
    double result = evaluatePostfix("izraz.txt");
    printf("Result: %.2f\n", result);
    return 0;
}