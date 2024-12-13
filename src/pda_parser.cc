#include <fla/pda.h>

#include <fstream>
#include <iostream>
#include <regex>
#include <sstream>
#include <string>

namespace fla {

void PDASimulator::parse(const std::string &filepath) {
    std::clog << "Parsing PDM from file: " << filepath << std::endl;
    std::ifstream fin(filepath);

    if (!fin.is_open()) {
        std::cerr << "Error: Could not open the file: " << filepath << std::endl;
        exit(EXIT_FAILURE);
    }

    std::string line;
    size_t line_number = 0;

    while (std::getline(fin, line)) {
        line_number++;

        ignore_after(line, ';');
        strip(line);
        if (line.empty())
            continue;

        if (line.compare(0, 2, "#Q") == 0)
            parse_states(line);
        else if (line.compare(0, 2, "#S") == 0)
            parse_input_alphabet(line);
        else if (line.compare(0, 2, "#G") == 0)
            parse_stack_alphabet(line);
        else if (line.compare(0, 3, "#q0") == 0)
            parse_start_state(line);
        else if (line.compare(0, 3, "#z0") == 0)
            parse_stack_start_symbol(line);
        else if (line.compare(0, 2, "#F") == 0)
            parse_accept_states(line);
        else
            parse_transitions(line);

        if (_error != Error::None) {
            _error_logs.push_back("Error in line " + std::to_string(line_number) + ": " + line);
            error_handler();
        }
    }

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

    if (_error_logs.size() > 0) {
        _error = Error::SyntaxError;
        error_handler();
    }
}

void PDASimulator::parse_states(std::string &line) {
    std::regex stateLineRegex(R"(#Q = \{([^}]+)\})");
    std::smatch match;

    if (std::regex_match(line, match, stateLineRegex)) {
        std::string states = match[1];
        std::stringstream ss(states);
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
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing states");
        return;
    }
}

void PDASimulator::parse_input_alphabet(std::string &line) {
    std::regex alphabetLineRegex(R"(#S = \{([^}]+)\})");
    std::smatch match;
    if (std::regex_match(line, match, alphabetLineRegex)) {
        std::string alphabet = match[1];
        std::stringstream ss(alphabet);
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
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing input alphabet");
        return;
    }
}

void PDASimulator::parse_stack_alphabet(std::string &line) {
    std::regex alphabetLineRegex(R"(#G = \{([^}]+)\})");
    std::smatch match;
    if (std::regex_match(line, match, alphabetLineRegex)) {
        std::string alphabet = match[1];
        std::stringstream ss(alphabet);
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
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing stack alphabet");
        return;
    }
}

void PDASimulator::parse_start_state(std::string &line) {
    std::regex startStateLineRegex(R"(#q0 = ([a-zA-Z0-9_]+))");
    std::smatch match;
    if (std::regex_match(line, match, startStateLineRegex)) {
        std::string start_state = match[1];
        if (!State::is_valid(start_state)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid start state name: " + start_state);
            return;
        }
        _start_state.set_name(start_state);
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing start state");
        return;
    }
}

void PDASimulator::parse_stack_start_symbol(std::string &line) {
    std::regex stackStartSymbolLineRegex(R"(#z0 = ([^ ]))");
    std::smatch match;
    if (std::regex_match(line, match, stackStartSymbolLineRegex)) {
        std::string stack_start_symbol = match[1];
        if (!Alphabet::is_valid(stack_start_symbol)) {
            _error = Error::SyntaxError;
            _error_logs.push_back("Invalid stack start symbol: " + stack_start_symbol);
            return;
        }
        _stack_start_symbol = stack_start_symbol;
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing stack start symbol");
        return;
    }
}

void PDASimulator::parse_accept_states(std::string &line) {
    std::regex acceptStatesLineRegex(R"(#F = \{([^}]+)\})");
    std::smatch match;
    if (std::regex_match(line, match, acceptStatesLineRegex)) {
        std::string accept_states = match[1];
        std::stringstream ss(accept_states);
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
    } else {
        _error = Error::SyntaxError;
        _error_logs.push_back("Syntax error when parsing accept states");
        return;
    }
}

void PDASimulator::parse_transitions(std::string &line) {
    std::stringstream ss(line);
    std::string from_state, input_char, stack_top, to_state, stack_push;

    ss >> from_state >> input_char >> stack_top >> to_state >> stack_push;

    if (_states.find(State(from_state)) == _states.end())
        _error_logs.push_back("Invalid from state name: " + from_state);

    if (input_char != "_" && !_input_alphabet.contains(input_char))
        _error_logs.push_back("Invalid input character: " + input_char);

    if (!_stack_alphabet.contains(stack_top)) // The stack top can not be empty
        _error_logs.push_back("Invalid stack top symbol: " + stack_top);

    if (_states.find(State(to_state)) == _states.end())
        _error_logs.push_back("Invalid to state name: " + to_state);

    for (char c : stack_push) {
        if (stack_push != "_" && !_stack_alphabet.contains(std::string(1, c))) {
            _error_logs.push_back("Invalid stack push symbol: " + stack_push);
            break;
        }
    }

    if (_error_logs.size() > 0) {
        _error = Error::SyntaxError;
        return;
    }

    _transitions[std::make_tuple(State(from_state), input_char[0], stack_top[0])] =
        std::make_tuple(State(to_state), stack_push);
}

} // namespace fla
