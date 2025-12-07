/*Napisati program koji cita datoteku racuni.txt u kojoj su zapisani nazivi svih datoteka koji
predstavljaju pojedini racun.Na pocetku svake datoteke je zapisan datum u kojem vremenu je
racun izdat u formatu YYYY - MM - DD.Svaki sljedeci red u datoteci predstavlja artikl u formatu
naziv, kolicina, cijena.Potrebno je formirati vezanu listu racuna sortiranu po datumu.Svaki cvor
vezane liste sadržava vezanu listu artikala sortiranu po nazivu artikla.Nakon toga potrebno je
omoguciti upit kojim ce korisnik saznati koliko je novaca sveukupno potroseno na specificni
artikl u odredenom vremenskom razdoblju i u kojoj je kolicini isti kupljen.*/

#define _CRT_SECURE_NO_WARNINGS

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//struktura za artikl
typedef struct Item {
    char name[50];
    int quantity;
    float price;
    struct Item* next;
} Item;

//struktura za racun
typedef struct Receipt {
    char date[11];
    Item* items;
    struct Receipt* next;
} Receipt;

//struktura za odredivanje koji je artikl kupljen najvise puta
typedef struct PopularItem {
    char name[50];
    int quantity;
} PopularItem;

//sve koristene funkcije
Item* newItem(char* name, int quantity, float price);
Receipt* newReceipt(char* date);
void addItem(Receipt* receipt, char* name, int quantity, float price);
void addReceipt(Receipt** head, Receipt* new);
Receipt* loadReceipt(char* filename);
Receipt* loadAllReceipts(char* filename);
void totalSpentOnItem(Receipt* head, char* itemName, char* dateFrom, char* dateTo);
void totalSpentOnDate(Receipt* head, char* targetDate);
void updatePopular(PopularItem arr[], int* size, char* name, int quantity);
void mostPopularItem(Receipt* head, char* dateFrom, char* dateTo);
void mostExpensiveReceipt(Receipt* head);
void freeMemory(Receipt* head);

int main() {
    Receipt* receipts = loadAllReceipts("receipts.txt");

    if (receipts == NULL) {
        printf("No receipts loaded.\n");
        return 1;
    }

    char itemName[50];
    char dateFrom[11], dateTo[11];

    printf("\nEnter item name: ");
    scanf(" %[^\n]", itemName);

    printf("Enter date from: ");
    scanf("%s", dateFrom);

    printf("Enter date to: ");
    scanf("%s", dateTo);

    totalSpentOnItem(receipts, itemName, dateFrom, dateTo);

    char targetDate[11];
    printf("\nEnter date to see total spent : ");
    scanf("%s", targetDate);
    totalSpentOnDate(receipts, targetDate);

    printf("\nEnter date range for most popular item:\n");
    printf("Enter date from: ");
    scanf("%s", dateFrom);
    printf("Enter date to: ");
    scanf("%s", dateTo);
    mostPopularItem(receipts, dateFrom, dateTo);

    mostExpensiveReceipt(receipts);

    freeMemory(receipts);

    return 0;
}

//funkcija za stvaranje novog artikla
Item* newItem(char* name, int quantity, float price) {
    Item* i = (Item*)malloc(sizeof(Item));
    strcpy(i->name, name);
    i->quantity = quantity;
    i->price = price;
    i->next = NULL;
    return i;
}

//funkcija za stvaranje novog racuna
Receipt* newReceipt(char* date) {
    Receipt* r = (Receipt*)malloc(sizeof(Receipt));
    strcpy(r->date, date);
    r->items = NULL;
    r->next = NULL;
    return r;
}

//funkcija za dodavanje artikla u listu (sortirano po nazivu)
void addItem(Receipt* receipt, char* name, int quantity, float price) {
    Item* new = newItem(name, quantity, price);

    if (receipt->items == NULL || strcmp(name, receipt->items->name) < 0) {
        new->next = receipt->items;
        receipt->items = new;
    }
    else {
        Item* current = receipt->items;
        while (current->next != NULL && strcmp(name, current->next->name) > 0) {
            current = current->next;
        }
        new->next = current->next;
        current->next = new;
    }
}

//funkcija za dodavanje racuna u listu (sortirano po datumu)
void addReceipt(Receipt** head, Receipt* new) {
    if (*head == NULL || strcmp(new->date, (*head)->date) < 0) {
        new->next = *head;
        *head = new;
    }
    else {
        Receipt* current = *head;
        while (current->next != NULL && strcmp(new->date, current->next->date) > 0) {
            current = current->next;
        }
        new->next = current->next;
        current->next = new;
    }
}

//funkcija za ucitavanje jednog racuna iz datoteke
Receipt* loadReceipt(char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }

    char date[11];
    if (fscanf(f, "%s", date) != 1) {
        fclose(f);
        return NULL;
    }

    Receipt* receipt = newReceipt(date);

    char name[50];
    int quantity;
    float price;

    while (fscanf(f, " %[^,], %d, %f", name, &quantity, &price) == 3) {
        addItem(receipt, name, quantity, price);
    }

    fclose(f);
    return receipt;
}

