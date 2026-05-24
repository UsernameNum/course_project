#include "genreRuleset.h"

#include <iostream>
#include <fstream>
#include <random>
#include <sstream>
#include <string>

std::string cut(const std::string& str) {
    size_t first = str.find_first_not_of(" \t\r\n");
    if (first == std::string::npos) return "";
    size_t last = str.find_last_not_of(" \t\r\n");
    return str.substr(first, (last - first + 1));
}

        // "I    = 0:5"
void genreRuleset::parseChordLine(const std::string& line) {
    size_t eqPos = line.find('=');
        // "I"
    std::string leftPart = cut(line.substr(0, eqPos));
        // "0:5"
    std::string rightPart = cut(line.substr(eqPos + 1));
    size_t colonPos = rightPart.find(':');
        // "0" -> 0, int
    int rootOffset = std::stoi(cut(rightPart.substr(0, colonPos)));
        // "5"
    std::string chordType = cut(rightPart.substr(colonPos + 1));
        // <"I", <0, "5">>
    degreeDefinitions[leftPart] = {rootOffset, chordType};
}

        // "I    -> bIII:0.30, IV:0.30, bVII:0.20, I:0.20"
void genreRuleset::parseTransitionLine(const std::string& line) {
    size_t arrPos = line.find("->");
        // "I"
    std::string currentChord = cut(line.substr(0, arrPos));
        // "bIII:0.30, IV:0.30, bVII:0.20, I:0.20"
    std::string allTargets = cut(line.substr(arrPos + 2));
    std::stringstream ss(allTargets);
    std::string elem;
    while (std::getline(ss, elem, ',')) {
        size_t eqPos = elem.find(':');
        // "bIII"
        std::string scale = cut(elem.substr(0, eqPos));
        // "0.30" -> 0.3, double
        double chance = std::stod(cut(elem.substr(eqPos + 1)));
        Transition transition = {scale, chance};
        // <"I", <"bIII", 0.3>>
        transitions[currentChord].push_back(transition);
    }
}

bool genreRuleset::loadFromFile(const std::string &fileName) {
    std::string currentSelection;
    std::string fullFileName = "rules/" + fileName;
    std::string line;

    std::ifstream in (fullFileName);
    if (!in.is_open()) {
        std::cout << "unable to open file" << std::endl;
        return false;
    }

    while (std::getline(in, line))
    {
        if (line.empty()) {
            continue;
        }

        if (line == "[CHORDS]") {
            currentSelection = line;
        } else if (line == "[TRANSITIONS]") {
            currentSelection = line;
        }

        if (currentSelection == "[CHORDS]") {
            parseChordLine(line);
        } else if (currentSelection == "[TRANSITIONS]") {
            parseTransitionLine(line);
        }
    }
    in.close();
    return true;
}

std::vector<int> genreRuleset::getType(const std::string &type) {
    if (chordLibrary.find(type) == chordLibrary.end()) { return chordLibrary[type]; }
    return {};
}

std::string genreRuleset::getNextChord(const std::string &current) {
        // если в рулсете не нашлось следующего аккорда, приходим в тонику
    if (transitions.find(current) == transitions.end() || transitions[current].empty())
        { return "I"; }
        // {0.3, 0.3, 0.2, 0.2}
    std::vector<double> weights;
    for (const auto& t : transitions[current]) {
        weights.push_back(t.probability);
    }
        // генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
        // взвешенный подбор
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    int selectedIndex = dist(gen);

    return transitions[current][selectedIndex].nextChord;
}