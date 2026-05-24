#include "harmonyEngine.h"

#include <random>

// key = "Abm"
std::vector<chord> generate(const std::string &key, const genreRuleset& rules, int length) {
    std::vector<chord> harmony;
    std::vector<std::string> allNotes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};
    std::vector<std::string> allModes = {"maj", "min"};
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> noteDist(0, allNotes.size() - 1);
    std::uniform_int_distribution<int> modeDist(0, allModes.size() - 1);

    std::string rootNoteName = "";
    std::string keyMode = "";

    for (const auto& note : allNotes) {
        if (key.find(note) == 0) {
            if (note.length() > rootNoteName.length()) {
                rootNoteName = note;
            }
        }
    }

    std::string modePart = key;
    if (!rootNoteName.empty()) {
        modePart = key.substr(rootNoteName.length());
    } else {
        rootNoteName = allNotes[noteDist(gen)];
    }

    if (modePart == "m" || modePart == "min" || modePart == "minor") {
        keyMode = "min";
    } else if (modePart == "maj" || modePart == "major" || (modePart.empty() && !key.empty())) {
        keyMode = "maj";
    } else {
        if (key.empty()) {
            keyMode = allModes[modeDist(gen)];
        } else {
            keyMode = "maj";
        }
    }

    std::string currentChord = "I";
    for (int i = 0; i < length; i++) {
    }

}

int voiceLeading(const std::vector<chord>& progression) {

}

void saveToFiles(const std::vector<chord>& progression, int bpm) {

}