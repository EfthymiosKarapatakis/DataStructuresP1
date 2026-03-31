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

// Node functions

struct Node* createNode(int value);
struct Node* insert(struct Node* node, int value);


// Queue functions

struct Queue* createQueue();
void enQueue(struct Queue* q, struct Node* node, int level);
struct QueueNode* deQueue(struct Queue* q);
int isEmpty(struct Queue* q);

// Tree functions

int search(int arr[], int value, int left, int right);
struct Node* buildTreeRecursive(int inorder[], int preorder[], int *preIndex, int left, int right);
struct Node* buildTree(int inorder[], int preorder[]);
int getHeight(struct Node* root, int h);


int max(int a, int b) {
    return (a>b) ? a:b;
}

// Node functions

struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    newNode->data = value;
    newNode->left = NULL;
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

// Queue functions

struct Queue* createQueue() {
    struct Queue* q = (struct Queue*)malloc(sizeof(struct Queue));
    q->front = q->rear = NULL;
    return q;
}

void enQueue(struct Queue* q, struct Node* node, int level) {
    struct QueueNode* temp = (struct QueueNode*)malloc(sizeof(struct QueueNode));
    temp->node = node;
    temp->level = level;
    temp->next = NULL;

    if (q->rear == NULL) {
        q->front= q->rear = temp;
        return;
    }

    q->rear->next = temp;
    q->rear = temp;
}

struct QueueNode* deQueue(struct Queue* q) {
    if (q->front == NULL) {
        return NULL;
    }

    struct QueueNode* temp = q-> front;
    q->front = q->front->next;

    if (q->front == NULL) {
        q->rear = NULL;
    }

    return temp;
}

int isEmpty(struct Queue* q) {
    return q->front == NULL;
}


// Tree functions

int search(int arr[], int value, int left, int right){
    for(int i=left; i<=right; i++) {
        if(arr[i] == value) {
            return i;
        }
    }
    return -1;
}

struct Node* buildTreeRecursive(int inorder[], int preorder[], int *preIndex, int left, int right) {
    
    if (left > right) {
        return NULL;
    }

    int rootVal = preorder[*preIndex];
    (*preIndex)++;

    struct Node* root = createNode(rootVal);

    int index = search(inorder, rootVal, left, right);

    root->left = buildTreeRecursive(inorder, preorder, preIndex, left, index-1);
    root->right = buildTreeRecursive(inorder, preorder, preIndex, index+1,right);

    return root;
}

struct Node* buildTree(int inorder[], int preorder[]) {
    int preIndex = 0;
    struct Node* root = buildTreeRecursive(inorder, preorder, &preIndex, 0, sizeof(preorder)/sizeof(preorder[0]) - 1);

    return root;
}

int getHeight(struct Node* root, int h) {
    if (root == NULL) {
        return h-1;
    }

    return max(getHeight(root->left, h+1), getHeight(root->right, h+1));
}

void levelOrder(struct Node* root) {
    if (root == NULL) {
        return;
    }

    struct Queue* q = createQueue();
    enQueue(q, root, 0);

    int lastLevel = 0;
    int height = getHeight(root,0);

    while(!isEmpty(q)) {
        struct QueueNode* top = deQueue(q);
        struct Node* node = top->node;
        int lvl = top->level;

        if (lvl>lastLevel) {
            printf("\n");
            lastLevel = lvl;
        }

        if (lvl > height) {
            break;
        }

        if (node->data != -1) {
            printf("%d ", node->data);
        } else {
            printf("N ");
        }

        if (node->left == NULL) {
            enqueue(q, createNode(-1), lvl +1);
        } else {
            enqueue(q, node->left, lvl +1);
        }

        if (node->right == NULL) {
            enqueue(q, createNode(-1), lvl +1);
        } else {
            enqueue(q, node->right, lvl +1);
        }

        free(top);
    }
}


int main() {
    struct Node* root = NULL;  // Αρχικοποιηση ριζας
    FILE *fptr1 = fopen("preorder.txt", "r");
    int preorder[BUFFER];

    if (fptr1 == NULL) {
        printf("Error!\n");
        return 1;
    }

    int value;
    int i=0;

    while (fscanf(fptr1, "%d", &value) == 1) {
        preorder[i] = value;
        i++;
    }

    FILE *fptr2 = fopen("inorder.txt", "r");
    int inorder[BUFFER];

    if (fptr2 == NULL) {
        printf("Error!\n");
        return 1;
    }

    i=0;

    while (fscanf(fptr2, "%d", &value) == 1) {
        inorder[i] = value;
        i++;
    }

    root = buildTree(inorder, preorder);

    return 0;
}
