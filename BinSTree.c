#include "BinSTree.h"

/*
 * ==================== MODUL: IMPLEMENTASI TREE NODE ====================
 * Implementasi fungsi-fungsi yang dideklarasikan di BinSTree.h.
 * =======================================================================
 */

/*
 * createNode: Konstruktor node sequential chain.
 * Param id: identifier (koridor*100 + index), name: nama halte,
 *           jalur: 0=biasa, 1=transit.
 * Return: pointer ke node baru (alokasi heap).
 * Memanggil: malloc().
 * Dipanggil oleh: insert_bentang() di connection.c.
 */
TreeNode* createNode(int id, char name[], int jalur)
{
    TreeNode* node = (TreeNode*)malloc(sizeof(TreeNode));

    node->id = id;
    strcpy(node->name, name);
    node->jalur = jalur;

    node->leftChild = NULL;
    node->rightChild = NULL;
    node->parent = NULL;

    return node;
}

/*
 * insert: Insert node ke BST berdasarkan aturan BST (id).
 * TIDAK DIPAKAI di alur utama (digantikan insert_bentang + sequential chain).
 * Dipertahankan sebagai referensi.
 */
TreeNode* insert(TreeNode* root,
                 int id,
                 char name[],
                 int jalur)
{
    if(root == NULL)
        return createNode(id, name, jalur);

    if(id < root->id)
    {
        root->leftChild =
            insert(root->leftChild, id, name, jalur);

        root->leftChild->parent = root;
    }
    else if(id > root->id)
    {
        root->rightChild =
            insert(root->rightChild, id, name, jalur);

        root->rightChild->parent = root;
    }

    return root;
}

/*
 * insertLeft: Ikat child sebagai leftChild parent.
 * TIDAK DIPAKAI di alur utama.
 */
void insertLeft(TreeNode* parent, TreeNode* child)
{
    if(parent != NULL)
    {
        parent->leftChild = child;

        if(child != NULL)
            child->parent = parent;
    }
}

/*
 * insertRight: Ikat child sebagai rightChild parent.
 * TIDAK DIPAKAI di alur utama (rightChild di-set langsung oleh insert_bentang).
 */
void insertRight(TreeNode* parent, TreeNode* child)
{
    if(parent != NULL)
    {
        parent->rightChild = child;

        if(child != NULL)
            child->parent = parent;
    }
}

/*
 * destroyTree: Hapus seluruh node tree dari heap (postorder).
 * Param root: root tree.
 * Dipanggil oleh: build_tree() di connection.c.
 */
void destroyTree(TreeNode* root)
{
    if(root == NULL)
        return;

    destroyTree(root->leftChild);
    destroyTree(root->rightChild);

    free(root);
}

/*
 * preorder: Traversal preorder (root → kiri → kanan).
 * Untuk debugging/visualisasi.
 */
void preorder(TreeNode* root)
{
    if(root == NULL)
        return;

    printf("%d - %s\n", root->id, root->name);

    preorder(root->leftChild);
    preorder(root->rightChild);
}

/*
 * inorder: Traversal inorder (kiri → root → kanan).
 * Untuk debugging/visualisasi.
 */
void inorder(TreeNode* root)
{
    if(root == NULL)
        return;

    inorder(root->leftChild);

    printf("%d - %s\n", root->id, root->name);

    inorder(root->rightChild);
}

/*
 * postorder: Traversal postorder (kiri → kanan → root).
 * Untuk debugging/visualisasi.
 */
void postorder(TreeNode* root)
{
    if(root == NULL)
        return;

    postorder(root->leftChild);
    postorder(root->rightChild);

    printf("%d - %s\n", root->id, root->name);
}

/*
 * printTree: Cetak tree secara visual dengan indentasi.
 * TIDAK DIPAKAI di alur utama.
 */
void printTree(TreeNode* root, int level)
{
    if(root == NULL)
        return;

    for(int i = 0; i < level; i++)
        printf("    ");

    printf("%s\n", root->name);

    printTree(root->leftChild, level + 1);
    printTree(root->rightChild, level + 1);
}

/*
 * ===== SERIALISASI TREE =====
 */

/*
 * tulis_node: Menulis node ke file secara rekursif (preorder).
 * Format: "id|name|jalur" dengan indentasi per level.
 * Node NULL ditulis sebagai "#".
 * Param fp: file pointer, node: node yang akan ditulis, depth: level kedalaman.
 * Dipanggil oleh: simpan_tree().
 */
static void tulis_node(FILE *fp, TreeNode *node, int depth) {
    if (node == NULL) {
        for (int i = 0; i < depth; i++) fprintf(fp, "    ");
        fprintf(fp, "#\n");
        return;
    }
    for (int i = 0; i < depth; i++) fprintf(fp, "    ");
    fprintf(fp, "%d|%s|%d\n", node->id, node->name, node->jalur);
    tulis_node(fp, node->leftChild, depth + 1);
    tulis_node(fp, node->rightChild, depth + 1);
}

/*
 * simpan_tree: Menyimpan tree ke file dalam format preorder.
 * Param filename: path file output (biasanya "data/tree.txt").
 * Format: "# BST Preorder Traversal" di baris pertama,
 *         lalu "id|nama|jalur" per node dengan indentasi.
 * Memanggil: tulis_node().
 * Dipanggil oleh: build_tree() di connection.c.
 */
void simpan_tree(const char *filename, TreeNode *root) {
    FILE *fp = fopen(filename, "w");
    if (!fp) {
        printf("Gagal menyimpan tree ke '%s'\n", filename);
        return;
    }
    fprintf(fp, "# BST Preorder Traversal\n");
    tulis_node(fp, root, 0);
    fclose(fp);
    printf("Tree berhasil disimpan ke '%s'\n", filename);
}