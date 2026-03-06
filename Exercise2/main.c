/*
 * Exercise 2
 *
 * Υλοποιήστε συνάρτηση με το όνομα binaryTreeSearch που επιχειρεί να εντοπίσει μια
 * καθορισμένη τιμή σε ένα δυαδικό δέντρο αναζήτησης με ακεραίους σαν περιεχόμενο.
 *
 * Η συνάρτηση θα πρέπει να λαμβάνει ως ορίσματα έναν δείκτη στη ρίζα του δυαδικού
 * δέντρου και ένα κλειδί αναζήτησης που πρέπει να εντοπιστεί. Εάν βρεθεί ο κόμβος που
 * περιέχει το κλειδί αναζήτησης, η συνάρτηση θα πρέπει να επιστρέψει έναν δείκτη σε
 * αυτόν τον κόμβο. Διαφορετικά, η συνάρτηση θα πρέπει να επιστρέψει έναν δείκτη NULL.
 *
 * Υλοποιήστε επίσης συναρτήσεις ένθεσης και διαγραφής τιμής από το δέντρο
 *
 * Τεστάρετε τη συνάρτηση με ένα κυρίως πρόγραμμα με δεδομένα της επιλογής σας.
 * Υλοποιήστε είτε φυλλοπροσανατολισμένο είτε κομβοπροσανατολισμένο δέντρο.
 *
 * Εξηγήστε (θεωρητικά) τη διαφοροποίηση στις δύο επιλογές υλοποίησης και πως
 * θα άλλαζε ο κώδικας αν επιλέγατε διαφορετική υλοποίηση.
*/

//                               κομβοπροσανατολισμένο
#include <stdio.h>
#include <stdlib.h>


struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

struct Node* binaryTreeSearch(struct Node* node, int target);
struct Node* findMin(struct Node* node);
struct Node* insert(struct Node* node, int value);
struct Node* createNode(int value);
struct Node* deleteNode(struct Node* node, int value);


struct Node* binaryTreeSearch(struct Node* node, int target) {
    // Αν ειναι κενο το δεντρο επεστρεψε NULL
    if (node == NULL) {
        return NULL;
    }

    // Αν το target ειναι το ορισμα τησ συναρτισης επεστρεψε το
    if (node->data == target) {
        return node;
    }

    // Αν το target ειναι μικροτερο απο το ορισμα πηγαινε στο αριστερο παιδι
    if (target < node->data) {
        return binaryTreeSearch(node->left, target);
    } else {    // Αλλιως αν το target ειναι μεγαλυτερο απο το ορισμα πηγαινε στο δεξι παιδι
        return binaryTreeSearch(node->right, target);
    }
}


struct Node* insert(struct Node* node, int value) {
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->data) {
        node->left = insert(node->left, value);
    }

    else if (value > node->data) {
        node->right = insert(node->right, value);
    }

    // todo
    // Αν υπαρχει duplicate τοτε?
    // Option A: το αγνοω
    // Option B: το επιτρεπω
    /*
    else if (value == tree->data) {

    }
    */
    return node;
}

struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data = value;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct Node* findMin(struct Node* node) {
    if (node == NULL) {
        return NULL;
    }
    while (node->left != NULL) {
        node = node->left;
    }
    return node;
}

struct Node* deleteNode(struct Node* node, int value) {

    if (node == NULL) {
        return NULL;
    }

    if (value < node->data) {
        node->left = deleteNode(node->left, value);
    }
    else if (value > node->data) {
        node->right = deleteNode(node->right, value);
    }
    else {

        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;
        }

        else if (node->left != NULL && node->right == NULL) {
            struct Node* temp = node->left;
            free(node);
            return temp;
        }

        else if (node->left == NULL && node->right != NULL) {
            struct Node* temp = node->right;
            free(node);
            return temp;
        }
        else {
            struct Node* successor = findMin(node->right);
            node->data = successor->data;
            node->right = deleteNode(node->right, successor->data);
        }
    }

    return node;
}

void freeTree(struct Node* node) {
    if (node == NULL) return;
    freeTree(node->left);
    freeTree(node->right);
    free(node);
}

int main() {
    struct Node* root = NULL;  // Αρχικοποιηση ριζας
    FILE *fptr = fopen("numbers.txt", "r");

    if (fptr == NULL) {
        printf("Error!\n");
        return 1;
    }

    int value;
    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value);
    }
    int epilogh = 1;

    int target;
    int insertnode;
    int deletenode;

    while (epilogh != 4) {
        printf("Search(1), Insert(2), Delete(3), Exit(4): ");
        fflush(stdout);
        scanf("%d", &epilogh);
        switch (epilogh) {
            case 1: {
                printf("Enter target: ");
                scanf("%d", &target);

                struct Node* result = binaryTreeSearch(root, target);
                if (result != NULL) {
                    printf("Found: %d\n", result->data);
                } else {
                    printf("Not found\n");
                }
                break;
            }
            case 2: {
                printf("Enter node to insert: ");
                scanf("%d", &insertnode);
                root = insert(root, insertnode);
                break;
            }
            case 3: {
                printf("Enter node to delete: ");
                scanf("%d", &deletenode);
                root = deleteNode(root, deletenode);
                break;
            }
            default:
                printf("Not found\n");
                break;
        }
    }
    printf("Have a nice day!");
    fclose(fptr);
    freeTree(root);
    return 0;
}