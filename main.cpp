
#include "avl.hpp"
#include "util.hpp"

struct Node *root;

int main() {
    
    string csv = "words-meaning-final.csv";
    root = read_csv(csv);

    menu();
}

