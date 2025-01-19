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

void Simulator::set_verbose(bool verbose) noexcept {
    std::clog << "Verbose mode: " << (verbose ? "on" : "off") << std::endl;
    _verbose = verbose;
}

void Simulator::reset() noexcept {
    _verbose = false;
    _error = Error::None;
    _error_logs.clear();
}

void Simulator::error_handler() {
    if (_error == Error::None)
        return;

    if (!_verbose) {
        switch (_error) {
        case Error::SyntaxError:
            std::cerr << "syntax error" << std::endl;
            break;
        case Error::InputError:
            std::cerr << "illegal input" << std::endl;
            break;
        default:
            break;
        }
    } else {
        std::clog << "Error logs:" << std::endl;
        for (const std::string &error : _error_logs) {
            std::cerr << error << std::endl;
        }
        std::cerr << "==================== END ====================" << std::endl;
    }

    throw _error;
}

} // namespace fla
