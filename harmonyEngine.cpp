#include "harmonyEngine.h"

#include <algorithm>
#include <random>

/*  0  1  2  3  4  5  6  7  8  9  10 11
 *  c  c# d  d# e  f  f# g  g# a  a# b
 */

static const std::vector<std::string> allNotes = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

int harmonyEngine::noteIndex(const std::string& name) {
    for (int i = 0; i < (int)allNotes.size(); ++i) {
        if (allNotes[i] == name) return i;
    }
    return 0; // C
}

int harmonyEngine::mod12(int x) {
    int r = x % 12;
    return (r < 0) ? (r + 12) : r;
}

// key = "Ab", genreRuleset = "jazz_major", int = 2-32
std::vector<chord> harmonyEngine::generate(const std::string &key, const genreRuleset& rules, int length) {
    std::string tonic; // тоника
    std::vector<chord> progression; // искомая прогрессия аккордов
    progression.reserve(length); // выделяем память для заполнения

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> noteDist(0, allNotes.size() - 1);
    // ищем тонику среди библиотеки
    for (const auto & allNote : allNotes) {
        if (key == allNote) {
            tonic = key;
        }
    }
    // если не нашли, подбираем случайную тонику
    if (tonic.empty()) {
        tonic = allNotes[noteDist(gen)];
    }

    int tonicNum = noteIndex(tonic);

    std::vector<std::string> degrees; // последовательность ступеней
    degrees.reserve(length);
    std::string firstDegree; // первая всегда тоника
    if (rules.degreeDefinitions.contains("I") &&
    !rules.degreeDefinitions.contains("i")) firstDegree = "i";
    else firstDegree = "I";

    std::string currentDegree = firstDegree;

    for (int i = 0; i < length; i++) {
        degrees.push_back(currentDegree);
        if (i == length-1) break;
        // на последнем аккорде при длине больше 4 аккорд разрешается в тонику
        if (i == length - 2 && length > 4) {
            currentDegree = rules.getClosingChord(currentDegree, firstDegree);
        } else { // иначе строится по правилам построения рулсетов
            currentDegree = rules.getNextChord(currentDegree);
        }
    }

    for (const auto& degree : degrees) {
        auto it = rules.degreeDefinitions.find(degree);
        if (it == rules.degreeDefinitions.end()) {
            // если степень не найдена — fallback в I
            it = rules.degreeDefinitions.find(firstDegree);
            if (it == rules.degreeDefinitions.end()) {
                // ruleset не загружен или неверный
                return {};
            }
        }
        int rootOffset = it->second.first; // 7 для V
        const std::string& chordType = it->second.second; // "maj7"

        std::vector<int> chordRule = rules.getType(chordType); // интервалы относительно корня аккорда
        int chordRootNum = mod12(tonicNum + rootOffset); // корень аккорда (0..11)

        std::string chordName = degree + chordType;
        progression.emplace_back(chordName, chordRootNum, chordRule);
    }
    return progression;
}
        // считаем общий счёт нот по принципу модуль(предыдущая - следующая)
int voiceLeading(const std::vector<chord>& progression) {
}

void saveToFiles(const std::vector<chord>& progression, int bpm) {

}