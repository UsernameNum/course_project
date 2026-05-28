#include <iostream>

#include "chord.h"
#include "genreRuleset.h"
#include "harmonyEngine.h"

/* Моделирование процесса подбора гармонии музыкальной композиции
 * chord:
 *  name
 *  notes
 *  void complexityLogic(int level)
 * genreRuleset:
 *  genreName
 *  map <name, vector<int(notes)>> chordLibrary - словарь аккордов
 *  map <name, vector<string(chords)>> transitionArray - цепи маркова для подбора аккордов
 * harmonyGenerator:
 *  vector<chord> generate(GenreRules rules, int length, int complexity) - создание гармонии
 *  int voiceLeading(vector<Chord> progression) - плавность переходов
 *  void saveToFiles(vector<Chord> progression, int bpm) - сохранение результата в файлы
 */

int main() {
    chord chC("C", 3, {0,4,7});
    chord chAm("A#m", 1, {0,3,7});

    std::cout << "chord: " << chC.getName() << std::endl;
    std::cout << "its absolute notes: " << std::endl;
    std::vector<int> notes = chC.getAbsoluteNotes();
    for (int note : notes) {
        std::cout << note << " ";
    } std::cout << std::endl;

    genreRuleset jazz;
    jazz.loadFromFile("jazz_minor.txt");
    harmonyEngine engine;
    std::vector<chord> g = engine.generate("A#", jazz, 4);
    for (auto note : g) {
        std::cout << note.getName() << " ";
        std::vector<int> notes2 = note.getAbsoluteNotes();
        std::cout << std::endl;
        for (int note2 : notes2) {
            std::cout << note2 << " ";
        } std::cout << std::endl;
    }
    std::vector<std::string> gLead = engine.voiceLeading(g);
    for (const auto& note : gLead) {
        std::cout << note << " ";
    }
}