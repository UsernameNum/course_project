#ifndef COURSE_PROJECT_HARMONYENGINE_H
#define COURSE_PROJECT_HARMONYENGINE_H
#include <vector>

#include "chord.h"
#include "genreRuleset.h"


class harmonyEngine {
    std::vector<chord> generate(genreRuleset rules, int length, int complexity);
    int voiceLeading(std::vector<chord> progression);
    void saveToFiles(std::vector<chord> progression, int bpm);
};



#endif //COURSE_PROJECT_HARMONYENGINE_H
