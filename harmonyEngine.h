#ifndef COURSE_PROJECT_HARMONYENGINE_H
#define COURSE_PROJECT_HARMONYENGINE_H
#include <vector>
#include "chord.h"
#include "genreRuleset.h"

class harmonyEngine {
    std::string currentDegree = "I"; // первый элемент всегда тоника
public:
    std::vector<chord> generate(const std::string &key = "", const genreRuleset &rules = {}, int length = 4);
    int voiceLeading(const std::vector<chord>& progression);
    void saveToFiles(const std::vector<chord>& progression, int bpm);
};

#endif //COURSE_PROJECT_HARMONYENGINE_H
