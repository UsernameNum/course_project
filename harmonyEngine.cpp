#include "harmonyEngine.h"

#include <algorithm>
#include <random>

// key = "Ab", genreRuleset = "jazz_major", int = 2-32
std::vector<chord> harmonyEngine::generate(const std::string &key, const genreRuleset& rules, int length) {
    std::string tonic; // тоника
    std::vector<chord> progression; // искомая прогрессия аккордов
    progression.reserve(length); // выделяем память для заполнения

    std::vector<std::string> allNotes = {"C", "C#", "D", "D#", "E", "F", "F#", "G", "G#", "A", "A#", "B"};

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> noteDist(0, allNotes.size() - 1);

    for (int i = 0; i < allNotes.size(); i++) { // ищем тонику среди библиотеки
        if (key == allNotes[i]) {
            tonic = key;
        }
    }
    if (tonic.empty()) { // если не нашли, подбираем случайную тонику
        tonic = allNotes[noteDist(gen)];
    }


    for (int i = 0; i < length; i++) {
        if (i == length-1) { // если последний аккорд последовательности, направляем в сторону тоники

        } else { // если любой другой, оставляем правила рулсетов

        }
    }

    return progression;
}

int voiceLeading(const std::vector<chord>& progression) {

}

void saveToFiles(const std::vector<chord>& progression, int bpm) {

}