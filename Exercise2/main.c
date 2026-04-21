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

/*
 * Επιλογή: Κομβοπροσανατολισμένο δέντρο (Node-oriented BST)
 * Οι τιμές αποθηκεύονται σε ΟΛΟΥΣ τους κόμβους (εσωτερικούς + φύλλα),
 * όχι μόνο στα φύλλα όπως θα γινόταν στο φυλλοπροσανατολισμένο.
 *
 * Ιδιότητα BST: για κάθε κόμβο Χ:
 *   - όλοι οι κόμβοι στο ΑΡΙΣΤΕΡΟ υποδέντρο έχουν τιμή < X
 *   - όλοι οι κόμβοι στο ΔΕΞΙ υποδέντρο έχουν τιμή > X
 * Αυτό επιτρέπει αναζήτηση σε O(log n) για ισορροπημένο δέντρο.
*/

#include <stdio.h>
#include <stdlib.h>

struct Node {
    int data;
    struct Node* left;
    struct Node* right;
};

/* Πρότυπα συναρτήσεων (forward declarations) */
struct Node* binaryTreeSearch(struct Node* node, int target);
struct Node* findMin(struct Node* node);
struct Node* insert(struct Node* node, int value);
struct Node* createNode(int value);
struct Node* deleteNode(struct Node* node, int value);

struct Node* binaryTreeSearch(struct Node* node, int target) {
    /* Βάση αναδρομής: φτάσαμε σε κενό κόμβο → το target δεν υπάρχει */
    if (node == NULL) {
        return NULL;
    }

    /* Βρήκαμε το target στον τρέχοντα κόμβο */
    if (node->data == target) {
        return node;
    }

    /* Το target είναι μικρότερο → συνεχίζουμε αναδρομικά αριστερά */
    if (target < node->data) {
        return binaryTreeSearch(node->left, target);
    } else {
        /* Το target είναι μεγαλύτερο → συνεχίζουμε αναδρομικά δεξιά */
        return binaryTreeSearch(node->right, target);
    }
}

struct Node* insert(struct Node* node, int value) {
    /* Βρήκαμε την κατάλληλη κενή θέση → δημιουργούμε και τοποθετούμε νέο κόμβο */
    if (node == NULL) {
        return createNode(value);
    }

    if (value < node->data) {
        /* Η νέα τιμή ανήκει στο αριστερό υποδέντρο */
        node->left = insert(node->left, value);
    } else if (value > node->data) {
        /* Η νέα τιμή ανήκει στο δεξί υποδέντρο */
        node->right = insert(node->right, value);
    } else {
        /* Η τιμή υπάρχει ήδη στο δέντρο → αγνοούμε (δεν επιτρέπουμε διπλότυπα) */
        printf("The value %d is already in the tree!\n", value);
    }
    return node;
}

struct Node* createNode(int value) {
    struct Node* newNode = malloc(sizeof(struct Node));
    if (newNode == NULL) {
        /* Αποτυχία δέσμευσης μνήμης → τερματισμός με μήνυμα σφάλματος */
        fprintf(stderr, "Memory failed\n");
        exit(EXIT_FAILURE);
    }
    newNode->data  = value;
    newNode->left  = NULL;  /* Αρχικά δεν έχει αριστερό παιδί */
    newNode->right = NULL;  /* Αρχικά δεν έχει δεξί παιδί */
    return newNode;
}

struct Node* findMin(struct Node* node) {
    if (node == NULL) {
        return NULL;
    }
    /* Συνεχίζουμε αριστερά μέχρι να μην υπάρχει αριστερό παιδί */
    while (node->left != NULL) {
        node = node->left;
    }
    return node;  /* Αυτός είναι ο κόμβος με την ελάχιστη τιμή */
}

struct Node* deleteNode(struct Node* node, int value) {

    /* Η τιμή δεν βρέθηκε στο δέντρο */
    if (node == NULL) {
        return NULL;
    }

