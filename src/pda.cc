#include <cstddef>
#include <cstdlib>
#include <fla/pda.h>

#include <iomanip>
#include <iostream>
#include <string>

namespace fla {

void PDASimulator::run(const std::string &input) {

    { // check input
        for (size_t i = 0; i < input.size(); ++i) {
            std::string tmp = std::string(1, input[i]);

            if (!_input_alphabet.contains(tmp)) {
                _error = Error::InputError;
                _error_logs.push_back("Input: " + input);
                _error_logs.push_back("==================== ERR ====================");
                _error_logs.push_back("error: '" + tmp +
                                      "' was not declared in the set of input symbols");
                _error_logs.push_back("Input: " + input);
                _error_logs.push_back(std::string(7 + i, ' ') + std::string(1, '^'));
                _error_logs.push_back("==================== END ====================");
                error_handler();
            }
        }
    }

    { // init PDA
        for (size_t i = 0; i < input.size(); ++i)
            _input.push(input[i]);
        _stack.push_back(_stack_start_symbol[0]);
        _current_state = _start_state;
    }

    if (_verbose) {
        std::cout << "Input: " + input << std::endl;
        std::cout << "==================== RUN ====================" << std::endl;
    }

    while (true) {
        if (_verbose)
            print_state();

        if (_input.empty() && _accept_states.find(_current_state) != _accept_states.end()) {
            _accept = true;
            halt();
        }

        if (_stack.empty())
            halt();

        step();
        _counter++;
    }
}

void PDASimulator::step() {
    char stack_top, input_char;
    stack_top = _stack[_stack.size() - 1];
    _stack.erase(_stack.end() - 1);
    if (!_input.empty()) {
        input_char = _input.front();
        _input.pop();
    } else
        input_char = '_';

    Condition current_condition = std::make_tuple(_current_state, input_char, stack_top);
    auto it = _transitions.find(current_condition);
    if (it == _transitions.end()) {
        halt();
    } else {
        _current_state = std::get<0>(it->second);
        std::string push_chars = std::get<1>(it->second);
        if (push_chars != "_") {
            std::reverse(push_chars.begin(), push_chars.end());
            for (char c : push_chars) {
                _stack.push_back(c);
            }
        }
    }
}

void PDASimulator::halt() {
    if (_verbose) {
        std::clog << "Halted after " << _counter << " steps." << std::endl;
        std::cout << "Result: " << std::boolalpha << _accept << std::endl;
        std::cout << "==================== END ====================" << std::endl;
    } else
        std::cout << std::boolalpha << _accept << std::endl;

    exit(EXIT_SUCCESS);
}

void PDASimulator::error_handler() {
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
            std::cerr << "unknown error" << std::endl;
            break;
        }
    } else {
        std::clog << "Error logs:" << std::endl;
        for (const std::string &error : _error_logs) {
            std::cerr << error << std::endl;
        }
    }
    exit(EXIT_FAILURE);
}

void PDASimulator::print_stack() {
    std::cout << std::left << std::setw(7) << "Index" << ": ";
    for (size_t i = 0; i < _stack.size(); i++)
        std::cout << i << ' ';
    std::cout << std::endl;

    std::cout << std::left << std::setw(7) << "Stack" << ": ";
    for (size_t i = 0; i < _stack.size(); i++)
        std::cout << std::left << std::setw(static_cast<int>(std::to_string(i).size())) << _stack[i]
                  << ' ';
    std::cout << std::endl;

    std::cout << std::left << std::setw(7) << "Head" << ": ";
    for (size_t i = 0; i < _stack.size(); i++)
        std::cout << std::left << std::setw(static_cast<int>(std::to_string(i).size()))
                  << (i == _stack.size() - 1 ? "^"
                                             : std::string(std::to_string(i).size() + 1, ' '));
    std::cout << std::endl;
}

void PDASimulator::print_state() {
    std::cout << std::left << std::setw(7) << "Step" << ": " << _counter << std::endl;
    std::cout << std::left << std::setw(7) << "State" << ": " << _current_state.name() << std::endl;
    if (_stack.empty()) {
        std::cout << std::left << std::setw(7) << "Index" << ": 0" << std::endl;
        std::cout << std::left << std::setw(7) << "Stack" << ": _" << std::endl;
        std::cout << std::left << std::setw(7) << "Head" << ": ^" << std::endl;
    } else {
        print_stack();
    }
    std::cout << "---------------------------------------------" << std::endl;
}

} // namespace fla
