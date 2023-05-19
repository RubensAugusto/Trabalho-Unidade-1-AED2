#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>

struct Data {
    std::string word;
    std::string meaning;
    std::string example;
};

std::vector<Data> readCSV(const std::string& filename) {
    std::vector<Data> data;

    std::ifstream file(filename);
    if (!file.is_open()) {
        std::cout << "Erro ao abrir o arquivo." << std::endl;
        return data;
    }

    std::string line;
    std::getline(file, line); // Ignorar o cabeçalho

    while (std::getline(file, line)) {
        std::stringstream ss(line);
        std::string word, meaning, example;

        std::getline(ss, word, ',');
        std::getline(ss, meaning, ',');
        std::getline(ss, example, ',');

        data.push_back({word, meaning, example});
    }

    file.close();

    return data;
}

int main() {
    std::string filename = "teste5.csv";

    std::vector<Data> data = readCSV(filename);

    // Exibir os dados lidos do CSV
    for (const auto& entry : data) {
        std::cout << "Word: " << entry.word << std::endl;
        std::cout << "Meaning: " << entry.meaning << std::endl;
        std::cout << "Example: " << entry.example << std::endl;
        std::cout << std::endl;
    }

    return 0;
}
