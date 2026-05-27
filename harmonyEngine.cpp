#include "harmonyEngine.h"

#include <algorithm>
#include <iostream>
#include <random>

/*  0  1  2  3  4  5  6  7  8  9  10 11
 *  c  c# d  d# e  f  f# g  g# a  a# b
 */

static const std::vector<std::string> allNotes = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

// с = min(mod(a-b), 12 - mod(a-b))
int minCost(int x, int y) {
    int n = abs(x - y);
    return std::min(n, 12-n) * std::min(n, 12-n); // штраф на большие скачки
}

// возвращает индекс ноты
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

// конвертировать ступени в полутона -> использовать degreeDefinitions<ступень, <полутона, тип>>

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
    !rules.degreeDefinitions.contains("i")) firstDegree = "I";
    else firstDegree = "i";

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

        std::string chordName = allNotes[chordRootNum] + chordType;
        progression.emplace_back(chordName, chordRootNum, chordRule);
    }
    return progression;
}
// chord("Am", 9, {0, 3, 7})
// chord("D5", 2, {0, 7})
// chord("E7", 4, {0, 4, 7, 10})
// chord("Am", 9, {0, 3, 7})
int voiceLeading(const std::vector<chord>& progression) {
    // двумерные вектора: нот, минимальных переходов конкретной ноты в следующую и её индекс
    std::vector<std::vector<int>> notes(progression.size()), dp(progression.size()), nextID(progression.size());
    for (int i = 0; i < progression.size(); i++) { notes[i] = progression[i].getAbsoluteNotes(); } // заполнение массива нот
    for (int i = 0; i < progression.size(); i++) { // каждый аккорд
        for (int j = 0; j < progression[i].getRule().size(); j++) { // каждая нота этого аккорда
            nextID[i][j] = -1; // первоначальный индекс до обработки
            dp[i][j] = minCost(notes[i][j], notes[i+1][0]) + dp[i+1][0];
            for (int k = 0; k < progression[i].getRule().size(); k++) { // каждая нота следующего аккорда
                // минимальный шаг между текущим и следующим аккордом + стоимость для каждого следующего
                int cand = minCost(notes[i][j], notes[i+1][k]) + dp[i+1][k];
                if (cand < dp[i][j]) { // если это минимум - запоминаем
                    dp[i][j] = cand;
                    nextID[i][j] = k;
                }
            }
        }
    }
    for (int i = 0; i < progression.size(); i++) {
        for (int j = 0; j < progression[i].getRule().size(); j++) {
            std::cout << dp[i][j] << " ";
        }
    }
    std::cout << std::endl;
    for (int i = 0; i < progression.size(); i++) {
        for (int j = 0; j < progression[i].getRule().size(); j++) {
            std::cout << nextID[i][j] << " ";
        }
    }
}

void saveToFiles(const std::vector<chord>& progression, int bpm) {

}