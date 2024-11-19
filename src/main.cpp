#include "CMU_Dict.h"
#include "silent_letters.h"
#include <iostream>
#include <sstream>



// Main for Silent Letter Duplication
int main() {
    CMU_Dict dict;
    dict.import_dictionary();
    while(true) {
        std::string line;
        std::cout << "Enter text: ";
        std::getline(std::cin, line);

        std::istringstream iss{line};
        std::string word{};
        
        while (iss >> word){
            print_word_with_underscores(word, dict);
            std::cout << " ";
        }
        std::cout << std::endl;
    }
    

    return 0;
}
