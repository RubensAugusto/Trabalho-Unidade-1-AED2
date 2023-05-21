#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define BLACK 0
#define RED 1

struct NO {
    int color;              //Cor usada para o balanceamento da arvore
    struct NO* left;
    struct NO* right;
    char key[51];
    char meaning[501];
};

typedef struct NO* ArvRB;

int color(struct NO* Z) {   //funcao que retorna cor do NO
    if(Z == NULL) 
        return (BLACK);
    else
        return Z->color;
}

void change_Color(struct NO* Z) {   //Muda a cor do no(raiz) e verifica os filhos
    Z->color = !(Z->color);
    if(Z->left != NULL)
        Z->left->color = !(Z->left->color);
    if(Z->right != NULL)
        Z->right->color = !(Z->right->color);
}

struct NO* rotateLeft(struct NO* A) {
    struct NO *B = A->right;

    A->right = B->left;
    B->left = A;
    B->color = A->color;
    A->color = RED;

    return (B);
}

struct NO* rotateRight(struct NO* A) {
    struct NO *B = A->left;

    A->left = B->right;
    B->right = A;
    B->color = A->color;
    A->color = RED;

    return (B);
}

struct NO* Move_LeftRED(struct NO* B) {     //movimenta NO red para esquerda
    change_Color(B);
    if(color(B->right->left) == RED) {
        B->right = rotateRight(B->right);
        B = rotateLeft(B);
        change_Color(B);
    }
    return (B);
}

struct NO* Move_RightRED(struct NO* B) {    //movimenta NO red para direita
    change_Color(B);
    if(color(B->left->left) == RED) {
        B = rotateRight(B);
        change_Color(B);
    }
    return (B);
}

struct NO* balance_RB(struct NO* A) {       //Balanceamento da Arvore RB
    if(color(A->right) == RED)
        A = rotateLeft(A);

    if(A->left != NULL && color(A->left) == RED && color(A->left->left) == RED){
        A = rotateRight(A);
    }
    if(color(A->left) == RED && color(A->right) == RED)
        change_Color(A);

    return (A);
}

struct NO* insert_NO(struct NO* A, char *key, char *meaning, int *value) {  //Insere o NO na arvore
    if(A == NULL) {
        struct NO* new;
        new = (struct NO*) malloc (sizeof(struct NO));

        if(new == NULL) {
            *value = 0;
            return (NULL);
        }
        strcpy(new->key, key);
        strcpy(new->meaning, meaning);
        new->color = RED;
        new->right = NULL;
        new->left = NULL;

        *value = 1;
        return (new);
    }

    if(strcmp(key, A->key) == 0) {
        *value = 0;
    }else{
        if(strcmp(key, A->key) < 0) 
            A->left = insert_NO(A->left, key, meaning, value);

        else
            A->right = insert_NO(A->right, key, meaning, value);

    }

    if(color(A->right) == RED && color(A->left) == BLACK)
        A = rotateLeft(A);

    if(color(A->left) == RED && color(A->left->left) == RED)
        A = rotateRight(A);

    if(color(A->left) == RED && color(A->right) == RED)
        change_Color(A);
    
    return (A);
}

int insert_ArvRB(struct NO** root, char *key, char *meaning) {  //Chama a insert_NO 
    int value;

    *root = insert_NO(*root, key, meaning, &value);
    if((*root) != NULL)
        (*root)->color = BLACK;

    return value;   //indica o sucesso ou nao da operacao
}

struct NO* search_NO(struct NO* root, const char *key) {    //Faz a busca do NO
    if (root == NULL || strcmp(root->key, key) == 0)
        return (root);

    int compareResult = strcmp(key, root->key);

    if (compareResult < 0)
        return search_NO(root->left, key);

    else
        return search_NO(root->right, key);

}

int verifyRB(struct NO** root, char *key) { //Busca e retorna true or false
    if(root == NULL) return 0;
    struct NO* current = *root;

    while(current != NULL) {
        if(strcmp(key,  current->key) == 0) {
            return (1);
        }
        if(strcmp(key, current->key) > 0) {
            current = current->right;
        }else{
            current = current->left;
        }
    }
    return 0;

}

struct NO* removeLower(struct NO* A) {      //Remocao do (NO) mais a esquerda da raiz
    if(A->left == NULL) {
        free(A);
        return (NULL);
    }
    if(color(A->left) == BLACK && color(A->left->left) == BLACK)
        A = Move_LeftRED(A);

    A->left = removeLower(A->left);

    return (balance_RB(A));
}

struct NO* searchLower(struct NO* current) {    //Procura o (NO) mais a esquerda
    struct NO* x = current->left;
    struct NO* y = current;

    while(x != NULL) {
        y = x;
        x = x->left;
    }
    return (y);
}

struct NO* removeNO(struct NO* A, char *key) {  //Remove (NO)
    struct NO* Z = NULL;
    if(strcmp(key, A->key) < 0) {
        if(color(A->left) == BLACK && color(A->left->left) == BLACK) {
            A = Move_LeftRED(A);
        }
        A->left = removeNO(A->left, key);

    }else{
        if(color(A->left) == RED)
            A = rotateRight(A);
        
        if(strcmp(key, A->key) == 0 && A->right == NULL) {
            free(A);
            return (NULL);
        }
        if(color(A->right) == BLACK && color(A->right->left) == BLACK)
            A = Move_RightRED(A);

        if(strcmp(key, A->key) == 0) {
            Z = searchLower(A->right);
            strcpy(A->key, Z->key);
            A->right = removeLower(A->right);
        }else{
            A->right = removeNO(A->right, key);
        }
    }
    return balance_RB(A);   

}

int remove_ArvRB(struct NO **root, char *key) { //Verifica o (NO) para remocao
    struct NO* X;
    if(verifyRB(root, key)){
        X = *root;
        *root = removeNO(X, key);   // Remove No  

        if(*root != NULL) {
            (*root)->color = BLACK; // Retorna dado cor(NO) como (BLACK) == 0
        }
        return (1);
    }else{
        return 0;
    }
}

int main() {

    struct NO* Arv = NULL;
    FILE* file = fopen("words-meaning.csv", "r");
    char word[51];      //Palavra
    char meaning[501];  //Significado
    char key[51];       //chave (Entrada) == palavra

    if(file == NULL) exit(1);
    fscanf(file, "%*[^\n]%*c"); //Desconsidera a linha inicial do arquivo

    while(fscanf(file, "%[^,],%[^\n]%*c", word, meaning) != EOF){
        
        insert_ArvRB(&Arv, word, meaning);  //Insere dados do arquivo
        memset(word, 0, 51);               //Limpar strings (garantia)
        memset(meaning, 0, 501);
    }

    printf("Buscar palavra: (Por favor colocar a primeira letra maiuscula)\n> ");

    scanf("%s",key);    
    struct NO* foundNode = search_NO(Arv, key); //Busca key == word
    
    if(foundNode != NULL) {
        printf("\nPalavra: < %s > - Significado:\n %s\n\n", foundNode->key, foundNode->meaning);

    }else{
        printf("\nPalavra: (%s) nao encontrada na arvore :(\n\n", key);
    }

    fclose(file);   

    return 0;
}
