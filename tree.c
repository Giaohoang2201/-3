#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_SIZE 100
#define MAX_PATH 100

/* =========================
   Binary Tree ADT
   ========================= */

typedef struct Node {
    char data;
    struct Node* left;
    struct Node* right;
} Node;

typedef struct {
    Node* root;
    int size;
    int max_size;
} BTree;


/* =========================
   Node
   ========================= */

Node* create_node(char data)
{
    Node* new_node;

    new_node = (Node*)malloc(sizeof(Node));

    if (new_node == NULL) {
        return NULL;
    }

    new_node->data = data;
    new_node->left = NULL;
    new_node->right = NULL;

    return new_node;
}


/* =========================
   Create Binary Tree
   ========================= */

BTree* create_btree(int size)
{
    BTree* tree;

    tree = (BTree*)malloc(sizeof(BTree));

    if (tree == NULL) {
        return NULL;
    }

    tree->root = NULL;
    tree->size = 0;
    tree->max_size = size;

    return tree;
}


/* =========================
   Find Node
   =========================

   Example:
   /A/B/C

   Start from A.
   Then search B among A's children.
   Then search C among B's children.

   Because left and right children
   cannot have the same data,
   the path is unambiguous.
*/

Node* find_node(BTree* tree, const char* path)
{
    Node* current;
    int i;
    int length;

    if (tree == NULL || tree->root == NULL || path == NULL) {
        return NULL;
    }

    length = (int)strlen(path);

    if (length < 2) {
        return NULL;
    }

    if (path[0] != '/') {
        return NULL;
    }

    current = tree->root;

    /* Check root */
    if (path[1] != current->data) {
        return NULL;
    }

    i = 2;

    while (i < length) {

        if (path[i] != '/') {
            return NULL;
        }

        i++;

        if (i >= length) {
            return NULL;
        }

        /* Search left child first */
        if (current->left != NULL &&
            current->left->data == path[i]) {

            current = current->left;
        }
        /* Otherwise search right child */
        else if (current->right != NULL &&
            current->right->data == path[i]) {

            current = current->right;
        }
        else {
            return NULL;
        }

        i++;
    }

    return current;
}


/* =========================
   Validate Path
   ========================= */

int valid_path(const char* path)
{
    int i;
    int length;

    if (path == NULL) {
        return 0;
    }

    length = (int)strlen(path);

    if (length < 2) {
        return 0;
    }

    if (path[0] != '/') {
        return 0;
    }

    /*
       Valid examples:
       /A
       /A/B
       /A/B/C
    */

    if (path[1] < 'A' || path[1] > 'Z') {
        return 0;
    }

    i = 2;

    while (i < length) {

        if (path[i] != '/') {
            return 0;
        }

        i++;

        if (i >= length) {
            return 0;
        }

        if (path[i] < 'A' || path[i] > 'Z') {
            return 0;
        }

        i++;
    }

    return 1;
}


/* =========================
   Insert Root
   ========================= */

int insert_root(BTree* tree, char value)
{
    Node* new_node;

    if (tree == NULL) {
        return 0;
    }

    /* Root already exists */
    if (tree->root != NULL) {
        return 0;
    }

    if (tree->size >= tree->max_size) {
        return 0;
    }

    if (value < 'A' || value > 'Z') {
        return 0;
    }

    new_node = create_node(value);

    if (new_node == NULL) {
        return 0;
    }

    tree->root = new_node;
    tree->size++;

    return 1;
}


/* =========================
   Insert Child
   ========================= */

int insert_child(BTree* tree,
    const char* parent_path,
    char child,
    char value)
{
    Node* parent;
    Node* new_node;

    if (tree == NULL) {
        return 0;
    }

    if (!valid_path(parent_path)) {
        return 0;
    }

    if (child != 'L' && child != 'R') {
        return 0;
    }

    if (value < 'A' || value > 'Z') {
        return 0;
    }

    if (tree->size >= tree->max_size) {
        return 0;
    }

    parent = find_node(tree, parent_path);

    if (parent == NULL) {
        return 0;
    }

    /*
       Assignment requirement:
       Parent must be a leaf.
    */
    if (parent->left != NULL ||
        parent->right != NULL) {

        return 0;
    }

    new_node = create_node(value);

    if (new_node == NULL) {
        return 0;
    }

    if (child == 'L') {
        parent->left = new_node;
    }
    else {
        parent->right = new_node;
    }

    tree->size++;

    return 1;
}


