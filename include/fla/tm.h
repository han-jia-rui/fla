#pragma once

#include <fla/simulator.h>
#include <fla/util.h>

#include <cassert>
#include <deque>
#include <map>
#include <string>
#include <tuple>
#include <vector>

namespace fla {

class SymbolSeq {
  public:
    SymbolSeq() = default;
    SymbolSeq(const std::string &s) : _symbol_seq(s) {};
    ~SymbolSeq() = default;

    size_t size() const { return _symbol_seq.size(); };

    std::string to_string() const { return _symbol_seq; };

    char operator[](size_t index) const { return _symbol_seq[index]; }

    bool operator==(const SymbolSeq &rhs) const;

    bool operator<(const SymbolSeq &rhs) const { return _symbol_seq < rhs.to_string(); }

  private:
    std::string _symbol_seq;
};

class Tape {
  public:
    Tape() = default;
    ~Tape() = default;

    void init(const std::string &input) { _tape.assign(input.begin(), input.end()); };
    char read() const {
        assert(_head >= 0 && _head < static_cast<int>(_tape.size()));
        return _tape[static_cast<size_t>(_head)];
    };
    void step(char symbol, char direction);

    std::string to_string();
    void print(size_t idx);

  private:
    void expand();
    void shrink();

    int _offset = 0;
    int _head = 0;
    std::deque<char> _tape{'_'};
};

class TMSimulator : public Simulator {
  public:
    TMSimulator() = default;
    ~TMSimulator() = default;

    void parse(const std::string &filepath) override;
    void run(const std::string &input) override;

  private:
    // Parsing
    void parse_states(const std::string &line);
    void parse_input_alphabet(const std::string &line);
    void parse_stack_alphabet(const std::string &line);
    void parse_start_state(const std::string &line);
    void parse_empty_symbol(const std::string &line);
    void parse_accept_states(const std::string &line);
    void parse_tape_number(const std::string &line);
    void parse_transitions(const std::string &line);

    // Running
    void step();
    void halt() override;

    // Logging
    void print_state();
    void error_handler() override;

    // Configuration
    using Condition = std::tuple<State, SymbolSeq>;
    using Action = std::tuple<SymbolSeq, std::string, State>;
    std::set<State> _states{};
    Alphabet _input_alphabet{};
    Alphabet _tape_alphabet{};
    State _start_state{};
    std::string _empty_symbol{};
    std::set<State> _accept_states{};
    size_t _tape_number = 0;
    std::map<Condition, Action> _transitions{};

    // Run-time data
    size_t _counter = 0;
    std::vector<Tape> _tapes{};
    State _current_state{};
    bool _accept = false;
};

} // namespace fla
