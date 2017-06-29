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
    /*
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
    */
    
    
    
    int option; // user's entered option will be saved in this variable
    Node *root = NULL;
    SplayTree st;
    
    do {
        //Displaying Options for the menu
        std::cout << "1) Create Splay Tree with 10 elements" << std::endl;
        std::cout << "2) Add element to Splay Tree " << std::endl;
        std::cout << "3) Remove element from Splay Tree" << std::endl;
        std::cout << "4) Find element" << std::endl;
        std::cout << "5) Print BST" << std::endl;
        std::cout << "6) Free BST" << std::endl;
        std::cout << "7) Splay Tree" << std::endl;
        std::cout << "8) Exit Program " << std::endl;
        //Prompting user to enter an option according to menu
        std::cout << "Please select an option : ";
        std::cin >> option;  // taking option value as input and saving in variable "option"
        
        if (option == 1) {
            if (root != NULL) {
                std::cout << "Splay Tree is already created, select other options" << std::endl;
                continue;
            }
            std::cout << "Creating Splay Tree" << std::endl;
            const int length = 10;
            int i;
            int vector[10] = {9,8,7,6,5,4,3,2,1,0};
            for(i = 0; i < length; i++){
                root = st.Insert(vector[i], root);
                std::cout<<"Adding element: "<<vector[i]<<std::endl;
            }
            
        }
        else if (option == 2) // Checking if user selected option 1
        {
            int element;
            if (root == NULL) {
                std::cout << "Please create Splay Tree first" << std::endl;
                continue;
            }
            std::cout << "Adding element to Splay Tree" << std::endl;
            std::cout << "Provide element value" << std::endl;
            std::cin >> element;
            root = st.Insert(element, root);
        }
        else if (option == 3) // Checking if user selected option 2
        {
            int element;
            if (root == NULL) {
                std::cout << "Please create Splay Tree first" << std::endl;
                continue;
            }
            std::cout << "Removing element to BST" << std::endl;
            std::cout << "Provide element value to remove" << std::endl;
            std::cin >> element;
            root = st.Delete(element, root);
        }
        else if (option == 4) // Checking if user selected option 3
        {
            int element;
            if (root == NULL) {
                std::cout << "Please create Splay Tree first" << std::endl;
                continue;
            }
            std::cout << "Find element in Splay Tree" << std::endl;
            std::cout << "Provide element value to find" << std::endl;
            std::cin >> element;
            root = st.Search(element, root);
        }
        else if (option == 5) // Checking if user selected option 4
        {
            PrettyPrint(root);
        }else if (option == 6)
        {
            std::cout << "Removing all BST" << std::endl;
            freeBST(root);
            root = NULL;
        }
        else if (option == 7)
        {
            int element;
            if (root == NULL) {
                std::cout << "Please create Splay Tree first" << std::endl;
                continue;
            }
            std::cout << "Splay Tree" << std::endl;
            std::cout << "Provide element value to Splay" << std::endl;
            std::cin >> element;
            root = st.Splay(element, root);
            
        }
        else //if user has entered invalid choice (other than 1,2,3 or 4)
        {
            std::cout << "Quitting program..." << std::endl;
            break;
        }
    } while (option <= 7);
    return 0;

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



