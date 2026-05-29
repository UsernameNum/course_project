#ifndef COURSE_PROJECT_HARMONYENGINE_H
#define COURSE_PROJECT_HARMONYENGINE_H
#include <vector>
#include "chord.h"
#include "genreRuleset.h"

class harmonyEngine {
    static int noteIndex(const std::string& name);
    static int mod12(int x);
public:
    static std::vector<chord> generate(const std::string &key = "", const genreRuleset &rules = {}, int length = 4);
    static std::vector<std::string> voiceLeading(const std::vector<chord> &progression);
    static bool saveToTxt(const std::vector<chord>& progression, const genreRuleset& g);
};

#endif //COURSE_PROJECT_HARMONYENGINE_H
