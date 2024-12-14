#include <fla/pda.h>

#include <fstream>
#include <functional>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace fla {

void PDASimulator::parse(const std::string &filepath) {
    std::clog << "Parsing PDM from file: " << filepath << std::endl;
    std::ifstream fin(filepath);

    if (!fin.is_open()) {
        _error_logs.push_back("Error: Could not open the file: " + filepath);
        _error = Error::OtherError;
        error_handler();
    }

    // init parse_handlers
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
            std::regex(R"(#z0 = ([^ ]+))"),
            [this](const std::string &line) -> void { this->parse_stack_start_symbol(line); },
        },
        {
            std::regex(R"(#F = \{([^}]+)\})"),
            [this](const std::string &line) -> void { this->parse_accept_states(line); },
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

    { // check PDA configuration
        if (_states.empty())
            _error_logs.push_back("No states defined");

        if (_input_alphabet.empty())
            _error_logs.push_back("No input alphabet defined");

        if (_stack_alphabet.empty())
            _error_logs.push_back("No stack alphabet defined");

        if (_start_state.empty())
            _error_logs.push_back("No start state defined");

        if (_stack_start_symbol.empty())
            _error_logs.push_back("No stack start symbol defined");

        if (_accept_states.empty())
            _error_logs.push_back("No accept states defined");

        if (_transitions.empty())
            _error_logs.push_back("No transitions defined");

        for (const auto &transition : _transitions) {
            const auto &from_state = std::get<0>(transition.first);
            const auto &input_char = std::get<1>(transition.first);
            const auto &stack_top = std::get<2>(transition.first);

            if (input_char != '_') {
                Condition empty_input_condition = std::make_tuple(from_state, '_', stack_top);
                if (_transitions.find(empty_input_condition) != _transitions.end()) {
                    _error_logs.push_back("Duplicate transition condition:");
                    _error_logs.push_back(from_state.name() + " " + input_char + " " + stack_top);
                    _error_logs.push_back(from_state.name() + " " + '_' + " " + stack_top);
                }
            }
        }

        if (!_error_logs.empty()) {
            _error = Error::SyntaxError;
            error_handler();
        }
    }
}

void PDASimulator::parse_states(const std::string &line) {
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

void PDASimulator::parse_input_alphabet(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!Alphabet::is_valid(tmp)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid alphabet symbol: " + tmp);
            return;
        }
        _input_alphabet.add(tmp);
    }
}

void PDASimulator::parse_stack_alphabet(const std::string &line) {
    std::stringstream ss(line);
    std::string tmp;
    while (std::getline(ss, tmp, ',')) {
        strip(tmp);
        if (!Alphabet::is_valid(tmp)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid alphabet symbol: " + tmp);
            return;
        }
        _stack_alphabet.add(tmp);
    }
}

void PDASimulator::parse_start_state(const std::string &line) {
    const std::string &start_state = line;
    if (!State::is_valid(start_state)) {
        _error = Error::SyntaxError;
        _error_logs.push_back("Invalid start state name: " + start_state);
        return;
    }
    _start_state.set_name(start_state);
}

void PDASimulator::parse_stack_start_symbol(const std::string &line) {
    const std::string &stack_start_symbol = line;
    if (!Alphabet::is_valid(stack_start_symbol)) {
        _error = Error::SyntaxError;
        _error_logs.push_back("Invalid stack start symbol: " + stack_start_symbol);
        return;
    }
    _stack_start_symbol = stack_start_symbol;
}

void PDASimulator::parse_accept_states(const std::string &line) {
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

void PDASimulator::parse_transitions(const std::string &line) {
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
    const std::string &input_char = elements[1];
    const std::string &stack_top = elements[2];
    const std::string &to_state = elements[3];
    const std::string &stack_push = elements[4];

    { // check transition grammar
        if (_states.find(State(from_state)) == _states.end())
            _error_logs.push_back("Invalid from state name: " + from_state);

        if (input_char != "_" &&
            !_input_alphabet.contains(input_char)) // The input char can be empty
            _error_logs.push_back("Invalid input character: " + input_char);

        if (!_stack_alphabet.contains(stack_top)) // The stack top can not be empty
            _error_logs.push_back("Invalid stack top symbol: " + stack_top);

        if (_states.find(State(to_state)) == _states.end())
            _error_logs.push_back("Invalid to state name: " + to_state);

        if (stack_push != "_") { // The stack push string can be empty
            for (char c : stack_push) {
                if (!_stack_alphabet.contains(std::string(1, c))) {
                    _error_logs.push_back("Invalid stack push symbol: " + stack_push);
                    break;
                }
            }
        }

        if (!_error_logs.empty()) {
            _error = Error::SyntaxError;
            return;
        }
    }

    Condition condition = std::make_tuple(State(from_state), input_char[0], stack_top[0]);
    Action action = std::make_tuple(State(to_state), stack_push);

    if (_transitions.find(condition) != _transitions.end()) {
        _error_logs.push_back("Duplicate transition condition");
        _error = Error::SyntaxError;
        return;
    }

    _transitions[condition] = action;
}

} // namespace fla
