//
//  main.cpp
//  SplayTree
//
//  Created by Andi Palo on 28/12/2016.
//  Copyright © 2016 Andi Palo. All rights reserved.
//


#include <stdio.h>
#include <stdlib.h>
#include <iostream>

typedef struct node {
    int value;
    struct node *left;
    struct node *right;
} Node;
void PrettyPrint(Node *node);
void freeBST(Node* root);
class SplayTree
{

public:
    SplayTree()
    {
    }
    
    // RR(Y rotates to the right)
    Node* RR_Rotate(Node* k2)
    {
        Node* k1 = k2->left;
        k2->left = k1->right;
        k1->right = k2;
        return k1;
    }
    
    // LL(Y rotates to the left)
    Node* LL_Rotate(Node* k2)
    {
        Node* k1 = k2->right;
        k2->right = k1->left;
        k1->left = k2;
        return k1;
    }
    
    // An implementation of top-down splay tree
    Node* Splay(int key, Node* root, bool isInsert = false)
    {
        if (!root)
            return NULL;

        Node header;
        /* header.rchild points to L tree;
         header.lchild points to R Tree */
        header.left = header.right = NULL;
        Node* LeftTreeMax = &header;
        Node* RightTreeMin = &header;
        while (1)
        {
            if (key < root->value)
            {
                if (!root->left)
                    break;
                if (key < root->left->value)
                {
                    root = RR_Rotate(root);
                    // only zig-zig mode need to rotate once,
                    if (!root->left)
                        break;
                }
                /* Link to R Tree */
                RightTreeMin->left = root;
                RightTreeMin = RightTreeMin->left;
                root = root->left;
                RightTreeMin->left = NULL;
            }
            else if (key > root->value)
            {
                if (!root->right)
                    break;
                if (key > root->right->value)
                {
                    root = LL_Rotate(root);
                    // only zag-zag mode need to rotate once,
                    if (!root->right)
                        break;
                }
                /* Link to L Tree */
                LeftTreeMax->right = root;
                LeftTreeMax = LeftTreeMax->right;
                root = root->right;
                LeftTreeMax->right = NULL;
            }
            else
                break;
        }
        /* assemble L Tree, Middle Tree and R tree */
        LeftTreeMax->right = root->left;
        RightTreeMin->left = root->right;
        root->left = header.right;
        root->right = header.left;
        
        if (root->value != key && !isInsert){
            std::cout<<"Value not found"<<std::endl;
        }
        
        return root;
    }
    
    Node* New_Node(int key)
    {
        Node* p_node = new Node;
        if (!p_node)
        {
            fprintf(stderr, "Out of memory!\n");
            exit(1);
        }
        p_node->value = key;
        p_node->left = p_node->right = NULL;
        return p_node;
    }
    
    Node* Insert(int key, Node* root)
    {
        static Node* p_node = NULL;
        if (!p_node)
            p_node = New_Node(key);
        else
            p_node->value = key;
        if (!root)
        {
            root = p_node;
            p_node = NULL;
            return root;
        }
        root = Splay(key, root, true);
        /* This is BST that, all keys <= root->key is in root->lchild, all keys >
         root->key is in root->rchild. */
        if (key < root->value)
        {
            p_node->left = root->left;
            p_node->right = root;
            root->left = NULL;
            root = p_node;
        }
        else if (key > root->value)
        {
            p_node->right = root->right;
            p_node->left = root;
            root->right = NULL;
            root = p_node;
        }
        else
            return root;
        p_node = NULL;
        return root;
    }
    
    Node* Delete(int key, Node* root)
    {
        Node* temp;
        if (!root)
            return NULL;
        root = Splay(key, root);
        if (key != root->value)
            return root;
        else
        {
            if (!root->left)
            {
                temp = root;
                root = root->right;
            }
            else
            {
                temp = root;
                /*Note: Since key == root->key,
                 so after Splay(key, root->lchild),
                 the tree we get will have no right child tree.*/
                root = Splay(key, root->left, true);
                root->right = temp->right;
            }
            free(temp);
            return root;
        }
    }
    
    Node* Search(int key, Node* root)
    {
        return Splay(key, root);
    }
    
    void InOrder(Node* root)
    {
        if (root)
        {
            InOrder(root->left);
            std::cout<< "key: " <<root->value;
            if(root->left)
                std::cout<< " | left child: "<< root->left->value;
            if(root->right)
                std::cout << " | right child: " << root->right->value;
            std::cout<< "\n";
            InOrder(root->right);
        }
    }
};

int _print_t(Node *tree, int is_left, int offset, int depth, char s[20][255]);

int main(int argc, const char * argv[]) {
    SplayTree st;
    int vector[10] = {9,8,7,6,5,4,3,2,1,0};
    Node *root;
    root = NULL;
    const int length = 10;
    int i;
    for(i = 0; i < length; i++)
        root = st.Insert(vector[i], root);
    PrettyPrint(root);
    root = st.Splay(5, root);
    PrettyPrint(root);
    root = st.Splay(11, root);
    PrettyPrint(root);
    root = st.Search(6, root);
    PrettyPrint(root);
    PrettyPrint(root = st.Delete(7, root));
    freeBST(root);
}


void PrettyPrint(Node *node){
    char s[20][255];
    for (int i = 0; i < 20; i++)
        sprintf(s[i], "%80s", " ");
    
    _print_t(node, 0, 0, 0, s);
    
    for (int i = 0; i < 20; i++)
        printf("%s\n", s[i]);
}

void freeBST(Node* root){
    if (root == NULL) {
        return;
    }
    freeBST(root->left);
    freeBST(root->right);
    free(root);
}


//*******************
int _print_t(Node *tree, int is_left, int offset, int depth, char s[20][255])
{
    char b[20];
    int width = 5;
    
    if (!tree) return 0;
    
    sprintf(b, "(%3d)", tree->value);
    
    int left  = _print_t(tree->left,  1, offset,                depth + 1, s);
    int right = _print_t(tree->right, 0, offset + left + width, depth + 1, s);
    
#ifdef COMPACT
    for (int i = 0; i < width; i++)
        s[depth][offset + left + i] = b[i];
    
    if (depth && is_left) {
        
        for (int i = 0; i < width + right; i++)
            s[depth - 1][offset + left + width/2 + i] = '-';
        
        s[depth - 1][offset + left + width/2] = '.';
        
    } else if (depth && !is_left) {
        
        for (int i = 0; i < left + width; i++)
            s[depth - 1][offset - width/2 + i] = '-';
        
        s[depth - 1][offset + left + width/2] = '.';
    }
#else
    for (int i = 0; i < width; i++)
        s[2 * depth][offset + left + i] = b[i];
    
    if (depth && is_left) {
        
        for (int i = 0; i < width + right; i++)
            s[2 * depth - 1][offset + left + width/2 + i] = '-';
        
        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset + left + width + right + width/2] = '+';
        
    } else if (depth && !is_left) {
        
        for (int i = 0; i < left + width; i++)
            s[2 * depth - 1][offset - width/2 + i] = '-';
        
        s[2 * depth - 1][offset + left + width/2] = '+';
        s[2 * depth - 1][offset - width/2 - 1] = '+';
    }
#endif
    
    return left + width + right;
}



