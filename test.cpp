#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "avl.hpp"

struct Node * read_csv(const string& nome_arquivo) {

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

int main() {

    struct Node* root = nullptr;
    string filename = "words-meaning-final.csv";

    auto start = chrono::system_clock::now();

    root = read_csv(filename);
    
    auto end = chrono::system_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << std::endl;

    // print_inorder(root);        

    return 0;
}