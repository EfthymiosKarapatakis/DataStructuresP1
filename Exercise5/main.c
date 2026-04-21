/*
 * Exercise 5
 *
 * (Ανάπλαση από διαπεράσεις δυαδικού δένδρου) Εξηγήστε πως αν έχουμε στη
 * διάθεσή μας μία μεταδιαπέραση (μεταδιάταξη) και μία ενδοδιαπέραση
 * (ενδοδιάταξη) τότε μπορούμε να αναπλάσουμε την μορφή του δυαδικού δένδρου.
 * Υλοποιήστε κυρίως πρόγραμμα το οποίο διαβάζει μία ενδοδιαπέραση και μία
 * μεταδιαπέραση. Θεωρούμε για απλότητα ότι το δένδρο περιέχει χαρακτήρες
 * μοναδικούς μεταξύ τους, οπότε οι δύο διαπεράσεις μπορούν να διαβαστούν και να
 * αποθηκευτούν σαν δύο συμβολοσειρές. Επίσης θεωρούμε ότι ανάπλαση δένδρου
 * σημαίνει εύρεση για κάθε κόμβο αριστερού και δεξιού παιδιού και επιστροφή της ρίζας.
 *
 * Κάντε το ίδιο θεωρώντας ότι οι διαπεράσεις που έχουμε διαθέσιμες είναι
 * προδιαπέραση και όχι μεταδιαπέραση.
 *
 * Επιβεβαιώστε το αποτέλεσμα, καλώντας μία από τις συναρτήσεις διαπέρασης που
 * ορίσαμε στο ερώτημα 4.
 *
 * Τεστάρετε τη συνάρτηση με ένα κυρίως πρόγραμμα με δεδομένα της επιλογής σας και
 * δοκιμάστε ενδεχομένως και διαφορετικές υλοποιήσεις
*/

#include <stdio.h>
#include <stdlib.h>

#define BUFFER 100

struct Node {
    char data;
    struct Node* left;
    struct Node* right;
};

/* Πρότυπα συναρτήσεων */
struct Node* createNode(char value);
void freeTree(struct Node* node);
int search(char arr[], char value, int left, int right);
struct Node* buildTreePreRecursive(char inorder[], char preorder[], int *preIndex, int left, int right);
struct Node* buildPreTree(char inorder[], char preorder[], int size);
struct Node* buildTreePostRecursive(char inorder[], char postorder[], int *postIndex, int left, int right);
struct Node* buildPostTree(char inorder[], char postorder[], int size);
void inorderPrint(struct Node* root);
void preorderPrint(struct Node* root);
void postorderPrint(struct Node* root);