//funkcija za ucitavanje svih racuna iz datoteke
Receipt* loadAllReceipts(char* filename) {
    FILE* f = fopen(filename, "r");
    if (f == NULL) {
        printf("Error: cannot open file %s\n", filename);
        return NULL;
    }

    Receipt* head = NULL;
    char receiptName[100];

    while (fscanf(f, "%s", receiptName) == 1) {
        Receipt* r = loadReceipt(receiptName);
        if (r != NULL) {
            addReceipt(&head, r);
        }
    }

    fclose(f);
    return head;
}

//funkcija koja vraca koliko je puta neki artikl kupljen i koliko je novca na njega potroseno u zadanom periodu
void totalSpentOnItem(Receipt* head, char* itemName, char* dateFrom, char* dateTo) {
    float totalPrice = 0;
    int totalQuantity = 0;

    Receipt* r = head;
    while (r != NULL) {
        if (strcmp(r->date, dateFrom) >= 0 && strcmp(r->date, dateTo) <= 0) {
            Item* i = r->items;
            while (i != NULL) {
                if (strcmp(i->name, itemName) == 0) {
                    totalPrice += i->quantity * i->price;
                    totalQuantity += i->quantity;
                }
                i = i->next;
            }
        }
        r = r->next;
    }

    printf("Item: %s\n", itemName);
    printf("Period: %s to %s\n", dateFrom, dateTo);
    printf("Total quantity: %d pcs\n", totalQuantity);
    printf("Total spent: %.2f EUR\n", totalPrice);
}

//funkcija koja vraca koliko je ukupno novca potreseno na zadani datum
void totalSpentOnDate(Receipt* head, char* targetDate) {
    float total = 0;

    Receipt* r = head;
    while (r != NULL) {
        if (strcmp(r->date, targetDate) == 0) {
            Item* i = r->items;
            while (i != NULL) {
                total += i->quantity * i->price;
                i = i->next;
            }
        }
        r = r->next;
    }

    printf("\nTotal spent on %s: %.2f EUR\n", targetDate, total);
}

//pomocna funkcija za odredivanje artikla koji je najvise puta kupljen
void updatePopular(PopularItem arr[], int* size, char* name, int quantity) {
    for (int i = 0; i < *size; i++) {
        if (strcmp(arr[i].name, name) == 0) {
            arr[i].quantity += quantity;
            return;
        }
    }
    strcpy(arr[*size].name, name);
    arr[*size].quantity = quantity;
    (*size)++;
}

//funkcija koja vraca koji je artikl kupljen najvise puta u zadanom periodu
void mostPopularItem(Receipt* head, char* dateFrom, char* dateTo) {
    PopularItem items[500];
    int itemCount = 0;

    Receipt* r = head;
    while (r != NULL) {
        if (strcmp(r->date, dateFrom) >= 0 &&
            strcmp(r->date, dateTo) <= 0) {

            Item* i = r->items;
            while (i != NULL) {
                updatePopular(items, &itemCount, i->name, i->quantity);
                i = i->next;
            }
        }
        r = r->next;
    }

    if (itemCount == 0) {
        printf("\nNo items purchased in that period.\n");
        return;
    }

    PopularItem* max = &items[0];
    for (int i = 1; i < itemCount; i++) {
        if (items[i].quantity > max->quantity) {
            max = &items[i];
        }
    }

    printf("\nMost popular item from %s to %s: %s\n", dateFrom, dateTo, max->name);
    printf("Total purchased: %d pcs\n", max->quantity);
}

//funkcija koja vraca racun dana kada je potroseno najvise novca
void mostExpensiveReceipt(Receipt* head) {
    if (head == NULL) {
        printf("No receipts loaded.\n");
        return;
    }

    Receipt* maxReceipt = NULL;
    float maxTotal = 0;

    Receipt* r = head;
    while (r != NULL) {
        float sum = 0;
        Item* i = r->items;

        while (i != NULL) {
            sum += i->quantity * i->price;
            i = i->next;
        }

        if (sum > maxTotal) {
            maxTotal = sum;
            maxReceipt = r;
        }

        r = r->next;
    }

    if (maxReceipt != NULL) {
        printf("\nMost expensive receipt:\n");
        printf("Date: %s\n", maxReceipt->date);
        printf("Total amount: %.2f EUR\n", maxTotal);
        printf("Items:\n");

        Item* i = maxReceipt->items;
        while (i != NULL) {
            printf("  %s, %d pcs, %.2f EUR (%.2f EUR)\n",
                i->name, i->quantity, i->price, i->quantity * i->price);
            i = i->next;
        }
    }
}

//funkcija za oslobadanje memorije
void freeMemory(Receipt* head) {
    while (head != NULL) {
        Receipt* tempR = head;
        head = head->next;

        while (tempR->items != NULL) {
            Item* tempI = tempR->items;
            tempR->items = tempR->items->next;
            free(tempI);
        }
        free(tempR);
    }
}