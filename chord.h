#ifndef COURSE_PROJECT_CHORD_H
#define COURSE_PROJECT_CHORD_H
#include <string>
#include <vector>

/*  0  1  2  3  4  5  6  7  8  9  10 11
 *  a  a# b  c  c# d  d# e  f  f# g  g#
 */

class chord {
protected:
    std::string chordName;  // полное название
    int rootNote;           // тоника (комментарий над классом)
    std::vector<int> chordRule; // полутоны от 0 - тоники
public:
    chord(std::string name, int root, std::vector<int> rule);

    std::string getName() { return chordName; }

    std::vector<int> getAbsoluteNotes() const;
    void complexityLogic(int level);
};

#endif //COURSE_PROJECT_CHORD_H