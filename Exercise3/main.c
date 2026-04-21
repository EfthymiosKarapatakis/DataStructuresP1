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

// Ιδιότητα: αριστερό παιδί < γονέας < δεξί παιδί

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>

#define BUFFER 100  /* Μέγιστο μέγεθος ουράς – αρκεί να είναι >= πλήθος κόμβων */

/* -------------------------------------------------------
 * Δομή κόμβου BST
 * data: η ακέραια τιμή του κόμβου
 * left: δείκτης στο αριστερό παιδί (μικρότερες τιμές)
 * right: δείκτης στο δεξί παιδί (μεγαλύτερες τιμές)
 * ------------------------------------------------------- */
struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Queue {
    struct Node* items[BUFFER];
    int front;
    int rear;
};

// declarations
struct Node* createNode(int value);
struct Node* insert(struct Node* node, int value);
void freeTree(struct Node* node);
void preorder(struct Node* n);
void postorder(struct Node* n);
void levelOrder(struct Node* n);
void initializeQueue(struct Queue *q);
bool isEmpty(struct Queue *q);
bool isFull(struct Queue *q);
void add_to_queue(struct Queue *q, struct Node* n);
struct Node* remove_from_queue(struct Queue *q);

struct Node* createNode(int value) {
    // Δεσμεύει μνήμη, αποθηκεύει την τιμή και
    // θέτει τους δείκτες παιδιών σε NULL (φύλλο).
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data  = value;
    newNode->left  = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* node, int value) {
    /* Βρήκαμε κενή θέση → τοποθετούμε νέο κόμβο */
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->data) {
        /* Η τιμή ανήκει στο αριστερό υποδέντρο */
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        /* Η τιμή ανήκει στο δεξί υποδέντρο */
        node->right = insert(node->right, value);
    } else {
        /* Η τιμή υπάρχει ήδη — δεν επιτρέπουμε διπλότυπα */
        printf("The value %d is already in the tree!\n", value);
    }

    return node;
}

void freeTree(struct Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

/* -------------------------------------------------------
 * preorder:
 *      Σειρά: root → left → right
 *      Χρήσιμη για: αντιγραφή δέντρου, έκφραση prefix
 * ------------------------------------------------------- */
void preorder(struct Node* n) {
    if (n == NULL) return;      /* Βάση αναδρομής: κενός κόμβος */

    printf("%d ", n->data);     /* 1. Εκτύπωσε τον τρέχοντα κόμβο */
    preorder(n->left);          /* 2. Διάσχισε αναδρομικά αριστερά */
    preorder(n->right);         /* 3. Διάσχισε αναδρομικά δεξιά */
}

/* -------------------------------------------------------
 * postorder: Μεταδιατεταγμένη διαπέραση
 *      Σειρά: left → right → root
 *      Χρήσιμη για: διαγραφή δέντρου, έκφραση postfix
 * ------------------------------------------------------- */
void postorder(struct Node* n) {
    if (n == NULL) return;      /* Βάση αναδρομής: κενός κόμβος */

    postorder(n->left);         /* 1. Διάσχισε αναδρομικά αριστερά */
    postorder(n->right);        /* 2. Διάσχισε αναδρομικά δεξιά */
    printf("%d ", n->data);     /* 3. Εκτύπωσε τον τρέχοντα κόμβο */
}

/* -------------------------------------------------------
 * levelOrder: Διαπέραση κατά επίπεδα (BFS)
 *
 * Αλγόριθμος:
 *  1. Εισάγουμε τη ρίζα στην ουρά
 *  2. Όσο η ουρά δεν είναι κενή:
 *     α. Αφαιρούμε τον πρώτο κόμβο
 *     β. Εκτυπώνουμε την τιμή του
 *     γ. Αν έχει αριστερό παιδί → το εισάγουμε στην ουρά
 *     δ. Αν έχει δεξί παιδί    → το εισάγουμε στην ουρά
 *
 * Η ουρά (FIFO) εγγυάται ότι επεξεργαζόμαστε κόμβους
 * επίπεδο-επίπεδο, από αριστερά προς δεξιά.
 * ------------------------------------------------------- */
void levelOrder(struct Node* n) {
    if (n == NULL) return;

    struct Queue q;
    initializeQueue(&q);        /* Αρχικοποίηση κενής ουράς */

    add_to_queue(&q, n);        /* Βήμα 1: εισαγωγή ρίζας */

    while (!isEmpty(&q)) {
        /* Βήμα 2α: αφαίρεση πρώτου κόμβου (FIFO) */
        struct Node* current = remove_from_queue(&q);

        /* Βήμα 2β: εκτύπωση τιμής */
        printf("%d ", current->data);

        /* Βήμα 2γ: αν υπάρχει αριστερό παιδί → εισαγωγή στην ουρά */
        if (current->left != NULL) {
            add_to_queue(&q, current->left);
        }

        /* Βήμα 2δ: αν υπάρχει δεξί παιδί → εισαγωγή στην ουρά */
        if (current->right != NULL) {
            add_to_queue(&q, current->right);
        }
    }
}

void initializeQueue(struct Queue *q) {
    q->front = -1;
    q->rear  = -1;
}

bool isEmpty(struct Queue *q) {
    // True όταν front == rear (κανένα στοιχείο δεν έχει μπει, ή όλα έχουν αφαιρεθεί).
    return (q->front == q->rear);
}

bool isFull(struct Queue *q) {
    // True όταν rear έχει φτάσει στο τέλος του πίνακα.
    return (q->rear == BUFFER - 1);
}

void add_to_queue(struct Queue *q, struct Node* n) {
    // Αν η ουρά είναι γεμάτη, εκτυπώνουμε μήνυμα και φεύγουμε.
    if (isFull(q)) {
        printf("Queue is full\n");
        return;
    }
    q->rear++;                  /* Μετακίνηση rear στην επόμενη κενή θέση */
    q->items[q->rear] = n;      /* Τοποθέτηση κόμβου */
}

struct Node* remove_from_queue(struct Queue *q) {
    if (isEmpty(q)) {
        printf("Queue is empty\n");
        return NULL;
    }
    q->front++;                     /* Μετακίνηση front στο επόμενο στοιχείο */
    return q->items[q->front];      /* Επιστροφή κόμβου */
}


int main() {
    struct Node* root = NULL;   /* Αρχικά το δέντρο είναι κενό */

    /* Διάβασμα τιμών από αρχείο και κατασκευή BST */
    FILE *fptr = fopen("numbers3.txt", "r");
    if (fptr == NULL) {
        printf("Error opening file!\n");
        return 1;
    }

    int value;
    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value); /* Εισαγωγή κάθε τιμής στο BST */
    }
    fclose(fptr);   /* Κλείσιμο αρχείου μετά την ανάγνωση */

    /* Level-Order: επίπεδο προς επίπεδο, left → right */
    printf("Level Order:\n");
    levelOrder(root);

    /* Preorder: root → left → right */
    printf("\nPreorder:\n");
    preorder(root);

    /* Postorder: left → right → root */
    printf("\nPostorder:\n");
    postorder(root);

    printf("\n");

    /* Αποδέσμευση όλης της δυναμικής μνήμης */
    freeTree(root);

    return 0;
}