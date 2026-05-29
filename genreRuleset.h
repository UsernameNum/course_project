#ifndef COURSE_PROJECT_GENRERULESET_H
#define COURSE_PROJECT_GENRERULESET_H

#include <map>
#include <string>
#include <vector>

// оптимизация большой карты переходов
struct Transition {
    std::string nextChord;
    double probability;
};

class genreRuleset {
    std::string genreName;
    std::map <std::string, std::vector<int>> chordLibrary;
    std::map<std::string, std::vector<Transition>> transitions;

    std::string Tonic() const;
    void parseChordLine(const std::string& line);
    void parseTransitionLine(const std::string& line);
public:
    std::map<std::string, std::pair<int, std::string>> degreeDefinitions; // мы будем обращаться к нему непосредственно в движке

    genreRuleset() { // библиотека подгружается при создании
        chordLibrary["5"] = {0, 7};
        chordLibrary["min"] = {0, 3, 7};
        chordLibrary["maj"] = {0, 4, 7};
        chordLibrary["dim"] = {0, 3, 6};
        chordLibrary["dim7"] = {0, 3, 6, 9};
        chordLibrary["min6"] = {0, 3, 7, 9};
        chordLibrary["min7"] = {0, 3, 7, 10};
        chordLibrary["7"] = {0, 4, 7, 10};
        chordLibrary["maj7"] = {0, 4, 7, 11};
        chordLibrary["m7b5"] = {0, 3, 6, 10};
    }

    bool loadFromFile(const std::string &fileName);

    std::vector<int> getType(const std::string &type) const;
    std::string getGenreName() const { return genreName; }

    std::string getNextChord(const std::string &current) const;
    std::string getClosingChord(const std::string &from, const std::string &to) const;
};

#endif //COURSE_PROJECT_GENRERULESET_H