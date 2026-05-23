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
    genreRuleset() {
        chordLibrary["min"] = {0, 3, 7};
        chordLibrary["maj"] = {0, 4, 7};
        chordLibrary["dim7"] = {0, 3, 6, 9};
        chordLibrary["min6"] = {0, 3, 7, 9};
        chordLibrary["min7"] = {0, 3, 7, 10};
        chordLibrary["7"] = {0, 4, 7, 10};
        chordLibrary["maj7"] = {0, 4, 7, 11};
        chordLibrary["m7b5"] = {0, 3, 6, 10};
    }

    bool loadFromFile(const std::string &fileName);
    std::vector<int> getType(std::string type);
    std::string getNextChord(std::string current);
};

#endif //COURSE_PROJECT_GENRERULESET_H
