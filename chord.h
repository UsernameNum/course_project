#ifndef COURSE_PROJECT_CHORD_H
#define COURSE_PROJECT_CHORD_H
#include <string>
#include <vector>

class chord {
protected:
    std::string chordName;  // полное название
    int rootNote;           // тоника (комментарий над классом)
    std::vector<int> chordRule; // полутоны от 0 - тоники
public:
    chord(std::string name, int root, std::vector<int> rule);
    std::string getName() const { return chordName; }
    std::vector<int> getRule() const { return chordRule; }
    std::vector<int> getAbsoluteNotes() const;
};

#endif //COURSE_PROJECT_CHORD_H