#ifndef COURSE_PROJECT_CHORD_H
#define COURSE_PROJECT_CHORD_H
#include <string>
#include <utility>
#include <vector>


class chord {
protected:
    std::string chordName;
    int rootNote;
    std::vector<int> chordRule;
public:
    chord() = default;
    chord(std::string name, int root, std::vector<int> chord): chordName(std::move(name)), rootNote(root), chordRule(std::move(chord)) {}

    void complexityLogic(int level);
};



#endif //COURSE_PROJECT_CHORD_H
