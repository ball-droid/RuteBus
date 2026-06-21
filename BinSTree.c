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