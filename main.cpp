#include <iostream>
#include <fstream>
#include "yandex_music_app.h"


int main() {
    std::ifstream inputFile("../token.txt"); // Open the file named "example.txt"
      std::string authToken;
    
    if (inputFile.is_open()) { // Check if the file was opened successfully
        std::getline(inputFile, authToken);
    }
    try {
        YandexMusicApp app(authToken);
        app.run();
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
    
    return 0;
}
