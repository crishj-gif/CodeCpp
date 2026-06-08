#include <stdio.h>
#include <stdlib.h>


typedef struct Node {
    int data;
    struct Node *left, *right;
} Node;
typedef Node* Pnode;

Pnode createNode(int data) {
    Pnode newNode = (Pnode)malloc(sizeof(Node));
    newNode->data = data;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

Pnode insert(Pnode root, int data) {
    if (root == NULL) {
        return createNode(data);
    }
    if (data < root->data) {
        root->left = insert(root->left, data);
    } else if (data > root->data) {
        root->right = insert(root->right, data);
    }
    return root;
}

Pnode search(Pnode root, int data) {
    if (root == NULL || root->data == data) {
        return root;
    }
    if (data < root->data) {
        return search(root->left, data);
    }
    return search(root->right, data);
}

void NLR(Pnode root) {
    if (root != NULL) {
        printf("%d ", root->data);
        NLR(root->left);           
        NLR(root->right);          
    }
}


void LNR(Pnode root) {
    if (root != NULL) {
        LNR(root->left);           
        printf("%d ", root->data); 
        LNR(root->right);          
    }
}
void LRN(Pnode root) {
    if (root != NULL) {
        LRN(root->left);           
        LRN(root->right);          
        printf("%d ", root->data); 
    }
}

int main() {
    Pnode root = NULL;
    root = insert(root, 50);
    insert(root, 30);
    insert(root, 20);
    insert(root, 40);
    insert(root, 70);
    insert(root, 60);
    insert(root, 80);

    printf("NLR: ");
    NLR(root);
    printf("\n");

    printf("LNR: ");
    LNR(root);
    printf("\n");

    printf("LRN: ");
    LRN(root);
    printf("\n");

    Pnode result = search(root, 40);
    if (result != NULL) {
        printf("%d\n", result->data);
    } else {
        printf("Not found\n");
    }

    return 0;
}
