#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

#include "avl.hpp"

vector<Word> read_csv(const string& nome_arquivo) {
    vector<Word> palavras;

    ifstream arquivo(nome_arquivo);
    if (!arquivo.is_open()) {
        cout << "Erro ao abrir o arquivo." << endl;
        return palavras;
    }

    string linha;
    getline(arquivo, linha); // Ignorar a primeira linha do csv

    string word, meaning, example;
    while (getline(arquivo, linha)) {
        stringstream ss(linha);

        getline(ss, word, ',');
        getline(ss, meaning, ',');
        getline(ss, example);
    
        palavras.push_back({word, meaning, example});
    }

    arquivo.close();

    return palavras;
}

int main() {

    struct Node* root = nullptr;
    string filename = "words-meaning-final.csv";

    vector<Word> data = read_csv(filename);

    auto start = chrono::system_clock::now();

    for (int i = 0; i < data.size(); i++) {
        root = insertNode(root, &data[i]);
    }

    auto end = chrono::system_clock::now();

    cout << chrono::duration_cast<chrono::milliseconds>(end - start).count() << "ms" << std::endl;
    
    return 0;
}