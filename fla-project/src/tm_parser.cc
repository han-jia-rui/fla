#include <fla/tm.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <utility>

namespace fla {

void TMSimulator::parse(const std::string &filepath) {
    std::clog << "Parsing TM from file: " << filepath << std::endl;
    std::ifstream fin(filepath);

    if (!fin.is_open()) {
        _error_logs.push_back("Error: Could not open the file: " + filepath);
        _error = Error::OtherError;
        error_handler();
    }

    // Init the parse_handlers
    std::vector<std::pair<std::regex, std::function<void(const std::string &)>>> parse_handlers = {
        {
            std::regex(R"(#Q = \{([^}]+)\})"),
            [this](const std::string &line) -> void { this->parse_states(line); },
        },
        {
            std::regex(R"(#S = \{([^}]+)\})"),
            [this](const std::string &line) -> void { this->parse_input_alphabet(line); },
        },
        {
            std::regex(R"(#G = \{([^}]+)\})"),
            [this](const std::string &line) -> void { this->parse_stack_alphabet(line); },
        },
        {
            std::regex(R"(#q0 = ([^ ]+))"),
            [this](const std::string &line) -> void { this->parse_start_state(line); },
        },
        {
            std::regex(R"(#B = ([^ ]+))"),
            [this](const std::string &line) -> void { this->parse_empty_symbol(line); },
        },
        {
            std::regex(R"(#F = \{([^}]+)\})"),
            [this](const std::string &line) -> void { this->parse_accept_states(line); },
        },
        {
            std::regex(R"(#N = ([^ ]+))"),
            [this](const std::string &line) -> void { this->parse_tape_number(line); },
        },
    };

    std::string line;
    size_t line_number = 0;

    while (std::getline(fin, line)) {
        line_number++;

        ignore_after(line, ';');
        strip(line);

        if (line.empty())
            continue;

        bool matched = false;
        for (const auto &handler : parse_handlers) {
            std::smatch match;
            if (std::regex_match(line, match, handler.first)) {
                handler.second(match[1]);
                matched = true;
                break;
            }
        }

        if (!matched)
            parse_transitions(line);

        if (_error != Error::None) {
            _error_logs.push_back("Error in line " + std::to_string(line_number) + ": " + line);
            error_handler();
        }
    }

    { // check the configuration of TM
        if (_states.empty())
            _error_logs.push_back("No states defined");

        if (_input_alphabet.empty())
            _error_logs.push_back("No input alphabet defined");

        if (_tape_alphabet.empty())
            _error_logs.push_back("No stack alphabet defined");

        if (_start_state.empty())
            _error_logs.push_back("No start state defined");

        if (_empty_symbol.empty())
            _error_logs.push_back("No stack start symbol defined");

        if (_accept_states.empty())
            _error_logs.push_back("No accept states defined");

        if (_tape_number <= 0)
            _error_logs.push_back("No tape number defined");

        if (_transitions.empty())
            _error_logs.push_back("No transitions defined");

        if (!_error_logs.empty()) {
            _error = Error::SyntaxError;
            error_handler();
        }
    }
}

void TMSimulator::parse_states(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!State::is_valid(tmp)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid state name: " + tmp);
            return;
        }
        _states.insert(State(tmp));
    }
}

void TMSimulator::parse_input_alphabet(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!Alphabet::is_valid(tmp) && tmp != "_") {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid alphabet symbol: " + tmp);
            return;
        }
        _input_alphabet.add(tmp);
    }
}

void TMSimulator::parse_stack_alphabet(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!Alphabet::is_valid(tmp) && tmp != "_") {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid alphabet symbol: " + tmp);
            return;
        }
        _tape_alphabet.add(tmp);
    }
}

void TMSimulator::parse_start_state(const std::string &line) {
    const std::string &start_state = line;
    if (!State::is_valid(start_state)) {
        _error = Error::SyntaxError;
        _error_logs.push_back("Invalid start state name: " + start_state);
        return;
    }
    _start_state.set_name(start_state);
}

void TMSimulator::parse_empty_symbol(const std::string &line) {
    const std::string &empty_symbol = line;
    if (empty_symbol != "_") {
        _error = Error::SyntaxError;
        _error_logs.push_back("Invalid stack start symbol: " + empty_symbol);
        return;
    }
    _empty_symbol = empty_symbol;
}

void TMSimulator::parse_accept_states(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!State::is_valid(tmp)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid accept state name: " + tmp);
            return;
        }
        _accept_states.insert(State(tmp));
    }
}

void TMSimulator::parse_tape_number(const std::string &line) {
    try {
        int tape_number = std::stoi(line);
        if (tape_number <= 0)
            throw std::out_of_range("tape_number must be greater than 0");

        _tape_number = static_cast<size_t>(tape_number);
    } catch (const std::invalid_argument &) {
        _error = Error::SyntaxError;
        _error_logs.push_back("Invalid tape_number: " + line);
    } catch (const std::out_of_range &) {
        _error = Error::SyntaxError;
        _error_logs.push_back("Tape_number out of range: " + line);
    }
}

void TMSimulator::parse_transitions(const std::string &line) {
    std::stringstream ss(line);
    std::string item;
    std::vector<std::string> elements;

    while (ss >> item)
        elements.push_back(item);

    if (elements.size() != 5) {
        _error_logs.push_back("Incorrect transition format");
        _error = Error::SyntaxError;
        return;
    }

    const std::string &from_state = elements[0];
    const std::string &old_str = elements[1];
    const std::string &new_str = elements[2];
    const std::string &direction_str = elements[3];
    const std::string &to_state = elements[4];

    { // check the grammar of transition

        if (_states.find(State(from_state)) == _states.end())
            _error_logs.push_back("Invalid from state name: " + from_state);

        if (old_str.size() != _tape_number)
            _error_logs.push_back("Invalid old string: " + old_str);
        for (char c : old_str)
            if (c != '*' && !_tape_alphabet.contains(std::string(1, c)))
                _error_logs.push_back("Invalid old string: " + old_str);

        if (new_str.size() != _tape_number)
            _error_logs.push_back("Invalid new string: " + new_str);
        for (char c : new_str)
            if (c != '*' && !_tape_alphabet.contains(std::string(1, c)))
                _error_logs.push_back("Invalid new string: " + new_str);

        if (direction_str.size() != _tape_number)
            _error_logs.push_back("Invalid direction string: " + direction_str);
        for (char c : direction_str)
            if (c != 'l' && c != 'r' && c != '*')
                _error_logs.push_back("Invalid direction string: " + direction_str);

        if (_states.find(State(to_state)) == _states.end())
            _error_logs.push_back("Invalid to state name: " + to_state);

        for (size_t i = 0; i < new_str.size(); ++i)
            if (new_str[i] == '*' && old_str[i] != '*')
                _error_logs.push_back("Invalid string transition: " + old_str + " -> " + new_str);

        if (!_error_logs.empty()) {
            _error = Error::SyntaxError;
            return;
        }
    }

    Condition condition = std::make_tuple(from_state, old_str);
    Action action = std::make_tuple(new_str, direction_str, to_state);

    for (const auto &transition : _transitions) {
        if (transition.first == condition) {
            _error_logs.push_back("Duplicate transition condition");
            _error = Error::SyntaxError;
            return;
        }
    }

    _transitions.push_back(std::make_pair(condition, action));
}

} // namespace fla
