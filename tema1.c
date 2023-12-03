/* URECHE Andreea-Maria - 312CC */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define BUFFER_SIZE 100

/* implementare structura de tip lista pentru coada de operatii UPDATE */
typedef struct queueNode {
    char data[BUFFER_SIZE];
    char character;
    struct queueNode* next;
} queueNode;

/* front si rear-ul cozii */
typedef struct queue {
    queueNode* front;
    queueNode* rear;
} Queue;

/* implementare nod lista dublu inlantuita */
typedef struct Node {
    struct Node* prev;
    struct Node* next;
    char data;
} Node;

/* implementare lista dublu inlantuita cu santinela si current(deget) */
typedef struct List {
    struct Node* sentinel;
    struct Node* current;
} List;

/* implementare stiva pentru operatii UNDO/REDO */
typedef struct cell {
    Node* elem;
    struct cell* next;
} Stack, *TStack;

/* prototipuri */

/* functii pentru coada UPDATE */
Queue* initQueue();
void enqueue(char value[], char chr, Queue* q);
void dequeue();
void freeQueue();

/* functii pentru stive UNDO/REDO */
TStack initStack();
Node* peek(TStack s);
TStack pop(TStack s);
TStack push(TStack s, Node* adr);
int isEmptyStack(TStack s);
void freeStacks(TStack s);

/* functii pentru lista */
void initList(List* list);
void execute(List* list, FILE* fptr_output, TStack* s, TStack* r, Queue* q);
void insertRIGHT(List* list, char data);
void insertLEFT(List* list, char data);
void insertAtEnd(List* list, char data);
void moveRightChar(List* list, char ch);
void moveLeftChar(List* list, FILE* fptr_output, char ch);
void printList(List* list, FILE* fptr_output);
void printCurr(List* list, FILE* fptr_output);
void freeList(List* list);

int main() {
    // initializare coada
    Queue* q = initQueue();

    // initializare lista(banda)
    List list;
    initList(&list);

    // initializare stive UNDO/REDO
    TStack stack_undo, stack_redo;
    stack_undo = initStack();
    stack_redo = initStack();

    // deschidere fisiere
    FILE *fptr_input, *fptr_output;
    fptr_input = fopen("tema1.in", "r");
    fptr_output = fopen("tema1.out", "w");
    if (fptr_input == NULL) exit(1);
    if (fptr_output == NULL) exit(1);

    int i, num_op; 
    //citire numar de comenzi/operatii
    fscanf(fptr_input, "%d", &num_op);

    char buffer[BUFFER_SIZE];
    char chr;
    for (i = 0; i < num_op; i++) {
        fscanf(fptr_input, "%s", buffer);

        // adaugare operatii de tip UPDATE in coada 
        if (strcmp(buffer, "MOVE_LEFT_CHAR") == 0 ||
            strcmp(buffer, "MOVE_RIGHT_CHAR") == 0 ||
            strcmp(buffer, "INSERT_LEFT") == 0 ||
            strcmp(buffer, "INSERT_RIGHT") == 0) {
            fscanf(fptr_input, " %c", &chr);
            enqueue(buffer, chr, q);
        } else if (strcmp(buffer, "WRITE") == 0) {
            fscanf(fptr_input, " %c", &chr);
            enqueue(buffer, chr, q);
        } else if (strcmp(buffer, "MOVE_LEFT") == 0 ||
                   strcmp(buffer, "MOVE_RIGHT") == 0) {
            enqueue(buffer, '\0', q);
        }

        // operatia EXECUTE
        if (strcmp(buffer, "EXECUTE") == 0) {
            execute(&list, fptr_output, &stack_undo, &stack_redo, q);
        }

        // operatii QUERY
        if (strcmp(buffer, "SHOW") == 0) {
            /*afisam toata lista/banda */
            printList(&list, fptr_output);
        }
        if (strcmp(buffer, "SHOW_CURRENT") == 0) {
            /*afisam elementul curent*/
            printCurr(&list, fptr_output);
        }

        // operatia UNDO
        if (strcmp(buffer, "UNDO") == 0) {
            /*extragem adresa de pe stiva de UNDO*/
            Node* newadr = peek(stack_undo);
            /*adaugam adresa curenta pe stiva de REDO*/
            stack_redo = push(stack_redo, list.current);
            /*actualizam pozitia*/
            list.current = newadr;
            /*eliminam elementul de pe stiva UNDO*/
            stack_undo = pop(stack_undo);
        }

        // operatia REDO
        if (strcmp(buffer, "REDO") == 0) {
            /*extragem adresa de pe stiva de REDO*/
            Node* newadr = peek(stack_redo);
            /*adaugam adresa curenta pe stiva de UNDO*/
            stack_undo = push(stack_undo, list.current);
            /*actualizam pozitia*/
            list.current = newadr;
            /*eliminam elementul de pe stiva REDO*/
            stack_redo = pop(stack_redo);
        }
    }

    /*inchidere fisiere*/
    fclose(fptr_input);
    fclose(fptr_output);

    /*eliberare memorie*/
    freeList(&list);
    freeQueue(q);
    freeStacks(stack_redo);
    freeStacks(stack_undo);
    free(q);

    return 0;
}

