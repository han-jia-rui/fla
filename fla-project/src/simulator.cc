#include <fla/simulator.h>

#include <iostream>
#include <regex>
#include <string>

namespace fla {

bool State::is_valid(std::string name) {
    std::regex stateNameRegex("^[a-zA-Z0-9_]+$");
    return std::regex_match(name, stateNameRegex);
}

bool Alphabet::is_valid(std::string s) {
    if (s.size() != 1) {
        return false;
    }
    std::set<char> invalidChars = {' ', ',', ';', '{', '}', '*', '_'};
    return std::isprint(s[0]) && invalidChars.find(s[0]) == invalidChars.end();
}

void Simulator::set_verbose(bool verbose) {
    std::clog << "Verbose mode: " << (verbose ? "on" : "off") << std::endl;
    _verbose = verbose;
}

} // namespace fla
