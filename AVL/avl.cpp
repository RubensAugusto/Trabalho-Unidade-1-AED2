#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <queue>

#include "avl.hpp"


// Função para obter a altura de um nó
int getHeight(struct Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Função para obter o máximo entre dois números
int max(int a, int b) {
    if (a > b) return a;
    else return b;
}

// Função para criar um novo nó da árvore AVL
struct Node* createNode(Word *w) {
    struct Node* node = (struct Node*) malloc(sizeof(struct Node));
    if (node != NULL) {
        node->data = w;
        node->left = NULL;
        node->right = NULL;
        node->height = 1;
    }
    return node;
}

// Função para realizar uma rotação simples à direita
struct Node* rotateRight(struct Node* z) {
    struct Node* y = z->left;
    struct Node* T3 = y->right;

    y->right = z;
    z->left = T3;

    z->height = max(getHeight(z->left), getHeight(z->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Função para realizar uma rotação simples à esquerda
struct Node* rotateLeft(struct Node* z) {
    struct Node* y = z->right;
    struct Node* T2 = y->left;

    y->left = z;
    z->right = T2;

    z->height = max(getHeight(z->left), getHeight(z->right)) + 1;
    y->height = max(getHeight(y->left), getHeight(y->right)) + 1;

    return y;
}

// Função para obter o fator de balanceamento de um nó
int getBalance(struct Node* node) {
    if (node == NULL)
        return 0;
    return getHeight(node->left) - getHeight(node->right);
}

// Função para inserir um novo nó na árvore AVL
struct Node* insertNode(struct Node *node, Word *w) {
    if (node == NULL)
        return createNode(w);

    int compareResult = strcmp(w->word.c_str(), node->data->word.c_str());
    if (compareResult < 0)
        node->left = insertNode(node->left, w);
    else if (compareResult > 0)
        node->right = insertNode(node->right, w);
    else
        return node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    // Caso de rotação à esquerda esquerda
    if (balance > 1 && compareResult < 0)
        return rotateRight(node);

    // Caso de rotação à direita direita
    if (balance < -1 && compareResult > 0)
        return rotateLeft(node);

    // Caso de rotação à esquerda direita
    if (balance > 1 && compareResult > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Caso de rotação à direita esquerda
    if (balance < -1 && compareResult < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

// Função para buscar um nó na árvore AVL
struct Node* searchNode(struct Node* node, string w) {
    if (node == NULL || strcmp(w.c_str(), node->data->word.c_str()) == 0)
        return node;

    int compareResult = strcmp(w.c_str(), node->data->word.c_str());
    if (compareResult < 0)
        return searchNode(node->left, w);

    return searchNode(node->right, w);
}

// Função para encontrar o nó com o valor mínimo (mais à esquerda) em uma árvore AVL
struct Node* findMinNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Função para deletar um nó na árvore AVL
struct Node* deleteNode(struct Node* root, Word *w) {
    if (root == NULL)
        return root;

    int compareResult = strcmp(w->word.c_str(), root->data->word.c_str());

    if (compareResult < 0)
        root->left = deleteNode(root->left, w);
    else if (compareResult > 0)
        root->right = deleteNode(root->right, w);
    else {
        if (root->left == NULL || root->right == NULL) {
            struct Node* temp = root->left ? root->left : root->right;

            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;

            free(temp);
            
        } else {
            struct Node* temp = findMinNode(root->right);
            root->data->word = temp->data->word;
            root->right = deleteNode(root->right, temp->data);
        }
    }

    if (root == NULL)
        return root;

    root->height = max(getHeight(root->left), getHeight(root->right)) + 1;

    int balance = getBalance(root);

    // Caso de rotação à esquerda esquerda
    if (balance > 1 && getBalance(root->left) >= 0)
        return rotateRight(root);

    // Caso de rotação à esquerda direita
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = rotateLeft(root->left);
        return rotateRight(root);
    }

    // Caso de rotação à direita direita
    if (balance < -1 && getBalance(root->right) <= 0)
        return rotateLeft(root);

    // Caso de rotação à direita esquerda
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rotateRight(root->right);
        return rotateLeft(root);
    }

    return root;
}

struct Node* read_csv(const string& nome_arquivo) {

    struct Node * root = nullptr;

    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return nullptr;
    }

    string linha;
    getline(arquivo, linha); // Ignorar a primeira linha do csv

    string word, meaning, example;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);
        
        getline(ss, word, ',');
        getline(ss, meaning, ',');
        getline(ss, example);

        Word w{word, meaning, example};

        root = insertNode(root, new Word(w));
    }

    arquivo.close();

    return root;
}

void print_word(struct Node *n) {
    Word *w = n->data;

    cout << "Word: " << w->word << endl;
    cout << "Meaning: " << w->meaning << endl;
    cout << "Example: " << w->example << endl;
    cout << endl;

}

void print_preorder(struct Node *root) {
    if (root != NULL) {
        cout << root->data->word << endl;
        print_preorder(root->left);
        print_preorder(root->right);
    }
}

void print_inorder(struct Node *root) {
    if (root != NULL) {
        print_inorder(root->left);
        cout << root->data->word << endl;
        print_inorder(root->right);
    }
}

void print_postorder(struct Node *root) {
    if (root != NULL) {
        print_postorder(root->left);
        print_postorder(root->right);
        cout << root->data->word << endl;
    }
}

void print_bfs(struct Node *root) {
    queue<struct Node*> q;

    auto temp = root;

    do {

        cout << temp->data->word << endl;
        if (temp->left != nullptr) {
            q.push(temp->left);
        }

        if (temp->right != nullptr) {
            q.push(temp->right);
        }

        if (q.empty()) {
            break;
        }

        temp = q.front();
        q.pop();

    } while(1);
}
