#ifndef COURSE_PROJECT_CHORD_H
#define COURSE_PROJECT_CHORD_H
#include <string>
#include <utility>
#include <vector>

/*  0  1  2  3  4  5  6  7  8  9  10 11
 *  a  a# b  c  c# d  d# e  f  f# g  g#
 */

class chord {
protected:
    std::string chordName;
    int rootNote;
    std::vector<int> chordRule;
public:
    chord() = default;
    chord(std::string name, int root, std::vector<int> chord);

    void complexityLogic(int level);
};

#endif //COURSE_PROJECT_CHORD_H