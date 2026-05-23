#ifndef COURSE_PROJECT_GENRERULESET_H
#define COURSE_PROJECT_GENRERULESET_H

#include <map>
#include <string>
#include <vector>

class genreRuleset {
protected:
    std::string genreName;
    std::map <std::string, std::vector<int>> chordLibrary;
    std::map <std::string, std::vector<std::string>> transitionArray;
public:
    bool loadFromFile(std::string fileName);
    std::vector<int> getType(std::string type);
    std::string getNextChord(std::string current);
};

#endif //COURSE_PROJECT_GENRERULESET_H
