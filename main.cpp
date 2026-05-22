#include <iostream>

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
    std::cout << "Hello, World!" << std::endl;
    return 0;
}