/* initializare coada pentru operatii UPDATE */
Queue* initQueue() {
    Queue* q = (Queue*)malloc(sizeof(Queue));
    q->front = q->rear = NULL;
    return q;
}

/* adaugare element in coada UPDATE */
void enqueue(char value[], char chr, Queue* q) {
    queueNode* newnode;
    newnode = (queueNode*)malloc(sizeof(queueNode));
    strcpy(newnode->data, value);
    newnode->character = chr;
    newnode->next = NULL;

    if (q->front == NULL) {
        q->front = q->rear = newnode;
    } else {
        q->rear->next = newnode;
        q->rear = newnode;
    }
}

/* eliminare element din coada UPDATE */
void dequeue(Queue* q) {
    queueNode* temp;
    temp = q->front;
    /* daca coada e goala */
    if (q->front == NULL && q->rear == NULL) {
        return;
    } else {
        q->front = q->front->next;
        free(temp);
    }
}

/* eliberare coada */
void freeQueue(Queue* q) {
    queueNode* temp;

    while (q->front != NULL) {
        temp = q->front;
        q->front = q->front->next;
        free(temp);
    }
}

/* initializare stiva */
TStack initStack() {
    TStack s;
    return s = NULL;
}

/* element curent stiva */
Node* peek(TStack s) {
    Node* el;
    if (s == NULL) {
        return NULL;
    }
    el = s->elem;
    return el;
}

/* eliminare element de pe stiva */
TStack pop(TStack s) {
    TStack t;

    if (s == NULL) {
        return NULL;
    }
    t = s;
    s = s->next;
    free(t);
    return s;
}

/* adaugare element pe stiva */
TStack push(TStack s, Node* adr) {
    TStack t;
    t = (TStack)malloc(sizeof(Stack));
    if (t == NULL) {
        return NULL;
    }
    t->elem = adr;
    t->next = s;
    return t;
}

/* test stiva goala */
int isEmptyStack(TStack s) {
    return s == NULL; 
}

/* eliberare stiva */
void freeStacks(TStack s) {
    TStack temp;
    while (s != NULL) {
        temp = s;
        s = s->next;
        free(temp);
    }
}

/* initializare lista cu santinela si primul nod ('#') */
void initList(List* list) {
    /* alocam memorie pentru santinela si o initializam */
    list->sentinel = (Node*)malloc(sizeof(Node));
    list->sentinel->prev = NULL;
    list->sentinel->next = NULL;

    /* adaugare primul nod ce contine doar ('#') */
    Node* first = (Node*)malloc(sizeof(Node));
    first->data = '#';
    /* legam nodul la santinela */
    first->prev = list->sentinel;
    first->next = NULL;
    list->sentinel->next = first;

    /* mutam pozitia curenta pe primul nod cu date ('#') */
    list->current = first;
}