/* =========================
   Find Parent
   ========================= */

Node* find_parent(BTree* tree, const char* path)
{
    char parent_path[MAX_PATH];
    int length;
    int i;

    if (tree == NULL || path == NULL) {
        return NULL;
    }

    length = (int)strlen(path);

    /*
       /A cannot have a parent.
    */
    if (length <= 2) {
        return NULL;
    }

    /*
       Example:
       /A/B/C
       becomes
       /A/B
    */

    for (i = 0; i < length - 2; i++) {
        parent_path[i] = path[i];
    }

    parent_path[length - 2] = '\0';

    return find_node(tree, parent_path);
}


/* =========================
   Delete Node
   ========================= */

int delete_node(BTree* tree, const char* path)
{
    Node* node;
    Node* parent;

    if (tree == NULL || path == NULL) {
        return 0;
    }

    if (!valid_path(path)) {
        return 0;
    }

    node = find_node(tree, path);

    if (node == NULL) {
        return 0;
    }

    /*
       Delete only leaf node.
    */
    if (node->left != NULL ||
        node->right != NULL) {

        return 0;
    }

    /*
       Root is a leaf.
       Delete root -> empty tree.
    */
    if (node == tree->root) {

        free(node);

        tree->root = NULL;
        tree->size--;

        return 1;
    }

    parent = find_parent(tree, path);

    if (parent == NULL) {
        return 0;
    }

    if (parent->left == node) {
        parent->left = NULL;
    }
    else if (parent->right == node) {
        parent->right = NULL;
    }
    else {
        return 0;
    }

    free(node);
    tree->size--;

    return 1;
}


/* =========================
   Check Duplicate Sibling
   ========================= */

int duplicate_sibling(Node* parent,
    Node* target,
    char new_value)
{
    if (parent == NULL) {
        return 0;
    }

    /*
       Target is left child,
       so check right child.
    */
    if (parent->left == target) {

        if (parent->right != NULL &&
            parent->right->data == new_value) {

            return 1;
        }
    }

    /*
       Target is right child,
       so check left child.
    */
    if (parent->right == target) {

        if (parent->left != NULL &&
            parent->left->data == new_value) {

            return 1;
        }
    }

    return 0;
}


/* =========================
   Update Node
   ========================= */

int update_value(BTree* tree,
    const char* path,
    char new_value)
{
    Node* node;
    Node* parent;

    if (tree == NULL || path == NULL) {
        return 0;
    }

    if (!valid_path(path)) {
        return 0;
    }

    if (new_value < 'A' || new_value > 'Z') {
        return 0;
    }

    node = find_node(tree, path);

    if (node == NULL) {
        return 0;
    }

    /*
       Root has no sibling,
       so no duplicate check is needed.
    */
    if (node == tree->root) {

        node->data = new_value;

        return 1;
    }

    parent = find_parent(tree, path);

    if (parent == NULL) {
        return 0;
    }

    /*
       Left and right children of the
       same parent cannot have the same data.
    */
    if (duplicate_sibling(parent,
        node,
        new_value)) {

        return 0;
    }

    node->data = new_value;

    return 1;
}


/* =========================
   Read Children
   ========================= */

int read_child(BTree* tree, const char* path)
{
    Node* parent;
    int printed;

    if (tree == NULL || path == NULL) {
        return 0;
    }

    if (!valid_path(path)) {
        return 0;
    }

    parent = find_node(tree, path);

    if (parent == NULL) {
        return 0;
    }

    printed = 0;

    if (parent->left != NULL) {

        printf("%c(L)", parent->left->data);

        printed = 1;
    }

    if (parent->right != NULL) {

        if (printed) {
            printf(", ");
        }

        printf("%c(R)", parent->right->data);

        printed = 1;
    }

    if (!printed) {
        printf("No children");
    }

    printf("\n");

    return 1;
}


/* =========================
   Print Tree
   =========================

   Required example:

   A
   +---B
       +---D
       +---E
   +---C
*/

void print_tree_node(Node* node, int depth)
{
    int i;

    if (node == NULL) {
        return;
    }

    /*
       Print current node
    */
    for (i = 0; i < depth; i++) {
        printf("    ");
    }

    if (depth > 0) {
        printf("+---");
    }

    printf("%c\n", node->data);

    /*
       Left subtree
    */
    print_tree_node(node->left, depth + 1);

    /*
       Right subtree
    */
    print_tree_node(node->right, depth + 1);
}


