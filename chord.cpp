#include <utility>
#include "chord.h"

// создание аккорда (cm7, 0, (0, 3, 7, 10))
chord::chord(std::string name, int root, std::vector<int> rule):
    chordName(std::move(name)), rootNote(root), chordRule(std::move(rule)) {}

// берём ноты аккорда относительно тоники
std::vector<int> chord::getAbsoluteNotes() const {
    std::vector<int> absoluteNotes;
    for (int i : chordRule) { // (7 + 7) % 12 = 2 || G + V = D
        int note = (rootNote + i) % 12;
        absoluteNotes.push_back(note);
    }
    return absoluteNotes;
}