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

// Δημιουργία κόμβου
struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

// Εισαγωγή (BST)
struct Node* insert(struct Node* node, int value) {
    if (node == NULL)
        return createNode(value);

    if (value < node->data)
        node->left = insert(node->left, value);
    else if (value > node->data)
        node->right = insert(node->right, value);

    return node;
}

int treeHeight(struct Node* n) {
    if (n == NULL)
        return -1;

    int left = treeHeight(n->left);
    int right = treeHeight(n->right);

    return 1 + (left > right ? left : right);   // max(treeHeight(n->right), treeHeight(n->left))
}

void treeDepth(struct Node* n, int depth) {
    if (n == NULL)
        return;

    printf("Node: %d, Depth: %d\n", n->data, depth);

    treeDepth(n->left, depth + 1);
    treeDepth(n->right, depth + 1);
}

int main() {
    struct Node* root = NULL;  // Αρχικοποιηση ριζας
    FILE *fptr = fopen("numbers4.txt", "r");

    if (fptr == NULL) {
        printf("Error while opening the file!\n");
        return 1;
    }

    int value;

    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value);
    }
    printf("Tree height= %d\n\n", treeHeight(root));
    treeDepth(root, 0);

    return 0;
}
