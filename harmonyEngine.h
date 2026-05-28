#ifndef COURSE_PROJECT_HARMONYENGINE_H
#define COURSE_PROJECT_HARMONYENGINE_H
#include <vector>
#include "chord.h"
#include "genreRuleset.h"

class harmonyEngine {
    std::string currentDegree = "I"; // первый элемент всегда тоника
    static int noteIndex(const std::string& name);
    static int mod12(int x);
public:
    std::vector<chord> generate(const std::string &key = "", const genreRuleset &rules = {}, int length = 4);

    std::vector<std::string> voiceLeading(const std::vector<chord> &progression);
    void saveToTxt(const std::vector<chord>& progression, int bpm);
};

#endif //COURSE_PROJECT_HARMONYENGINE_H
