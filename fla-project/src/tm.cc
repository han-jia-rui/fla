#include <cstddef>
#include <fla/tm.h>

#include <iomanip>
#include <iostream>
#include <string>
#include <tuple>

namespace fla {

bool SymbolSeq::operator==(const SymbolSeq &rhs) const {
    if (_symbol_seq.size() != rhs.size())
        return false;
    for (size_t i = 0; i < rhs.size(); ++i) {
        if (_symbol_seq[i] == rhs[i])
            continue;
        if (_symbol_seq[i] == '*' and rhs[i] != '_')
            continue;
        if (_symbol_seq[i] != '_' and rhs[i] == '*')
            continue;
        return false;
    }
    return true;
}

void Tape::step(char symbol, char direction) {
    if (symbol != '*') {
        _tape[static_cast<size_t>(_head)] = symbol;
    }

    switch (direction) {
    case 'l':
        _head--;
        break;
    case 'r':
        _head++;
        break;
    default:
        break;
    }

    expand();
    shrink();
}

void Tape::expand() {
    if (_head < 0) {
        _tape.push_front('_');
        _offset--;
        _head = 0;
    }

    if (_head >= static_cast<int>(_tape.size())) {
        _tape.push_back('_');
    }
}

void Tape::shrink() {
    if (_head > 0 && _tape.front() == '_') {
        _tape.pop_front();
        _head--;
        _offset++;
    }

    if (_head < static_cast<int>(_tape.size()) - 1 && _tape.back() == '_') {
        _tape.pop_back();
    }
}

std::string Tape::to_string() {
    std::string tmp{};
    std::copy(_tape.begin(), _tape.end(), std::back_inserter(tmp));
    while (tmp.front() == '_') {
        tmp.erase(tmp.begin());
    }

    while (tmp.back() == '_') {
        tmp.pop_back();
    }

    return tmp;
}

void Tape::print(size_t idx, int width) const {
    std::cout << std::left << std::setw(width) << "Index" + std::to_string(idx) << ": ";
    for (size_t i = 0; i < _tape.size(); i++) {
        int num = std::abs(_offset + static_cast<int>(i));
        std::cout << num << ' ';
    }
    std::cout << std::endl;

    std::cout << std::left << std::setw(width) << "Tape" + std::to_string(idx) << ": ";
    for (size_t i = 0; i < _tape.size(); i++) {
        int num = std::abs(_offset + static_cast<int>(i));
        std::cout << std::left << std::setw(static_cast<int>(std::to_string(num).size()))
                  << _tape[i] << ' ';
    }
    std::cout << std::endl;

    std::cout << std::left << std::setw(width) << "Head" + std::to_string(idx) << ": ";
    for (int i = 0; i <= _head; i++) {
        int num = std::abs(_offset + i);
        std::cout << std::left << std::setw(static_cast<int>(std::to_string(num).size()))
                  << (i == _head ? "^" : " ") << " ";
    }
    std::cout << std::endl;
}

void TMSimulator::run(const std::string &input) {

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
                error_handler();
            }
        }
    }

    { // init TM
        _tapes.resize(_tape_number);
        _tapes[0].init(input);
        _current_state = _start_state;

        if (_verbose) {
            std::cout << "Input: " + input << std::endl;
            std::cout << "==================== RUN ====================" << std::endl;
        }
    }

    while (true) {
        if (_verbose)
            print_state();

        step();
        _counter++;
    }
}

void TMSimulator::step() {
    std::string cur_str(_tapes.size(), '\0');
    std::transform(_tapes.begin(), _tapes.end(), cur_str.begin(),
                   [](const auto &tape) { return tape.read(); });

    Condition current_condition = std::make_tuple(_current_state, SymbolSeq(cur_str));

    auto it = std::find_if(_transitions.begin(), _transitions.end(),
                           [&current_condition](const auto &transition) {
                               return transition.first == current_condition;
                           });

    if (it != _transitions.end()) {
        const SymbolSeq &new_str = std::get<0>(it->second);
        const std::string &direction = std::get<1>(it->second);
        const State &next_state = std::get<2>(it->second);

        _current_state = next_state;
        for (size_t i = 0; i < _tape_number; ++i) {
            _tapes[i].step(new_str[i], direction[i]);
        }
        return;
    }

    halt();
}

void TMSimulator::halt() noexcept {
    if (_verbose) {
        std::clog << "Halted after " << _counter << " steps." << std::endl;
        std::cout << "Result: " << _tapes[0].to_string() << std::endl;
        std::cout << "==================== END ====================" << std::endl;
    } else
        std::cout << _tapes[0].to_string() << std::endl;

    exit(EXIT_SUCCESS);
}

void TMSimulator::print_state() {
    int width = 5 + static_cast<int>(std::to_string(_tape_number).size()) + 1;
    std::cout << std::left << std::setw(width) << "Step" << ": " << _counter << std::endl;
    std::cout << std::left << std::setw(width) << "State" << ": " << _current_state.name()
              << std::endl;
    for (size_t i = 0; i < _tapes.size(); ++i)
        _tapes[i].print(i, width);
    std::cout << "---------------------------------------------" << std::endl;
}

} // namespace fla
