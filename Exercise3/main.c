/*
 *Exercise 3
 * (i) (Διέλευση δυαδικού δέντρου κατά επίπεδα)
 * Αυτή η άσκηση παρουσιάζει τη διαπέραση κατά επίπεδα ενός δυαδικού δέντρου.
 * Αυτή η διέλευση εμφανίζει τις τιμές των κόμβων από επίπεδο προς επίπεδο και από
 * αριστερά προς τα δεξιά, ξεκινώντας από το επίπεδο της ρίζας. Ο αλγόριθμος δεν είναι
 * αναδρομικός αλλά χρησιμοποιεί τη δομή δεδομένων ουρά για την επεξεργασία των κόμβων 
 * με τη σωστή σειρά.
 *
 * Ο αλγόριθμος έχει ως εξής:
 *  1. Εισάγετε την ρίζα στην ουρά.
 *  2. Ενόσω υπάρχουν κόμβοι στην ουρά,
 *      ✓ Πάρε τον επόμενο κόμβο στην ουρά.
 *      ✓ Εκτύπωσε την τιμή του κόμβου.
 *      ✓ Εάν ο δείκτης στο αριστερό παιδί του κόμβου δεν είναι NULL, να
 *          εισάγετε το αριστερό παιδί στην ουρά.
 *      ✓ Εάν ο δείκτης στο δεξιό παιδί του κόμβου δεν είναι NULL να εισάγετε
 *      το δεξιό παιδί στην ουρά.
 * Γράψτε συνάρτηση με το όνομα levelOrder για να εκτελέσετε μια διαπέραση κατά
 * επίπεδα ενός δυαδικού δέντρου. Η συνάρτηση θα πρέπει να λαμβάνει ως όρισμα έναν
 * δείκτη στη ρίζα του δυαδικού δέντρου. Τεστάρετε τη συνάρτηση με ένα κυρίως
 * πρόγραμμα με δεδομένα της επιλογής σας και δοκιμάστε ενδεχομένως και
 * διαφορετικές υλοποιήσεις (λίστα ή πίνακας για την ουρά που θα χρησιμοποιήσετε).
 *
 * (ii) (Διελεύσεις δυαδικού δέντρου)
 * Αντίστοιχα υλοποιήστε προδιάταξη (προδιατεταγμένη διαπέραση) και
 * μεταδιάταξη (μεταδιατεταγμένη διαπέραση). Τεστάρετε τη συνάρτηση με ένα
 * κυρίως πρόγραμμα με δεδομένα της επιλογής σας
*/

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER 100
#define EXIT 0 

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Queue{
    struct Node* items[BUFFER];
    int front;
    int rear;
};     // Define Queue Structure

// Node functions
struct Node* insert(struct Node* node, int value);
struct Node* createNode(int value);

struct Node* insert(struct Node* node, int value) {
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->data) {
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        node->right = insert(node->right, value);
    } else if (value == node->data) {
        printf("The value %d is already in the tree!\n", value);
    }
    return node;
}

struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

void preorder(struct Node* n) {
    if(n == NULL) {
        return;
    }

    printf("%d ", n->data);
    preorder(n->left);
    preorder(n->right);
}

void postorder(struct Node* n) {
    if(n == NULL) {
        return;
    }

    postorder(n->left);
    postorder(n->right);
    printf("%d ", n->data);
}

// Queue functions

void initializeQueue(struct Queue *q) {        // Initializing function for queue
    q->front = -1;
    q->rear = -1;
}

bool isEmpty(struct Queue *q) {                // Check if the queue is empty
    return(q->front == q-> rear);
}

bool isFull(struct Queue *q) {                 // Check if the queue is full
    return(q->rear == BUFFER - 1 );
}

void add_to_queue(struct Queue *q, struct Node* n) {     // Add node in queue
    if(isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    
    q->rear++;
    q->items[q->rear] = n;
}

struct Node* remove_from_queue(struct Queue *q) {         // Remove node in queue
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return NULL;
    }
    q->front++;
    return q->items[q->front];
}

void levelOrder(struct Node* n) {

    if(n == NULL) {
        return;
    }

    struct Queue q;
    initializeQueue(&q);

    add_to_queue(&q, n);

    while(!isEmpty(&q)){
        struct Node* current = remove_from_queue(&q);

        printf("%d ", current->data);

        if(current->left != NULL) {
            add_to_queue(&q, current->left);
        }
        if(current->right != NULL) {
            add_to_queue(&q, current->right);
        }

    }
}

int main() {
    struct Node* root = NULL;  // Αρχικοποιηση ριζας
    FILE *fptr = fopen("numbers3.txt", "r");

    if (fptr == NULL) {
        printf("Error!\n");
        return 1;
    }

    int value;

    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value);
    }

    printf("Level Order: \n");
    levelOrder(root);

    printf("\nPreorder: \n");
    preorder(root);

    printf("\nPostorder: \n");
    postorder(root);

    return 0;
}
