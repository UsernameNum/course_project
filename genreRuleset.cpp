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

std::vector<int> genreRuleset::getType(const std::string &type) const {
    auto it = chordLibrary.find(type);
    if (it == chordLibrary.end()) return {};
    return it->second;
}

std::string genreRuleset::getNextChord(const std::string &current) const {
        // если в рулсете не нашлось следующего аккорда, приходим в тонику
    auto it = transitions.find(current);
    if (it == transitions.end() || it->second.empty())
        { return "I"; }
        // {0.3, 0.3, 0.2, 0.2}
    const auto& options = it->second;
    std::vector<double> weights;
    weights.reserve(options.size());
    for (const auto& t : options) {
        weights.push_back(t.probability);
    }
        // генератор случайных чисел
    std::random_device rd;
    std::mt19937 gen(rd());
        // взвешенный подбор
    std::discrete_distribution<int> dist(weights.begin(), weights.end());
    int selectedIndex = dist(gen);

    return options[selectedIndex].nextChord;
}

std::string genreRuleset::getClosingChord(const std::string& from, const std::string& to) const {
    auto itFrom = transitions.find(from);
    if (itFrom == transitions.end() || itFrom->second.empty()) {
        return "I";
    }

    std::string bestCandidate = itFrom->second.front().nextChord;
    double bestScore = -1.0;

    for (const auto& cand : itFrom->second) {
        const std::string& candidate = cand.nextChord;
        double pPrevToCand = cand.probability;

        // p(candidate -> to)
        double pCandTo = 0.0;
        auto itCand = transitions.find(candidate);
        if (itCand != transitions.end()) {
            for (const auto& t : itCand->second) {
                if (t.nextChord == to) {
                    pCandTo = t.probability;
                    break;
                }
            }
        }
        double score = pPrevToCand * pCandTo;
        if (score > bestScore) {
            bestScore = score;
            bestCandidate = candidate;
        }
    }
    return bestCandidate;
}