#include "harmonyEngine.h"

#include <fstream>
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
    int n = std::abs(x - y);
    int d = std::min(n, 12-n);
    return d*d; // штраф на большие скачки
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
    progression.reserve(length);

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
    std::cout << "tonic='" << tonic << "' tonicNum=" << tonicNum << "\n";

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

        std::cout << chordName << " root=" << chordRootNum << " rule:";
        for (int r: chordRule) std::cout << " " << r;
        std::cout << " abs:";
        for (int a: chord(chordName, chordRootNum, chordRule).getAbsoluteNotes()) std::cout << " " << a;
        std::cout << "\n";

        progression.emplace_back(chordName, chordRootNum, chordRule);

    }
    return progression;
}
// chord("Am", 9, {0, 3, 7})
// chord("D5", 2, {0, 7})
// chord("E7", 4, {0, 4, 7, 10})
// chord("Am", 9, {0, 3, 7})
std::vector<std::string> harmonyEngine::voiceLeading(const std::vector<chord> &progression) {
    int N = (int)progression.size();
    int INF = 1e9;
    if (N == 0) return {};

    // двумерные вектора: нот, минимальных переходов конкретной ноты в следующую и её индекс
    std::vector<std::vector<int>> notes(progression.size()), dp(progression.size()), nextID(progression.size());
    for (int i = 0; i < N; i++) {
        notes[i] = progression[i].getAbsoluteNotes(); // заполнение массива нот
        dp[i].assign(notes[i].size(), INF); // начальные переходы всега меньше
        nextID[i].assign(notes[i].size(), -1); // начальный индекс
    }
    for (int j = 0; j < (int)notes[N-1].size(); ++j) dp[N-1][j] = 0; // последний аккорд

    for (int i = N-2; i >= 0; --i) { // каждый аккорд
        for (int j = 0; j < notes[i].size(); ++j) { // каждая нота этого аккорда
            for (int k = 0; k < notes[i+1].size(); ++k) { // каждая нота следующего аккорда
                // минимальный шаг между текущим и следующим аккордом + стоимость для каждого следующего
                int cand = minCost(notes[i][j], notes[i+1][k]) + dp[i+1][k];
                if (cand < dp[i][j]) { // если это минимум - запоминаем
                    dp[i][j] = cand;
                    nextID[i][j] = k;
                }
            }
        }
    }

    // выбираем лучший старт на 0-м аккорде
    int jStart = 0;
    for (int j = 1; j < (int)dp[0].size(); ++j)
        if (dp[0][j] < dp[0][jStart]) jStart = j;

    // восстановление lead
    std::vector<int> leadNum(N);
    int j = jStart;
    for (int i = 0; i < N; ++i) {
        leadNum[i] = notes[i][j];
        if (i == N - 1) break;
        j = nextID[i][j];
        if (j < 0) break; // на случай пустых данных
    }
    std::vector<std::string> leadNames;
    leadNames.reserve(N);
    for (int pc : leadNum) leadNames.push_back(allNotes[pc]);

    return leadNames;
}

bool harmonyEngine::saveToTxt(const std::vector<chord>& progression, const genreRuleset& g) {
    std::ofstream out ("output.txt");
    if (!out) return false;
    if (progression.empty()) {
        out << "Empty progression\n";
        return false;
    }

    std::string genreName = g.getGenreName();
    const auto& prog = progression;
    std::vector<std::string> lead = voiceLeading(progression);
    std::vector<int> lint;
    for (const auto & i : lead) lint.push_back(noteIndex(i));
    int cost = 0;
    for (size_t i = 0; i + 1 < lint.size(); ++i) {
        int d = std::abs(lint[i] - lint[i+1]);
        d = std::min(d, 12 - d);
        cost += d*d;
    }

    out << "=== HARMONY GENERATOR ===\n\n";

    out << "This is but a tool to help You with creative tasks and not a \"do a song\" type AI.\n";
    out << "You can create chord sequence via setting one of many different and infinitely expanding rulesets, key and length.\n";
    out << "Logic of generating is based around loops witch allows you to jam easily or use it in your verses!\n\n";

    out << "    Chosen genre: " << genreName << "\n";
    out << "    Key: " << prog[0].getName() << "\n";
    out << "    Length: " << prog.size() << " chords \n\n";

    out << "Generated progression:\n";
    for (int i = 0; i < (int)prog.size(); ++i) { // 1. Gmaj7: {G, B, D, F#}
        auto abs = prog[i].getAbsoluteNotes();
        out << i << ". " << prog[i].getName()  << ": {";
        if (abs.empty()) {
            out << "}\n";            // или "{(empty)}\n"
            continue;
        }
        for (int j = 0; j < (int)abs.size()-1; ++j) out << allNotes[abs[j]] << ", ";
        out << allNotes[abs.size()-1] << "}\n";
    } out << "\n";

    out << "Best found lead in this progression:\n";
    if (lead.empty()) {
        out << "(empty lead)\n";
    } else {
        for (size_t i = 0; i + 1 < lead.size(); ++i) out << lead[i] << " - ";
        out << lead.back() << "\n";
    }

    return true;
}