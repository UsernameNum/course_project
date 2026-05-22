#include <utility>

#include "chord.h"


chord::chord(std::string name, int root, std::vector<int> chord):
    chordName(std::move(name)), rootNote(root), chordRule(std::move(chord)) {

}

void complexityLogic(int level) {

}