void print_btree(BTree* tree)
{
    if (tree == NULL || tree->root == NULL) {
        printf("Tree is empty\n");
        return;
    }

    print_tree_node(tree->root, 0);
}


/* =========================
   Destroy Tree
   ========================= */

void destroy_nodes(Node* node)
{
    if (node == NULL) {
        return;
    }

    destroy_nodes(node->left);
    destroy_nodes(node->right);

    free(node);
}


void destroy_btree(BTree* tree)
{
    if (tree == NULL) {
        return;
    }

    destroy_nodes(tree->root);

    free(tree);
}


/* =========================
   Main
   ========================= */

int main(void)
{
    BTree* tree;
    char input[256];

    tree = create_btree(MAX_SIZE);

    if (tree == NULL) {
        printf("Memory allocation error\n");
        return 1;
    }

    while (1) {

        char command[20];
        char arg1[MAX_PATH];
        char arg2[20];
        char arg3[20];

        int count;

        printf("> ");

        if (fgets(input, sizeof(input), stdin) == NULL) {
            break;
        }

        input[strcspn(input, "\n")] = '\0';

        if (strlen(input) == 0) {
            continue;
        }

        command[0] = '\0';
        arg1[0] = '\0';
        arg2[0] = '\0';
        arg3[0] = '\0';

        count = sscanf(input,
            "%19s %99s %19s %19s",
            command,
            arg1,
            arg2,
            arg3);


        /* =====================
           INSERT
           ===================== */

        if (strcmp(command, "Insert") == 0 ||
            strcmp(command, "I") == 0) {

            /*
               Insert / A
               Insert /A L B
            */

            if (count == 3 &&
                strcmp(arg1, "/") == 0) {

                if (strlen(arg2) != 1) {
                    printf("Error\n");
                    continue;
                }

                if (insert_root(tree, arg2[0])) {
                    printf("OK\n");
                }
                else {
                    printf("Error\n");
                }
            }

            else if (count == 4) {

                char child;
                char value;

                if (strlen(arg2) != 1 ||
                    strlen(arg3) != 1) {

                    printf("Error\n");
                    continue;
                }

                child = arg2[0];
                value = arg3[0];

                if (insert_child(tree,
                    arg1,
                    child,
                    value)) {

                    printf("OK\n");
                }
                else {
                    printf("Error\n");
                }
            }

            else {
                printf("Error\n");
            }
        }


        /* =====================
           DELETE
           ===================== */

        else if (strcmp(command, "Delete") == 0 ||
            strcmp(command, "D") == 0) {

            if (count != 2) {
                printf("Error\n");
                continue;
            }

            if (delete_node(tree, arg1)) {
                printf("OK\n");
            }
            else {
                printf("Error\n");
            }
        }


        /* =====================
           UPDATE
           ===================== */

        else if (strcmp(command, "Update") == 0 ||
            strcmp(command, "U") == 0) {

            if (count != 3 ||
                strlen(arg2) != 1) {

                printf("Error\n");
                continue;
            }

            if (update_value(tree,
                arg1,
                arg2[0])) {

                printf("OK\n");
            }
            else {
                printf("Error\n");
            }
        }


        /* =====================
           READ
           ===================== */

        else if (strcmp(command, "Read") == 0 ||
            strcmp(command, "R") == 0) {

            if (count != 2) {
                printf("Error\n");
                continue;
            }

            if (!read_child(tree, arg1)) {
                printf("Error\n");
            }
        }


        /* =====================
           PRINT
           ===================== */

        else if (strcmp(command, "Print") == 0 ||
            strcmp(command, "P") == 0) {

            if (count != 1) {
                printf("Error\n");
                continue;
            }

            print_btree(tree);
        }


        /* =====================
           EXIT
           ===================== */

        else if (strcmp(command, "Exit") == 0 ||
            strcmp(command, "Q") == 0 ||
            strcmp(command, "Quit") == 0) {

            if (count != 1) {
                printf("Error\n");
                continue;
            }

            break;
        }


        /* =====================
           INVALID COMMAND
           ===================== */

        else {
            printf("Error\n");
        }
    }

    destroy_btree(tree);

    return 0;
}