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

static const std::vector<std::string> allNotes = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

int main() {
    chord chC("G#", 3, {0,4,7});
    std::cout << "chord: " << chC.getName() << std::endl;
    std::cout << "its absolute notes: " << std::endl;
    std::vector<int> notes = chC.getAbsoluteNotes();
    for (int note : notes) {
        std::cout << note << " ";
    } std::cout << std::endl;

    genreRuleset jazz;
    jazz.loadFromFile("jazz_major.txt");
    harmonyEngine engine;
    std::vector<chord> g = engine.generate("G", jazz, 4);

    std::cout << jazz.genreName << " in " << std::endl;
    for (auto note : g) {
        std::cout << note.getName() << " {";
        std::vector<int> nts = note.getAbsoluteNotes();
        for (int n = 0; n < nts.size()-1; n++) {
            std::cout << allNotes[n] << ", ";
        } std::cout << allNotes[nts[nts.size()-1]] << "}" << std::endl;
    }
    std::vector<std::string> gLead = engine.voiceLeading(g);
    for (int n = 0; n < gLead.size()-1; n++) {
        std::cout << gLead[n] << " - ";
    } std::cout << gLead[gLead.size()-1] << std::endl;
}