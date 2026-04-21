/*
 * Exercise 4
 *
 * Αξιοποιώντας τις διαπεράσεις του προηγούμενου ερωτήματος, υπολογίστε:
 *
 * (Ύψος δυαδικού δέντρου) Υλοποιήστε μια συνάρτηση που λαμβάνει ως όρισμα ένα
 * δείκτη στη ρίζα ενός δυαδικού δέντρου και επιστρέφει το ύψος του. Τεστάρετε τη
 * συνάρτηση με ένα κυρίως πρόγραμμα με δεδομένα της επιλογής σας.
 *
 * (Βάθος κόμβων) Υλοποιήστε μία συνάρτηση η οποία λαμβάνει ως όρισμα ένα δείκτη
 * στη ρίζα ενός δυαδικού δέντρου και για κάθε κόμβο υπολογίζει και εκτυπώνει το βάθος
 * του. Τεστάρετε τη συνάρτηση με ένα κυρίως πρόγραμμα με δεδομένα της επιλογής σας
*/
#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* createNode(int value) {
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
    if (node == NULL)
        return createNode(value);

    if (value < node->data)
        node->left = insert(node->left, value);
    else if (value > node->data)
        node->right = insert(node->right, value);

    return node;
}

/* -------------------------------------------------------
 * Ύψους: αριθμός ΑΚΜΩΝ στο μακρύτερο μονοπάτι από τη ρίζα ως φύλλο.
 *   - Κενό δέντρο (NULL) → -1  (βάση αναδρομής)
 *   - Φύλλο              →  0  (1 + max(-1,-1))
 *   - Κόμβος με παιδιά   →  1 + max(ύψος_αριστερά, ύψος_δεξιά)
* ------------------------------------------------------- */
int treeHeight(struct Node* n) {
    /* Βάση αναδρομής: κενό δέντρο έχει ύψος -1 */
    if (n == NULL)
        return -1;

    /* Υπολογισμός ύψους αριστερού και δεξιού υποδέντρου */
    int left  = treeHeight(n->left);
    int right = treeHeight(n->right);

    /* Το ύψος είναι 1 (η τρέχουσα ακμή) + το μέγιστο από τα δύο */
    return 1 + (left > right ? left : right);
}

/* -------------------------------------------------------
 * Βάθους: αριθμός ΑΚΜΩΝ από τη ρίζα ως τον κόμβο.
 *   - Ρίζα          → βάθος 0
 *   - Παιδιά ρίζας  → βάθος 1
 *   - Εγγόνια ρίζας → βάθος 2  κ.ο.κ.
* ------------------------------------------------------- */
void treeDepth(struct Node* n, int depth) {
    /* Βάση αναδρομής: κενός κόμβος, δεν κάνουμε τίποτα */
    if (n == NULL)
        return;

    /* Εκτύπωση τρέχοντος κόμβου και βάθους του */
    printf("Node: %2d,  Depth: %d\n", n->data, depth);

    /* Αναδρομική κλήση για αριστερό παιδί — βάθος αυξάνεται κατά 1 */
    treeDepth(n->left,  depth + 1);

    /* Αναδρομική κλήση για δεξί παιδί — βάθος αυξάνεται κατά 1 */
    treeDepth(n->right, depth + 1);
}

void freeTree(struct Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int main() {
    struct Node* root = NULL;   /* Αρχικά το δέντρο είναι κενό */

    /* Άνοιγμα αρχείου και κατασκευή BST */
    FILE *fptr = fopen("numbers4.txt", "r");
    if (fptr == NULL) {
        printf("Error while opening the file!\n");
        return 1;
    }

    int value;
    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value);
    }
    fclose(fptr);   /* Κλείσιμο αρχείου μετά την ανάγνωση */

    /* Εκτύπωση ύψους δέντρου */
    printf("Tree height = %d\n\n", treeHeight(root));

    /* Εκτύπωση βάθους κάθε κόμβου (preorder: root→left→right) */
    treeDepth(root, 0);

    /* Αποδέσμευση μνήμης */
    freeTree(root);

    return 0;
}