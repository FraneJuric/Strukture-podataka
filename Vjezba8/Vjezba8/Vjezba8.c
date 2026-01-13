/*Napisati program koji omogucava rad s binarnim stablom pretrazivanja.Treba
omoguciti unosenje novog elementa u stablo, ispis elemenata(inorder, preorder, postorder i 
level order), brisanje i pronalazenje nekog elementa.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>

//klasa za element u stablu
typedef struct Node {
    int data;
    struct Node* left;
    struct Node* right;
} Node;

//klasa za red
typedef struct QueueNode {
    Node* treeNode;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
} Queue;

//stvaranje novog elementa
Node* createNode(int data) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

//stavljanje elementa u stablo
Node* insertNode(Node* root, int data) {
    if (root == NULL) {
        return createNode(data);
    }

    if (data < root->data) {
        root->left = insertNode(root->left, data);
    }
    else if (data > root->data) {
        root->right = insertNode(root->right, data);
    }

    return root;
}

//trazenje elementa u stablu
Node* searchNode(Node* root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }

    if (data < root->data) {
        return searchNode(root->left, data);
    }

    return searchNode(root->right, data);
}

//trazenje najmanje vrijednosti u stablu
Node* findMinNode(Node* root) {
    while (root->left != NULL) {
        root = root->left;
    }
    return root;
}

//brisanje elementa iz stabla
Node* deleteNode(Node* root, int data) {
    if (root == NULL) {
        return root;
    }

    if (data < root->data) {
        root->left = deleteNode(root->left, data);
    }
    else if (data > root->data) {
        root->right = deleteNode(root->right, data);
    }
    else {
        if (root->left == NULL) {
            Node* temp = root->right;
            free(root);
            return temp;
        }
        else if (root->right == NULL) {
            Node* temp = root->left;
            free(root);
            return temp;
        }

        Node* temp = findMinNode(root->right);
        root->data = temp->data;
        root->right = deleteNode(root->right, temp->data);
    }

    return root;
}

//inorder
void inorderTraversal(Node* root) {
    if (root != NULL) {
        inorderTraversal(root->left);
        printf("%d ", root->data);
        inorderTraversal(root->right);
    }
}

//preorder
void preorderTraversal(Node* root) {
    if (root != NULL) {
        printf("%d ", root->data);
        preorderTraversal(root->left);
        preorderTraversal(root->right);
    }
}

//postorder
void postorderTraversal(Node* root) {
    if (root != NULL) {
        postorderTraversal(root->left);
        postorderTraversal(root->right);
        printf("%d ", root->data);
    }
}

//stvaranje reda
Queue* createQueue() {
    Queue* queue = (Queue*)malloc(sizeof(Queue));
    queue->front = NULL;
    queue->rear = NULL;
    return queue;
}

int isQueueEmpty(Queue* queue) {
    return queue->front == NULL;
}

//dodavanje elementa u red
void enqueue(Queue* queue, Node* treeNode) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->treeNode = treeNode;
    newNode->next = NULL;

    if (queue->rear == NULL) {
        queue->front = newNode;
        queue->rear = newNode;
        return;
    }

    queue->rear->next = newNode;
    queue->rear = newNode;
}

Node* dequeue(Queue* queue) {
    if (isQueueEmpty(queue)) {
        return NULL;
    }

    QueueNode* temp = queue->front;
    Node* treeNode = temp->treeNode;
    queue->front = queue->front->next;

    if (queue->front == NULL) {
        queue->rear = NULL;
    }

    free(temp);
    return treeNode;
}

//level order
void levelOrderTraversal(Node* root) {
    if (root == NULL) {
        return;
    }

    Queue* queue = createQueue();
    enqueue(queue, root);

    while (!isQueueEmpty(queue)) {
        Node* current = dequeue(queue);
        printf("%d ", current->data);

        if (current->left != NULL) {
            enqueue(queue, current->left);
        }

        if (current->right != NULL) {
            enqueue(queue, current->right);
        }
    }

    free(queue);
}

//oslobadanje memorije
void freeTree(Node* root) {
    if (root != NULL) {
        freeTree(root->left);
        freeTree(root->right);
        free(root);
    }
}

//meni
void displayMenu() {
    printf("1. Insert element\n");
    printf("2. Delete element\n");
    printf("3. Search element\n");
    printf("4. Inorder traversal\n");
    printf("5. Preorder traversal\n");
    printf("6. Postorder traversal\n");
    printf("7. Level order traversal\n");
    printf("8. Exit\n");
    printf("Enter your choice: ");
}

int main() {
    Node* root = NULL;
    int choice, value;
    Node* searchResult;

    while (1) {
        displayMenu();
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            printf("Enter value to insert: ");
            scanf("%d", &value);
            root = insertNode(root, value);
            printf("Value %d inserted successfully.\n", value);
            break;

        case 2:
            printf("Enter value to delete: ");
            scanf("%d", &value);
            searchResult = searchNode(root, value);
            if (searchResult != NULL) {
                root = deleteNode(root, value);
                printf("Value %d deleted successfully.\n", value);
            }
            else {
                printf("Value %d not found in the tree.\n", value);
            }
            break;

        case 3:
            printf("Enter value to search: ");
            scanf("%d", &value);
            searchResult = searchNode(root, value);
            if (searchResult != NULL) {
                printf("Value %d found in the tree.\n", value);
            }
            else {
                printf("Value %d not found in the tree.\n", value);
            }
            break;

        case 4:
            printf("Inorder traversal: ");
            inorderTraversal(root);
            printf("\n");
            break;

        case 5:
            printf("Preorder traversal: ");
            preorderTraversal(root);
            printf("\n");
            break;

        case 6:
            printf("Postorder traversal: ");
            postorderTraversal(root);
            printf("\n");
            break;

        case 7:
            printf("Level order traversal: ");
            levelOrderTraversal(root);
            printf("\n");
            break;

        case 8:
            printf("Exiting program...\n");
            freeTree(root);
            return 0;

        default:
            printf("Invalid choice! Please try again.\n");
        }
    }

    return 0;
}