/* functie pentru EXECUTE */
void execute(List* list, FILE* fptr_output, TStack* s, TStack* r, Queue* q) {
    /* extragem prima operatie din coada UPDATE si o executam */
    if (strcmp(q->front->data, "MOVE_LEFT_CHAR") == 0) {
        char ch = q->front->character;
        moveLeftChar(list, fptr_output, ch);
    }
    if (strcmp(q->front->data, "MOVE_RIGHT_CHAR") == 0) {
        char ch = q->front->character;
        moveRightChar(list, ch);
    }
    if (strcmp(q->front->data, "INSERT_LEFT") == 0) {
        char ch = q->front->character;
        if (list->current->prev !=
            list->sentinel) {  // daca nu ne aflam pe prima poz (element
                               // anterior nu e santinela)
            insertLEFT(list, ch);
        } else {
            fprintf(fptr_output, "%s\n", "ERROR");
        }
    }
    if (strcmp(q->front->data, "INSERT_RIGHT") == 0) {
        char ch = q->front->character;
        insertRIGHT(list, ch);
    }
    if (strcmp(q->front->data, "WRITE") == 0) {
        char ch = q->front->character;
        list->current->data = ch;
        /* comanda WRITE => eliberam stivele UNDO/REDO */
        while (!isEmptyStack(*s)) {
            *s = pop(*s);
        }
        while (!isEmptyStack(*r)) {
            *r = pop(*r);
        }
    }
    if (strcmp(q->front->data, "MOVE_RIGHT") == 0) {
        Node* temp = list->current;
        if (list->current->next != NULL) {  // daca exista nod dupa
            list->current = list->current->next;
            // comanda efectuata cu succes
            *s = push(*s, temp);
        } else {
            insertRIGHT(list, '#');
            // comanda efectuata cu succes
            *s = push(*s, temp);
        }
    }

    if (strcmp(q->front->data, "MOVE_LEFT") == 0) {
        Node* temp = list->current; 
        // daca exista nod inainte
        if (list->current->prev != list->sentinel) { 
            list->current = list->current->prev;
            // comanda efectuata cu succes => update STACK
            *s = push(*s, temp);
        } else {
            dequeue(q);
            return;
        }
    }
    dequeue(q);
}

/* inserare la dreapta */
void insertRIGHT(List* list, char data) {
    // alocam memorie pentru un nou nod si il initializam
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->prev = list->current;
    newNode->next = list->current->next;
    newNode->data = data;

    list->current->next = newNode;
    if (newNode->next != NULL) {
        newNode->next->prev = newNode;
    }
    // mutam cursorul pe noul nod inserat
    list->current = newNode;
}

/* inserare la stanga */
void insertLEFT(List* list, char data) {
    Node* newnode = (Node*)malloc(sizeof(Node));
    newnode->prev = list->current->prev;
    newnode->next = list->current;
    newnode->data = data;

    if (list->current->prev != NULL) {
        list->current->prev->next = newnode;
    }
    list->current->prev = newnode;
    list->current = newnode;
}

/* inserare nod la final de lista */
void insertAtEnd(List* list, char data) {
    // alocam memorie pentru un nou nod si il initializam
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->next = NULL;
    newNode->data = data;

    Node* currentNode = list->sentinel;
    while (currentNode->next != NULL) {
        currentNode = currentNode->next;
    }

    currentNode->next = newNode;
    newNode->prev = currentNode;
    list->current = newNode;
}

/* cautare caracter la dreapta */
void moveRightChar(List* list, char ch) {
    Node* tmp = list->current;
    while (tmp != NULL && tmp->data != ch) {
        tmp = tmp->next;
    }
    if (tmp == NULL) {  // daca nu s a gasit caracterul
        // ne mutam pe ultima pozitie
        insertAtEnd(list, '#');
    } else {
        list->current = tmp;  // mutam current pe elementul gasit
    }
}

/* cautare caracter la stanga */
void moveLeftChar(List* list, FILE* fptr_output, char ch) {
    // cautarea incepe de la pozita curenta a degetului
    Node* tmp = list->current;
    while (tmp != list->sentinel && tmp->data != ch) { 
        tmp = tmp->prev;
    }
    if (tmp == list->sentinel) {
        fprintf(fptr_output, "%s\n", "ERROR");
    } else {
        list->current = tmp;  // mutam current pe element gasit
    }
}

/* afisare lista */
void printList(List* list, FILE* fptr_output) {
    /* parcurgem lista incepand cu elementul de dupa
     santinela si afisam fiecare nod */
    Node* start = list->sentinel->next;
    while (start != NULL) {
        if (start == list->current) {
            fprintf(fptr_output, "|");
            fprintf(fptr_output, "%c", start->data);
            fprintf(fptr_output, "|");
        } else {
            fprintf(fptr_output, "%c", start->data);
        }
        start = start->next;
    }
    fprintf(fptr_output, "\n");
}

/* afisare element curent */
void printCurr(List* list, FILE* fptr_output) {
    fprintf(fptr_output, "%c\n", list->current->data);
}

/* eliberare lista */
void freeList(List* list) {
    Node* tmp = list->sentinel;
    while (tmp != NULL) {
        Node* tmp1 = tmp;
        tmp = tmp->next;
        free(tmp1);
    }
}
