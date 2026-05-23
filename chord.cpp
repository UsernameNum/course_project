#include <utility>
#include "chord.h"

// создание аккорда (cm7, c, (3, 6, 10, 1))
chord::chord(std::string name, int root, std::vector<int> rule):
    chordName(std::move(name)), rootNote(root), chordRule(std::move(rule)) {

}

std::vector<int> chord::getAbsoluteNotes() const {
    std::vector<int> absoluteNotes;

    for (int i : chordRule) {
        int note = (rootNote + i) % 12;
        absoluteNotes.push_back(note);
    }
    return absoluteNotes;
}

// количество изменений аккордов в зависимости от сложности (1-3)
void complexityLogic(int level) {

}