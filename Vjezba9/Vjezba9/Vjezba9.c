/*Zadan je niz brojeva 2, 5, 7, 8, 11, 1, 4, 2, 3, 7 koji su spremljeni u cvorove binarnog stabla.
a) Napisati funkciju insert koja dodaje element u stablo tako da se pozivima te funkcije za
sve element zadanog niza brojeva stvori stablo kao na slici Slika 1. Funkcije vraca
pokazivac na korijen stabla.
b) Napisati funkciju replace koja ce svaki element stabla zamijeniti sumom elemenata u
njegovom lijevom i desnom podstablu(tj.sumom svih potomaka prije zamjene
vrijednosti u tim potomcima).Npr.stablo sa slike Slika 1 transformirat ce se u stablo na
slici Slika 2.
c) Prepraviti program na nacin da umjesto predefiniranog cjelobrojnog polja koristenjem
funkcije rand() generira slu?ajne brojeve u rasponu <10, 90>.Takoder, potrebno je
upisati u datoteku sve brojeve u inorder prolasku nakon koristenja funkcije iz a), zatim b)
dijela zadatka.*/

#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

//definicija strukture za cvor
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

//kreira novi cvor sa zadanom vrijednoscu
Node* createNode(int value) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//dodaje cvor u stablo koristeci BFS pristup
Node* addNode(Node* root, int value) {
    if (root == NULL)
        return createNode(value);

    Node* queue[100];
    int front = 0, rear = 0;
    queue[rear++] = root;

    while (front < rear) {
        Node* current = queue[front++];

        if (current->left == NULL) {
            current->left = createNode(value);
            return root;
        }
        queue[rear++] = current->left;

        if (current->right == NULL) {
            current->right = createNode(value);
            return root;
        }
        queue[rear++] = current->right;
    }
    return root;
}

//transformira stablo - zamjenjuje vrijednosti zbrojem djece
int transformTree(Node* root) {
    if (root == NULL)
        return 0;

    int leftSum = transformTree(root->left);
    int rightSum = transformTree(root->right);
    int originalValue = root->data;
    root->data = leftSum + rightSum;

    return originalValue + root->data;
}

//ispisuje stablo inorder metodom
int printInorder(Node* root, FILE* output) {
    if (root == NULL)
        return 0;

    printInorder(root->left, output);
    fprintf(output, "%d ", root->data);
    printInorder(root->right, output);

    return 1;
}

//generira stablo sa slucajnim vrijednostima
Node* generateRandomTree(int size) {
    Node* root = NULL;
    for (int i = 0; i < size; i++) {
        int randomValue = rand() % 81 + 10;
        root = addNode(root, randomValue);
    }
    return root;
}

//dealocira memoriju stabla
void freeTree(Node* root) {
    if (root == NULL)
        return;

    freeTree(root->left);
    freeTree(root->right);
    free(root);
}

int main() {
    srand(time(NULL));

    Node* tree = generateRandomTree(9);

    FILE* outputFile = fopen("izlaz.txt", "w");

    printInorder(tree, outputFile);
    fprintf(outputFile, "\n");

    transformTree(tree);
    printInorder(tree, outputFile);

    fclose(outputFile);
    freeTree(tree);

    return 0;
}