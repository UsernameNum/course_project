#include <iostream>
#include <string>
#include <vector>
#include <limits>
#include "chord.h"
#include "genreRuleset.h"
#include "harmonyEngine.h"

static const std::vector<std::string> allNotes = {
    "C","C#","D","D#","E","F","F#","G","G#","A","A#","B"
};

static void flushLine() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

static int readInt(const std::string& prompt, int minV, int maxV) {
    while (true) {
        std::cout << prompt;
        int x;
        if (std::cin >> x) {
            flushLine();
            if (x >= minV && x <= maxV) return x;
            std::cout << "Please enter a number in range [" << minV << ".." << maxV << "].\n";
        } else {
            std::cin.clear();
            flushLine();
            std::cout << "Please enter a NUMBER.\n";
        }
    }
}

static std::string readLine(const std::string& prompt) {
    std::cout << prompt;
    std::string s;
    std::getline(std::cin, s);
    return s;
}

static void printProgressionToConsole(const std::vector<chord>& progression) {
    for (const auto& ch : progression) {
        auto abs = ch.getAbsoluteNotes();
        std::cout << ch.getName() << " : {";
        for (size_t i = 0; i < abs.size(); ++i) {
            int pc = abs[i];
            if (pc < 0 || pc >= (int)allNotes.size()) {
                std::cout << "[BAD_PC=" << pc << "]";
                break;
            }
            std::cout << allNotes[pc];
            if (i + 1 < abs.size()) std::cout << ", ";
        }
        std::cout << "}\n";
    }
}

int main() {
    std::cout <<
        "=== MAIN MENU ===\n"
        "This is but a tool to help You with creative tasks and not a \"do a song\" type AI.\n"
        "You can create chord sequence via setting one of many different and infinitely expanding rulesets, key and length.\n\n";

    harmonyEngine harmony;

    std::string tonic; // "" => рандомим
    std::string genreS; // "blues_major.txt"
    int length = 4;
    genreRuleset genre;
    std::vector<chord> progression;
    bool haveParams = false; // введены ли параметры
    bool haveProgression = false; // сгенерирована ли прогрессия

    while (true) {
        // Если параметров нет или пользователь выбрал "change input data" — спрашиваем заново
        if (!haveParams) {
            std::cout <<
                "You may choose tonic, genre and length (2-64)\n"
                "Rulesets: blues_major.txt; blues_minor.txt; jazz_major.txt; jazz_minor.txt; pop_major.txt; pop_minor.txt; rock.txt\n";
            // чтобы можно было оставить пустую строку, используем getline
            tonic = readLine("Choose tonic (C, C#, ... B) or leave blank for random: ");
            genreS = readLine("Write genre file name (e.g. blues_major.txt): ");
            length = readInt("Write length (2-64): ", 2, 64);
            if (!genre.loadFromFile(genreS)) {
                std::cout << "Failed to load ruleset. Check file name/path.\n\n";
                haveParams = false;
                continue;
            }
            haveParams = true;
            haveProgression = false;
        }

        // Если прогрессии ещё нет — генерим
        if (!haveProgression) {
            progression = harmonyEngine::generate(tonic, genre, length);
            if (progression.empty()) {
                std::cout << "Failed to generate progression (empty result). Try different ruleset/length.\n\n";
                haveParams = false;
                continue;
            }
            haveProgression = true;
        }
        std::cout <<
            "\nGreat! You've generated chord progression. Do you want to:\n"
            "1 - write it here;\n"
            "2 - output it into .txt file;\n"
            "3 - reroll it again (same input data);\n"
            "4 - change input data;\n"
            "0 - quit:\n";
        int choice = readInt("> ", 0, 4);
        if (choice == 0) {
            std::cout << "quit\n";
            break;
        }
        switch (choice) {
            case 1: {
                std::cout << genre.getGenreName() << "\n";
                printProgressionToConsole(progression);

                auto lead = harmonyEngine::voiceLeading(progression);
                if (!lead.empty()) {
                    std::cout << "Lead: ";
                    for (size_t i = 0; i < lead.size(); ++i) {
                        if (i) std::cout << " - ";
                        std::cout << lead[i];
                    }
                    std::cout << "\n";
                } else std::cout << "Lead: (empty)\n";
                break;
            }
            case 2: {
                if (harmonyEngine::saveToTxt(progression, genre)) std::cout << "Saved to output.txt\n";
                else std::cout << "Failed to save output.txt\n";
                break;
            }
            case 3: {
                haveProgression = false; // пересоздаст progression с теми же tonic/genre/length
                break;
            }
            case 4: {
                haveParams = false; // спросит tonic/genre/length заново
                haveProgression = false;
                break;
            }
            default:
                break;
        }
    }

    return 0;
}