struct Node* createNode(char value) {
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

void freeTree(struct Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

/* inorder: left → root → right */
void inorderPrint(struct Node* root) {
    if (root == NULL) return;
    inorderPrint(root->left);
    printf("%c ", root->data);
    inorderPrint(root->right);
}

/* Preorder: root → left → right */
void preorderPrint(struct Node* root) {
    if (root == NULL) return;
    printf("%c ", root->data);
    preorderPrint(root->left);
    preorderPrint(root->right);
}

/* postorder: left → right → root */
void postorderPrint(struct Node* root) {
    if (root == NULL) return;
    postorderPrint(root->left);
    postorderPrint(root->right);
    printf("%c ", root->data);
}


/* =======================================================
 * ΒΟΗΘΗΤΙΚΗ: search
 * Βρίσκει τη θέση του value στο arr[left..right]
 * Χρησιμοποιείται για να εντοπίσουμε τη ρίζα στο inorder,
 * ώστε να γνωρίζουμε πόσοι κόμβοι ανήκουν στο αριστερό
 * και πόσοι στο δεξί υποδέντρο.
 * ======================================================= */
int search(char arr[], char value, int left, int right) {
    for (int i = left; i <= right; i++) {
        if (arr[i] == value) return i;
    }
    return -1;
}


/* =======================================================
 * ΑΝΑΚΑΤΑΣΚΕΥΗ ΑΠΟ PREORDER + INORDER
 *  - Το preorder[preIndex] είναι η ρίζα του τρέχοντος υποδέντρου
 *  - Βρίσκουμε τη ρίζα στο inorder[left..right]
 *  - Ό,τι είναι ΑΡΙΣΤΕΡΑ της ρίζας στο inorder → αριστερό υποδέντρο
 *  - Ό,τι είναι ΔΕΞΙΑ  της ρίζας στο inorder → δεξί υποδέντρο
 *  - Αναδρομή και για τα δύο υποδέντρα
 * ======================================================= */
struct Node* buildTreePreRecursive(char inorder[], char preorder[], int *preIndex, int left, int right) {
    /* Βάση αναδρομής: κενό υποδέντρο */
    if (left > right) return NULL;

    /* Η τρέχουσα ρίζα είναι το επόμενο στοιχείο του preorder */
    char rootVal = preorder[*preIndex];
    (*preIndex)++;

    struct Node* root = createNode(rootVal);

    /* Βρίσκουμε τη ρίζα στο inorder για να χωρίσουμε σε αριστερό/δεξί */
    int index = search(inorder, rootVal, left, right);

    /* Πρώτα αριστερά (το preorder συνεχίζει αριστερά-πρώτα) */
    root->left  = buildTreePreRecursive(inorder, preorder, preIndex, left, index - 1);
    root->right = buildTreePreRecursive(inorder, preorder, preIndex, index + 1, right);

    return root;
}

struct Node* buildPreTree(char inorder[], char preorder[], int size) {
    int preIndex = 0;   /* Ξεκινάμε από το πρώτο στοιχείο του preorder */
    return buildTreePreRecursive(inorder, preorder, &preIndex, 0, size - 1);
}


/* =======================================================
 * ΑΝΑΚΑΤΑΣΚΕΥΗ ΑΠΟ POSTORDER + INORDER
 *  - Το postorder[postIndex] είναι η ρίζα (διαβάζουμε ΑΝAΠΟΔΑ)
 *  - Βρίσκουμε τη ρίζα στο inorder[left..right]
 *  - ΠΡΟΣΟΧΗ: Επειδή το postorder είναι left→right→root,
 *    διαβάζουμε ΠΡΩΤΑ το δεξί υποδέντρο και ΜΕΤΑ το αριστερό
 *    (ανάποδη σειρά από το preorder)
 * ======================================================= */
struct Node* buildTreePostRecursive(char inorder[], char postorder[], int *postIndex, int left, int right) {
    /* Βάση αναδρομής: κενό υποδέντρο */
    if (left > right) return NULL;

    /* Η τρέχουσα ρίζα είναι το τελευταίο αδιάβαστο στοιχείο του postorder */
    char rootVal = postorder[*postIndex];
    (*postIndex)--;

    struct Node* root = createNode(rootVal);

    /* Βρίσκουμε τη ρίζα στο inorder */
    int index = search(inorder, rootVal, left, right);

    /* ΠΡΟΣΟΧΗ: Πρώτα δεξιά, μετά αριστερά (λόγω ανάποδης ανάγνωσης) */
    root->right = buildTreePostRecursive(inorder, postorder, postIndex, index + 1, right);
    root->left  = buildTreePostRecursive(inorder, postorder, postIndex, left, index - 1);

    return root;
}

struct Node* buildPostTree(char inorder[], char postorder[], int size) {
    int postIndex = size - 1;   /* Ξεκινάμε από το ΤΕΛΕΥΤΑΙΟ στοιχείο του postorder */
    return buildTreePostRecursive(inorder, postorder, &postIndex, 0, size - 1);
}

int main() {
    int size = 0;
    FILE *fptr;

    char inorder[BUFFER];
    char preorder[BUFFER];
    char postorder[BUFFER];

    /* --- Ανάγνωση inorder --- */
    fptr = fopen("inorder.txt", "r");
    if (!fptr) { printf("Error opening inorder.txt!\n"); return 1; }
    int i = 0;
    while (fscanf(fptr, " %c", &inorder[i]) == 1) { i++; size++; }
    fclose(fptr);

    /* --- Ανάγνωση preorder --- */
    fptr = fopen("preorder.txt", "r");
    if (!fptr) { printf("Error opening preorder.txt!\n"); return 1; }
    i = 0;
    while (fscanf(fptr, " %c", &preorder[i]) == 1) { i++; }
    fclose(fptr);

    /* --- Ανάγνωση postorder --- */
    fptr = fopen("postorder.txt", "r");
    if (!fptr) { printf("Error opening postorder.txt!\n"); return 1; }
    i = 0;
    while (fscanf(fptr, " %c", &postorder[i]) == 1) { i++; }
    fclose(fptr);


    /* --- Ανακατασκευή από preorder + inorder --- */
    struct Node* preRoot = buildPreTree(inorder, preorder, size);

    /* Επιβεβαίωση: αν η ανακατασκευή είναι σωστή, το inorder
     * του δέντρου πρέπει να ταιριάζει με το αρχικό inorder */
    printf("=== Tree from PREORDER + INORDER ===\n");
    printf("Inorder: "); inorderPrint(preRoot);
    printf("\nPreorder: "); preorderPrint(preRoot);
    printf("\nPostorder: "); postorderPrint(preRoot);
    printf("\n\n");


    /* --- Ανακατασκευή από postorder + inorder --- */
    struct Node* postRoot = buildPostTree(inorder, postorder, size);

    printf("=== Tree from POSTORDER + INORDER ===\n");
    printf("Inorder: "); inorderPrint(postRoot);
    printf("\nPreorder: "); preorderPrint(postRoot);
    printf("\nPostorder: "); postorderPrint(postRoot);
    printf("\n");


    /* Αποδέσμευση μνήμης */
    freeTree(preRoot);
    freeTree(postRoot);

    return 0;
}