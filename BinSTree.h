/*
 * =========================================================================
 * MODUL      : BinSTree.h - Header Binary Search Tree (BST)
 * =========================================================================
 * FUNGSI     : Mendefinisikan struktur data TreeNode dan mendeklarasikan
 *              fungsi-fungsi untuk memanipulasi Binary Search Tree.
 *              Tree ini digunakan untuk menyimpan rute halte bus.
 *
 * CARA KERJA :
 *   - Setiap TreeNode mewakili SATU halte dengan: id, name, jalur (flag)
 *   - ID dihitung dari (koridor*100 + indeks_halte) agar unik
 *   - Tree diorganisir sebagai BST: id < root -> kiri, id > root -> kanan
 *   - Mendukung 3 traversal: preorder, inorder, postorder
 *   - Dilengkapi serialisasi ke file (simpan_tree / muat_tree)
 *
 * BERISI     : Deklarasi fungsi (belum implementasi).
 *              Implementasi ada di BinSTree.c
 *
 * DIPAKAI OLEH : connection.c (insert, insert_bentang, destroyTree, dll)
 * MODUL INI INDEPENDEN : tidak bergantung pada modul lain (self-contained)
 * =========================================================================
 */

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
TreeNode* muat_tree(const char *filename);

#endif