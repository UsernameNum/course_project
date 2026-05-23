#include <utility>

#include "chord.h"

// создание аккорда (cm7, c, (3, 6, 10, 1))
chord::chord(std::string name, int root, std::vector<int> chord):
    chordName(std::move(name)), rootNote(root), chordRule(std::move(chord)) {

}

std::vector<int> getAbsoluteNotes() {

}

// количество изменений аккордов в зависимости от сложности (1-3)
void complexityLogic(int level) {

}