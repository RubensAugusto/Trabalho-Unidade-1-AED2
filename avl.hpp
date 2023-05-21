#include <iostream>

using namespace std;

struct Word {
    string world;
    string meaning;
    string example;
};

// Definindo a estrutura para um nó da árvore AVL
struct Node {
    Word *data;
    int height;
    struct Node* left;
    struct Node* right;
};

// Função para obter a altura de um nó
int getHeight(struct Node* node);

// Função para obter o máximo entre dois números
int max(int a, int b);

// Função para criar um novo nó da árvore AVL
struct Node* createNode(Word *w);

// Função para realizar uma rotação simples à direita
struct Node* rotateRight(struct Node* z);

// Função para realizar uma rotação simples à esquerda
struct Node* rotateLeft(struct Node* z);

// Função para obter o fator de balanceamento de um nó
int getBalance(struct Node* node);

// Função para inserir um novo nó na árvore AVL
struct Node* insertNode(struct Node *node, Word *w);

// Função para buscar um nó na árvore AVL
struct Node* searchNode(struct Node* node, string w);

// Função para encontrar o nó com o valor mínimo (mais à esquerda) em uma árvore AVL
struct Node* findMinNode(struct Node* node);

// Função para deletar um nó na árvore AVL
struct Node* deleteNode(struct Node* root, Word *w);

void print_inorder(struct Node *root);