    /* Ψάχνουμε αναδρομικά για τον κόμβο προς διαγραφή */
    if (value < node->data) {
        /* Η τιμή βρίσκεται στο αριστερό υποδέντρο */
        node->left = deleteNode(node->left, value);
    }
    else if (value > node->data) {
        /* Η τιμή βρίσκεται στο δεξί υποδέντρο */
        node->right = deleteNode(node->right, value);
    }
    else {
        /* Βρήκαμε τον κόμβο προς διαγραφή */

        /* Περίπτωση 1: Φύλλο – δεν έχει παιδιά */
        if (node->left == NULL && node->right == NULL) {
            free(node);
            return NULL;    /* Ο γονέας θα λάβει NULL ως νέο παιδί */
        }

        /* Περίπτωση 2α: Έχει μόνο αριστερό παιδί */
        else if (node->left != NULL && node->right == NULL) {
            struct Node* temp = node->left; /* Κρατάμε τον δείκτη του παιδιού */
            free(node);
            return temp;    /* Ο γονέας συνδέεται απευθείας με το παιδί */
        }

        /* Περίπτωση 2β: Έχει μόνο δεξί παιδί */
        else if (node->left == NULL && node->right != NULL) {
            struct Node* temp = node->right;
            free(node);
            return temp;
        }

        /* Περίπτωση 3: Έχει και τα δύο παιδιά */
        else {
            /* Βρίσκουμε τον inorder successor (τον ελάχιστο του δεξια) */
            struct Node* successor = findMin(node->right);

            /* Αντιγράφουμε την τιμή του successor στον τρέχοντα κόμβο */
            node->data = successor->data;

            /* Διαγράφουμε τον successor από το δεξί υποδέντρο
             * (ο successor έχει το πολύ ένα παιδί, οπότε είναι απλή διαγραφή) */
            node->right = deleteNode(node->right, successor->data);
        }
    }

    return node;    /* Επιστρέφουμε τον ενημερωμένο κόμβο στον γονέα */
}

void freeTree(struct Node* node) {
    // freeTree: Αποδέσμευση όλης της μνήμης του δέντρου
    // Χρησιμοποιεί postorder διάσχιση (αριστερά → δεξιά → ρίζα)
    // ώστε να απελευθερώνουμε πρώτα τα παιδιά και μετά τον γονέα.
    // Αν κάναμε το αντίθετο, θα χάναμε τους δείκτες στα παιδιά.
    if (node == NULL) return;
    freeTree(node->left);   /* Πρώτα το αριστερό υποδέντρο */
    freeTree(node->right);  /* Μετά το δεξί υποδέντρο */
    free(node);             /* Τέλος ο ίδιος ο κόμβος */
}

int main() {
    struct Node* root = NULL;   /* Αρχικά το δέντρο είναι κενό */

    /* Διάβασμα αρχικών τιμών από αρχείο */
    FILE *fptr = fopen("numbers2.txt", "r");
    if (fptr == NULL) {
        printf("Error!\n");
        return 1;
    }

    /* Εισαγωγή κάθε ακεραίου του αρχείου στο BST */
    int value;
    while (fscanf(fptr, "%d", &value) == 1) {
        root = insert(root, value);
    }
    fclose(fptr);

    /* Μενού επιλογών για αλληλεπίδραση με το δέντρο */
    int epilogh = 1;
    int target, insertnode, deletenode;

    while (epilogh != 4) {
        printf("Search(1), Insert(2), Delete(3), Exit(4): ");
        fflush(stdout);
        scanf("%d", &epilogh);

        switch (epilogh) {
            case 1: {
                /* Αναζήτηση: ο χρήστης δίνει τιμή και βλέπει αν υπάρχει */
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
                /* Εισαγωγή: νέος κόμβος τοποθετείται στη σωστή θέση BST */
                printf("Enter node to insert: ");
                scanf("%d", &insertnode);
                root = insert(root, insertnode);
                break;
            }
            case 3: {
                /* Διαγραφή: αφαίρεση κόμβου διατηρώντας την ιδιότητα BST */
                printf("Enter node to delete: ");
                scanf("%d", &deletenode);
                root = deleteNode(root, deletenode);
                break;
            }
            case 4:
                break;
            default:
                printf("Invalid choice\n");
                break;
        }
    }
    /* Αποδέσμευση όλης της δυναμικής μνήμης πριν τον τερματισμό */
    freeTree(root);
    return 0;
}