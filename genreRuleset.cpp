#include "genreRuleset.h"

#include <iostream>
#include <fstream>
#include <string>

std::map <std::string, std::vector<int>> chordLibrary {

};

bool genreRuleset::loadFromFile(const std::string &fileName) {
    std::string fullFileName = "rules/" + fileName;
    std::string line;
    std::ifstream in (fullFileName);
    if (!in.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return false;
    }

    while (std::getline(in, line))
    {
        std::cout << line << std::endl;
    }
    in.close();
    return true;
}

std::vector<int> getType(std::string type) {

}
std::string getNextChord(std::string current) {

}