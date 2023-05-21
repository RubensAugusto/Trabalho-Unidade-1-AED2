#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "avl.hpp"

/* 
Peguei uma base de avl e comecei a implementar baseado na ideia do dicionário. 
Ainda faltam algumas coisas como um menu de operações, e criar a base de dados.
Usei avl por que funcionou bem, vou tentar com red-black mas achei essa versão daqui boa.
*/

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

/*
A rotação a direita é feita quando há um desbalanceamento no nó da esquerda, ou seja uma diferença de altura.
struct Node * desbalanceado é o nó desbalanceado
struct Node *new se tornará o novo nó no lugar de 'desbalanceado'
struct Node *Temp é um auxiliar para a rotação, armazenando os dadso a direieta de 'new'.

O nó new recebe 'desbalanceado'como seu filho a direita, em seguida o filho a esquerda de 
'desbalanceado' recebe o nó auxiliar(agora o filho a esquerda do nó inicialmente desbalanceado).

Em resumo, new se torna a subarvore a esquerda do nó desbalanceado, e vai para sua posição, enquanto temp
armazena a sub arvore da direita de new. New recebe o nó desbalanceado como sua subarvore da direita.
A antiga subarvore da direita é inserida como subarvore da direta do nó desbalanceado, a partir do auxiliar 'Temp'.

Feita a rotação, as alturas são atualizadas, e o nó new retorna na posição que o desbalanceado ocupava.
A subarvore New é retornada na posição que Desbalanceado ocupava.
*/
struct Node* rotateRight(struct Node* desbalanceado) {
    struct Node* new = desbalanceado->left;
    struct Node* Temp = new->right;

    new->right = desbalanceado;
    desbalanceado->left = Temp;

    desbalanceado->height = max(getHeight(desbalanceado->left), getHeight(desbalanceado->right)) + 1;
    new->height = max(getHeight(new->left), getHeight(new->right)) + 1;

    return new;
}

/*
Funciona de forma similar a rotação a direita, porém, com os lados invertidos.

A rotação a esquerda é feita quando há um desbalanceamento no nó da direita, ou seja uma diferença de altura.
struct Node * desbalanceado é o nó desbalanceado
struct Node *new se tornará o novo nó no lugar de 'desbalanceado'
struct Node *Temp é um auxiliar para a rotação, armazenando os dadso a esquerda de 'new'.

O nó new recebe 'desbalanceado'como seu filho a esquerda, em seguida o filho a direita de 
'desbalanceado' recebe o nó auxiliar(agora o filho a direita do nó inicialmente desbalanceado).

Em resumo, new se torna a subarvore a direita do nó desbalanceado, e vai para sua posição, enquanto temp
armazena a sub arvore da esquerda de new. New recebe o nó desbalanceado como sua subarvore da esquerda.
A antiga subarvore da esquerda é inserida como subarvore da direta do nó desbalanceado, a partir do auxiliar 'Temp'.

Feita a rotação, as alturas são atualizadas, e o nó new retorna na posição que o desbalanceado ocupava.
A subarvore New é retornada na posição que Desbalanceado ocupava.
*/
struct Node* rotateLeft(struct Node* desbalanceado) {
    struct Node* new = desbalanceado->right;
    struct Node* T2 = new->left;

    new->left = desbalanceado;
    desbalanceado->right = T2;

    desbalanceado->height = max(getHeight(desbalanceado->left), getHeight(desbalanceado->right)) + 1;
    new->height = max(getHeight(new->left), getHeight(new->right)) + 1;

    return new;
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

void print_inorder(struct Node *root) {
    if (root != NULL) {
        print_inorder(root->left);
        cout << root->data->word << endl;
        print_inorder(root->right);
    }
}
