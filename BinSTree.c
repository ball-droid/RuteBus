/*
 * =========================================================================
 * MODUL      : BinSTree.c - Implementasi Binary Search Tree (BST)
 * =========================================================================
 * FUNGSI     : Implementasi lengkap ADT Binary Search Tree untuk menyusun
 *              rute halte bus dalam bentuk pohon biner.
 *
 * CARA KERJA :
 *   - createNode : alokasi memori, isi id/name/jalur, child = NULL
 *   - insert     : sisip node secara rekursif (id < root = kiri, > = kanan)
 *   - insertLeft/insertRight : tempel child manual (bukan BST biasa)
 *   - destroyTree: hapus seluruh tree (postorder) + free memori
 *   - preorder/inorder/postorder : 3 jenis traversal pencetakan
 *   - simpan_tree / muat_tree : serialisasi tree ke/dari file teks
 *
 * MENERIMA   : Dipanggil oleh connection.c dengan parameter:
 *              - id (int), name (char[]), jalur (int) untuk create/insert
 *              - root (TreeNode*) untuk operasi tree
 *              - filename (const char*) untuk serialisasi
 *
 * MENGEMBALIKAN :
 *   - TreeNode* (pointer ke node yang dibuat/ditemukan, atau root baru)
 *   - void (untuk traversal, destroy, insertLeft/Right)
 *   - int (height, isLeaf)
 *
 * DIPAKAI OLEH : connection.c (build_tree, insert_bentang, dll)
 * MODUL INI INDEPENDEN : self-contained, hanya pakai stdlib/stdio/string
 * =========================================================================
 */

#include "BinSTree.h"

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

void insertLeft(TreeNode* parent, TreeNode* child)
{
    if(parent != NULL)
    {
        parent->leftChild = child;

        if(child != NULL)
            child->parent = parent;
    }
}

void insertRight(TreeNode* parent, TreeNode* child)
{
    if(parent != NULL)
    {
        parent->rightChild = child;

        if(child != NULL)
            child->parent = parent;
    }
}

void destroyTree(TreeNode* root)
{
    if(root == NULL)
        return;

    destroyTree(root->leftChild);
    destroyTree(root->rightChild);

    free(root);
}

void preorder(TreeNode* root)
{
    if(root == NULL)
        return;

    printf("%d - %s\n", root->id, root->name);

    preorder(root->leftChild);
    preorder(root->rightChild);
}

void inorder(TreeNode* root)
{
    if(root == NULL)
        return;

    inorder(root->leftChild);

    printf("%d - %s\n", root->id, root->name);

    inorder(root->rightChild);
}

void postorder(TreeNode* root)
{
    if(root == NULL)
        return;

    postorder(root->leftChild);
    postorder(root->rightChild);

    printf("%d - %s\n", root->id, root->name);
}

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

/* ===== SERIALISASI TREE ===== */

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

static TreeNode* baca_node(FILE *fp) {
    char buf[256];
    if (!fgets(buf, sizeof(buf), fp)) return NULL;

    size_t len = strlen(buf);
    if (len > 0 && buf[len - 1] == '\n') buf[len - 1] = '\0';
    if (strlen(buf) == 0) return NULL;

    /* Skip comment lines */
    if (buf[0] == '#') return baca_node(fp);

    /* NULL marker */
    if (buf[0] == '#' && strlen(buf) == 1) return NULL;

    /* Skip leading whitespace (indentation) */
    char *p = buf;
    while (*p == ' ' || *p == '\t') p++;

    /* Parse "id|name|jalur" */
    char *token_id = strtok(p, "|");
    char *token_name = strtok(NULL, "|");
    char *token_jalur = strtok(NULL, "|");

    if (!token_id || !token_name || !token_jalur) return NULL;

    int id = atoi(token_id);
    int jalur = atoi(token_jalur);

    TreeNode *node = createNode(id, token_name, jalur);
    node->leftChild = baca_node(fp);
    if (node->leftChild) node->leftChild->parent = node;
    node->rightChild = baca_node(fp);
    if (node->rightChild) node->rightChild->parent = node;

    return node;
}

TreeNode* muat_tree(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (!fp) {
        printf("Gagal membuka '%s'\n", filename);
        return NULL;
    }
    TreeNode *root = baca_node(fp);
    fclose(fp);
    if (root)
        printf("Tree berhasil dimuat dari '%s'\n", filename);
    else
        printf("Gagal memuat tree dari '%s'\n", filename);
    return root;
}