#ifndef BINARYTREE_H
#define BINARYTREE_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*
 * ==================== MODUL: TREE NODE (SEQUENTIAL CHAIN) ====================
 * Menyediakan tipe data TreeNode dan operasi dasar untuk membangun
 * sequential chain (right-skewed tree) yang merepresentasikan rute bus.
 *
 * Berbeda dengan BST biasa — parent pointer menunjuk ke node SEBELUMNYA
 * dalam urutan rute, bukan parent berdasarkan urutan ID.
 * =============================================================================
 */

/*
 * TreeNode — node sequential chain
 * - id: identifier (format: koridor*100 + index_halte)
 * - name[100]: nama halte
 * - jalur: 0 = halte biasa, 1 = titik transit
 * - leftChild: selalu NULL (tidak digunakan)
 * - rightChild: pointer ke node berikutnya dalam urutan rute
 * - parent: pointer ke node sebelumnya dalam urutan rute (untuk backtrack)
 */
typedef struct TreeNode {
    int id;
    char name[100];
    int jalur;

    struct TreeNode* leftChild;
    struct TreeNode* rightChild;
    struct TreeNode* parent;
} TreeNode;

/*
 * createNode: Konstruktor node.
 * Param id, name, jalur — nilai awal node.
 * Return pointer ke node baru (heap).
 * Dipanggil oleh: insert_bentang() di connection.c
 */
TreeNode* createNode(int id, char name[], int jalur);

/* ===== Operasi BST (tidak dipakai di alur utama) ===== */
TreeNode* insert(TreeNode* root, int id, char name[], int jalur);
void insertLeft(TreeNode* parent, TreeNode* child);
void insertRight(TreeNode* parent, TreeNode* child);

/* ===== Traversal ===== */
void preorder(TreeNode* root);
void inorder(TreeNode* root);
void postorder(TreeNode* root);

/* ===== Utilitas (tidak dipakai di alur utama) ===== */
int isLeaf(TreeNode* node);
int height(TreeNode* root);
TreeNode* search(TreeNode* root, int id);
void printTree(TreeNode* root, int level);

/*
 * destroyTree: Hapus seluruh node tree dari heap (postorder).
 * Dipanggil oleh: build_tree() di connection.c
 */
void destroyTree(TreeNode* root);

/*
 * simpan_tree: Simpan tree ke file (format preorder terindentasi).
 * Param filename: path file output.
 * Dipanggil oleh: build_tree() di connection.c
 */
void simpan_tree(const char *filename, TreeNode *root);

#endif