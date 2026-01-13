/*Napisati program za zbrajanje i mnozenje polinoma. Koeficijenti i eksponenti se
citaju iz datoteke.
Napomena: Eksponenti u datoteci nisu nuzno sortirani.*/


#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

//klasa za jedan element polinoma
typedef struct Term {
    int coefficient;
    int exponent;
    struct Term* next;
} Term;

//klasa polinoma - niz elemenata
typedef struct Polynomial {
    Term* head;
} Polynomial;

//stvaranje polinoma
Polynomial* createPolynomial() {
    Polynomial* poly = (Polynomial*)malloc(sizeof(Polynomial));
    poly->head = NULL;
    return poly;
}

//stvaranje jednog elementa
Term* createTerm(int coefficient, int exponent) {
    Term* newTerm = (Term*)malloc(sizeof(Term));
    newTerm->coefficient = coefficient;
    newTerm->exponent = exponent;
    newTerm->next = NULL;
    return newTerm;
}

//stavlja elemente u polinom i zbraja one s istim eksponentom
void insertTerm(Polynomial* poly, int coefficient, int exponent) {
    //preskace elemente bez potencije
    if (coefficient == 0) return;

    Term* current = poly->head;
    Term* previous = NULL;

    //slaze ih po redu od najveceg prema najmanjem
    while (current != NULL && current->exponent > exponent) {
        previous = current;
        current = current->next;
    }

    //zbrajanje onih s istim eksponentima
    if (current != NULL && current->exponent == exponent) {
        current->coefficient += coefficient;
        //ako je zbroj necega 0 brise element
        if (current->coefficient == 0) {
            if (previous == NULL) {
                poly->head = current->next;
            }
            else {
                previous->next = current->next;
            }
            free(current);
        }
        return;
    }

    //stvara novi element
    Term* newTerm = createTerm(coefficient, exponent);

    if (previous == NULL) {
        newTerm->next = poly->head;
        poly->head = newTerm;
    }
    else {
        newTerm->next = current;
        previous->next = newTerm;
    }
}

//stavlja polinom u zeljeni format
void parsePolynomial(Polynomial* poly, char* line) {
    int i = 0;
    int len = strlen(line);

    while (i < len) {
        while (i < len && isspace(line[i])) i++;
        if (i >= len) break;

        int sign = 1;
        if (line[i] == '+') {
            i++;
            while (i < len && isspace(line[i])) i++;
        }
        else if (line[i] == '-') {
            sign = -1;
            i++;
            while (i < len && isspace(line[i])) i++;
        }

        int coefficient = 0;
        int hasCoefficient = 0;
        while (i < len && isdigit(line[i])) {
            coefficient = coefficient * 10 + (line[i] - '0');
            hasCoefficient = 1;
            i++;
        }

        while (i < len && isspace(line[i])) i++;

        int exponent = 0;
        if (i < len && (line[i] == 'x' || line[i] == 'X')) {
            if (!hasCoefficient) coefficient = 1;
            i++;

            while (i < len && isspace(line[i])) i++;

            if (i < len && line[i] == '^') {
                i++;
                while (i < len && isspace(line[i])) i++;

                exponent = 0;
                while (i < len && isdigit(line[i])) {
                    exponent = exponent * 10 + (line[i] - '0');
                    i++;
                }
            }
            else {
                exponent = 1;
            }
        }
        else {
            if (!hasCoefficient) continue;
            exponent = 0;
        }

        coefficient *= sign;
        insertTerm(poly, coefficient, exponent);
    }
}

//cita dva polinoma iz jedne datoteke
void readPolynomialsFromFile(const char* filename, Polynomial* poly1, Polynomial* poly2) {
    FILE* file = fopen(filename, "r");
    if (file == NULL) {
        printf("Error opening file: %s\n", filename);
        return;
    }

    char line[1000];

    //citanje prvog polinoma
    if (fgets(line, sizeof(line), file) != NULL) {
        parsePolynomial(poly1, line);
    }

    //citanje drugog polinoma
    if (fgets(line, sizeof(line), file) != NULL) {
        parsePolynomial(poly2, line);
    }

    fclose(file);
}

//zbrajanje polinoma
Polynomial* addPolynomials(Polynomial* p1, Polynomial* p2) {
    Polynomial* result = createPolynomial();

    Term* term1 = p1->head;
    while (term1 != NULL) {
        insertTerm(result, term1->coefficient, term1->exponent);
        term1 = term1->next;
    }

    Term* term2 = p2->head;
    while (term2 != NULL) {
        insertTerm(result, term2->coefficient, term2->exponent);
        term2 = term2->next;
    }

    return result;
}

//mnozenje polinoma
Polynomial* multiplyPolynomials(Polynomial* p1, Polynomial* p2) {
    Polynomial* result = createPolynomial();

    Term* term1 = p1->head;
    while (term1 != NULL) {
        Term* term2 = p2->head;
        while (term2 != NULL) {
            int newCoefficient = term1->coefficient * term2->coefficient;
            int newExponent = term1->exponent + term2->exponent;
            insertTerm(result, newCoefficient, newExponent);
            term2 = term2->next;
        }
        term1 = term1->next;
    }

    return result;
}

//printanje u zeljenom formatu
void printPolynomial(Polynomial* poly) {
    if (poly->head == NULL) {
        printf("0\n");
        return;
    }

    Term* current = poly->head;
    int firstTerm = 1;

    while (current != NULL) {
        if (!firstTerm && current->coefficient > 0) {
            printf(" + ");
        }
        else if (current->coefficient < 0) {
            printf(" - ");
        }

        int absCoeff = abs(current->coefficient);

        if (current->exponent == 0) {
            printf("%d", absCoeff);
        }
        else if (current->exponent == 1) {
            if (absCoeff == 1) {
                printf("x");
            }
            else {
                printf("%dx", absCoeff);
            }
        }
        else {
            if (absCoeff == 1) {
                printf("x^%d", current->exponent);
            }
            else {
                printf("%dx^%d", absCoeff, current->exponent);
            }
        }

        firstTerm = 0;
        current = current->next;
    }
    printf("\n");
}

//oslobadanje memorije
void freePolynomial(Polynomial* poly) {
    Term* current = poly->head;
    while (current != NULL) {
        Term* temp = current;
        current = current->next;
        free(temp);
    }
    free(poly);
}

int main() {
    Polynomial* poly1 = createPolynomial();
    Polynomial* poly2 = createPolynomial();

    readPolynomialsFromFile("polynomials.txt", poly1, poly2);

    printf("Polynomial 1: ");
    printPolynomial(poly1);

    printf("Polynomial 2: ");
    printPolynomial(poly2);

    Polynomial* sum = addPolynomials(poly1, poly2);
    printf("\nSum: ");
    printPolynomial(sum);

    Polynomial* product = multiplyPolynomials(poly1, poly2);
    printf("Product: ");
    printPolynomial(product);

    freePolynomial(poly1);
    freePolynomial(poly2);
    freePolynomial(sum);
    freePolynomial(product);

    return 0;
}