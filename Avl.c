#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* 
Peguei uma base de avl e comecei a implementar baseado na ideia do dicionário. 
Ainda faltam algumas coisas como um menu de operações, e criar a base de dados.
Usei avl por que funcionou bem, vou tentar com red-black mas achei essa versão daqui boa.
*/

// Definindo a estrutura para um nó da árvore AVL
struct Node {
    char key[51];
    char traducao[51];
    struct Node* left;
    struct Node* right;
    int height;
};

// Função para obter a altura de um nó
int getHeight(struct Node* node) {
    if (node == NULL) return 0;
    return node->height;
}

// Função para obter o máximo entre dois números
int max(int a, int b) {
    if(a>b)return a;
    else return b;
}

// Função para criar um novo nó da árvore AVL
struct Node* createNode(const char* key, const char* traducao) {
    struct Node* node = (struct Node*)malloc(sizeof(struct Node));
    strncpy(node->key, key, 50);
    strncpy(node->traducao, traducao, 50);
    node->left = NULL;
    node->right = NULL;
    node->height = 1;
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
struct Node* insertNode(struct Node* node, const char* key, const char* traducao) {
    if (node == NULL)
        return createNode(key, traducao);

    int compareResult = strcmp(key, node->key);
    if (compareResult < 0)
        node->left = insertNode(node->left, key, traducao);
    else if (compareResult > 0)
        node->right = insertNode(node->right, key, traducao);
    else
        return node;

    node->height = max(getHeight(node->left), getHeight(node->right)) + 1;

    int balance = getBalance(node);

    // Caso de rotação à esquerda esquerda
    if (balance > 1 && strcmp(key, node->left->key) < 0)
        return rotateRight(node);

    // Caso de rotação à direita direita
    if (balance < -1 && strcmp(key, node->right->key) > 0)
        return rotateLeft(node);

    // Caso de rotação à esquerda direita
    if (balance > 1 && strcmp(key, node->left->key) > 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }

    // Caso de rotação à direita esquerda
    if (balance < -1 && strcmp(key, node->right->key) < 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}


// Função para buscar um nó na árvore AVL
struct Node* searchNode(struct Node* node, const char* key) {
    if (node == NULL || strcmp(node->key, key) == 0)
        return node;

    int compareResult = strcmp(key, node->key);
    if (compareResult < 0)
        return searchNode(node->left, key);

    return searchNode(node->right, key);
}

// Função para encontrar o nó com o valor mínimo (mais à esquerda) em uma árvore AVL
struct Node* findMinNode(struct Node* node) {
    struct Node* current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Função para deletar um nó na árvore AVL
struct Node* deleteNode(struct Node* root, const char* key) {
    if (root == NULL)
        return root;

    int compareResult = strcmp(key, root->key);
    if (compareResult < 0)
        root->left = deleteNode(root->left, key);
    else if (compareResult > 0)
        root->right = deleteNode(root->right, key);
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
            strncpy(root->key, temp->key, 50);
            strncpy(root->traducao, temp->traducao, 50);
            root->right = deleteNode(root->right, temp->key);
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

void printpre(struct Node *root){
    if(root != NULL){
        printf("%s\n",root->key);
        printpre(root->left);
        printpre(root->right);
    }
}

void printin(struct Node *root){
    if(root != NULL){
        printpre(root->left);
        printf("%s\n",root->key);
        printpre(root->right);
    }
}

void printpos(struct Node *root){
    if(root != NULL){
        printpre(root->left);
        printpre(root->right);
        printf("%s\n",root->key);
    }
}

void menu(){  
    int opcao;  
    char str[51];
    struct Node* root = NULL;
    while(1){
        printf("\nEscolha a opcao:\n1-encerrar o programa\n2-busca\n3-deletar no\n4-printar a arvore\n");
        scanf("%d",&opcao);
        root = insertNode(root, "apple", "maca");
            root = insertNode(root, "banana", "banana");
            root = insertNode(root, "cherry", "cereja");
            root = insertNode(root, "date", "tamara");
            root = insertNode(root, "grape", "uva");
            root = insertNode(root, "fig", "figo");
            root = insertNode(root, "kiwi", "kiwi");
            root = insertNode(root, "horse", "cavalo");
            root = insertNode(root, "duck", "pato");
            root = insertNode(root, "dog", "cachorro");
            root = insertNode(root, "cat", "gato");
        if(opcao == 0){
            printf("Programa encerrado");
            return;
        }
        switch (opcao)
        {
        case 1:
            break;
        case 2:
            str[0] = '\0';
            printf("\nChave a ser buscada\n");
            scanf("%s",str);
            struct Node* busca = searchNode(root,str);
            if (searchNode != NULL)
            printf("\nsignificado de: %s = %s", busca->key, busca->traducao);
            else printf("\nNo com chave %s nao encontrado na arvore.", str);
            break;
        case 3:
            str[0] = '\0';
            printf("\nChave a ser deletada:\n");
            scanf("%s",str);
            root = deleteNode(root, str);
            break;
        case 4:
            printf("\nQual formatacao de saida e desejada?\n");
            printf("1-preorder\n2-inorder\n3-posorder\n");
            int m;
            scanf("%d",&m);
            if(m == 1)printpre(root);
            else if(m == 2)printin(root);
            else if(m==3)printpos(root);
            break;
        }
    }
}
// Função principal
int main() {
    menu();
    return 0;
}
