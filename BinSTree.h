#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct TreeNode {
    int id;
    char name[100];
    int jalur;

    struct TreeNode* leftChild;
    struct TreeNode* rightChild;
    struct TreeNode* parent;
} TreeNode;

/* Konstruktor */
TreeNode* createNode(int id, char name[], int jalur);

/* Operasi insert */
TreeNode* insert(TreeNode* root, int id, char name[], int jalur);
void insertLeft(TreeNode* parent, TreeNode* child);
void insertRight(TreeNode* parent, TreeNode* child);

/* Traversal */
void preorder(TreeNode* root);
void inorder(TreeNode* root);
void postorder(TreeNode* root);

/* Utilitas */
int isLeaf(TreeNode* node);
int height(TreeNode* root);
TreeNode* search(TreeNode* root, int id);
void printTree(TreeNode* root, int level);

/* Hapus seluruh tree */
void destroyTree(TreeNode* root);

/* Serialisasi tree ke file */
void simpan_tree(const char *filename, TreeNode *root);

#endif