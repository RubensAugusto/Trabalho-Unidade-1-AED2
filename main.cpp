
#include "avl.hpp"

void menu();
void buscar_palavra();
void inserir_palavras();
void deletar_palavra();

struct Node *root = nullptr;

int main() {
    
    string csv = "words-meaning-final.csv";
    root = read_csv(csv);

    // menu();
}

void menu() {
    char opcao;
    do {
        printf("\n============ Dicionário ============\n");
        printf("Digite um comando para prosseguir:\n");
        printf("1 - Buscar Palavra\n");
        printf("2 – Inserir Palavra\n");
        printf("3 – Deletar Palavra\n");
        printf("4 - Mostrar Palavras\n");
        printf("S – Sair\n");
        printf("\nEscolha: ");

        scanf(" %c", &opcao);
        opcao = toupper(opcao);
        switch (opcao)
        {
        case '1':
            buscar_palavra();
            break;
        case '2':
            inserir_palavras();
            break;
        case '3':
            deletar_palavra();
            break;
        case '4':
            // menu_atualizar_cliente();
            break;
        case 'S':
            break;
        default:
            printf("\n** Comando inválido **\n");
        }
    } while (opcao != 'S');
}

void buscar_palavra() {
    
    string palavra;
    cout << "Digite uma palavra (inglês): ";
    cin >> palavra;

    struct Node *n = searchNode(root, palavra);

    if (n != nullptr) {
        print_word(n);
    } else {
        cout << "Palavra não encontrada!" << endl;
    }
}

void inserir_palavras() {

    string w, m, e;

    cout << "Digite a palavra:";
    cin >> w;

    cout << "Digite o significado:";
    cin >> m;

    cout << "Digite um examplo de uso desse palavra:";
    cin >> e;


    Word *nova_palavra = new Word{w, m, e};
    root = insertNode(root, nova_palavra);

    struct Node *check = searchNode(root, w);

    if (check != nullptr) {
        cout << "Palavra inserida com sucesso!" << endl;
    } else {
        cout << "Erro ao inserir a palavra!" << endl;
    }

}

void deletar_palavra() {
    string palavra;
    cout << "Digite uma palavra (inglês): ";
    cin >> palavra;

    struct Node *n = searchNode(root, palavra);

    if (n != nullptr) {
        print_word(n);
    } else {
        cout << "Palavra não encontrada!" << endl;
        return;
    }

    root = deleteNode(root, n->data);

    struct Node *check = searchNode(root, palavra);

    if (check == nullptr) {
        cout << "Palavra deletada com sucesso!" << endl;
    } else {
        cout << "Palavra não deletada" << endl;
    }

}