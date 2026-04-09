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

struct QueueNode {
    struct Node* node;
    int level;
    struct QueueNode* next;
};

struct Queue {
    struct QueueNode *front, *rear;
};

int max(int a, int b);
void inorderPrint(struct Node* root);

// Node functions
struct Node* createNode(char value);
struct Node* insert(struct Node* node, char value);

// Queue functions
struct Queue* createQueue();
void enQueue(struct Queue* q, struct Node* node, int level);
struct QueueNode* deQueue(struct Queue* q);
int isEmpty(struct Queue* q);

// Tree functions
int search(char arr[], char value, int left, int right);
struct Node* buildTreePreRecursive(char inorder[], char preorder[], int *preIndex, int left, int right);
struct Node* buildPreTree(char inorder[], char preorder[], int size);
int getHeight(struct Node* root, int h);
struct Node* buildTreePostRecursive(char inorder[], char postorder[], int *postIndex, int left, int right);
struct Node* buildPostTree(char inorder[], char postorder[], int size);

// ================= FUNCTIONS =================

int max(int a, int b) {
    return (a > b) ? a : b;
}

void inorderPrint(struct Node* root) {
    if(root == NULL) return;

    inorderPrint(root->left);
    printf("%c ", root->data);
    inorderPrint(root->right);
}

// Node

struct Node* createNode(char value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* insert(struct Node* node, char value) {
    if (node == NULL)
        return createNode(value);

    if (value < node->data)
        node->left = insert(node->left, value);
    else if (value > node->data)
        node->right = insert(node->right, value);

    return node;
}

// Queue

struct Queue* createQueue() {
    struct Queue* q = malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue* q, struct Node* node, int level) {
    struct QueueNode* temp = malloc(sizeof(struct QueueNode));
    temp->node = node;
    temp->level = level;
    temp->next = NULL;

    if (q->rear == NULL) {
        q->front = q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

struct QueueNode* deQueue(struct Queue* q) {
    if (q->front == NULL) return NULL;

    struct QueueNode* temp = q->front;
    q->front = q->front->next;

    if (q->front == NULL)
        q->rear = NULL;

    return temp;
}

int isEmpty(struct Queue* q) {
    return q->front == NULL;
}

// ================= TREE =================

int search(char arr[], char value, int left, int right){
    for(int i = left; i <= right; i++) {
        if(arr[i] == value) return i;
    }
    return -1;
}

// PREORDER

struct Node* buildTreePreRecursive(char inorder[], char preorder[], int *preIndex, int left, int right) {

    if (left > right) return NULL;

    char rootVal = preorder[*preIndex];
    (*preIndex)++;

    struct Node* root = createNode(rootVal);

    int index = search(inorder, rootVal, left, right);

    root->left = buildTreePreRecursive(inorder, preorder, preIndex, left, index - 1);
    root->right = buildTreePreRecursive(inorder, preorder, preIndex, index + 1, right);

    return root;
}

struct Node* buildPreTree(char inorder[], char preorder[], int size) {
    int preIndex = 0;
    return buildTreePreRecursive(inorder, preorder, &preIndex, 0, size - 1);
}

// POSTORDER

struct Node* buildTreePostRecursive(char inorder[], char postorder[], int *postIndex, int left, int right) {

    if(left > right) return NULL;

    char rootVal = postorder[*postIndex];
    (*postIndex)--;

    struct Node* root = createNode(rootVal);

    int index = search(inorder, rootVal, left, right);

    root->right = buildTreePostRecursive(inorder, postorder, postIndex, index + 1, right);
    root->left = buildTreePostRecursive(inorder, postorder, postIndex, left, index - 1);

    return root;
}

struct Node* buildPostTree(char inorder[], char postorder[], int size){
    int postIndex = size - 1;
    return buildTreePostRecursive(inorder, postorder, &postIndex, 0, size - 1);
}

// OPTIONAL (LEVEL ORDER FIXED PRINT)

int getHeight(struct Node* root, int h) {
    if (root == NULL) return h - 1;
    return max(getHeight(root->left, h+1), getHeight(root->right, h+1));
}

// ================= MAIN =================

int main() {
    int size = 0;
    FILE *fptr;

    char inorder[BUFFER];
    char preorder[BUFFER];
    char postorder[BUFFER];

    // INORDER
    fptr = fopen("inorder.txt", "r");
    if (!fptr) {
        printf("Error!\n");
        return 1;
    }

    int i = 0;
    while (fscanf(fptr, " %c", &inorder[i]) == 1) {
        i++;
        size++;
    }
    fclose(fptr);

    // PREORDER
    fptr = fopen("preorder.txt", "r");
    if (!fptr) {
        printf("Error!\n");
        return 1;
    }

    i = 0;
    while (fscanf(fptr, " %c", &preorder[i]) == 1) {
        i++;
    }
    fclose(fptr);

    // POSTORDER
    fptr = fopen("postorder.txt", "r");
    if (!fptr) {
        printf("Error!\n");
        return 1;
    }

    i = 0;
    while (fscanf(fptr, " %c", &postorder[i]) == 1) {
        i++;
    }
    fclose(fptr);

    // BUILD TREES

    struct Node* preRoot = buildPreTree(inorder, preorder, size);
    printf("Tree from preorder:\n");
    inorderPrint(preRoot);

    printf("\n\n");

    struct Node* postRoot = buildPostTree(inorder, postorder, size);
    printf("Tree from postorder:\n");
    inorderPrint(postRoot);

    printf("\n");

    return 0;
}