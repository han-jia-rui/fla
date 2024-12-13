#pragma once

#include <cstddef>
#include <fla/simulator.h>
#include <fla/util.h>

#include <map>
#include <queue>
#include <tuple>
#include <vector>

namespace fla {

class PDASimulator : public Simulator {
  public:
    PDASimulator() = default;
    ~PDASimulator() = default;

    void parse(const std::string &filepath) override;
    void run(const std::string &input) override;

  private:
    // Parsing
    void parse_states(std::string &line);
    void parse_input_alphabet(std::string &line);
    void parse_stack_alphabet(std::string &line);
    void parse_start_state(std::string &line);
    void parse_stack_start_symbol(std::string &line);
    void parse_accept_states(std::string &line);
    void parse_transitions(std::string &line);

    // Running
    void step();
    void halt();

    // Logging
    void print_state();
    void print_stack();
    void error_handler();

    // Configuration
    using Condition = std::tuple<State, char, char>;
    using Action = std::tuple<State, std::string>;
    std::set<State> _states{};
    Alphabet _input_alphabet{};
    Alphabet _stack_alphabet{};
    State _start_state{};
    std::string _stack_start_symbol{};
    std::set<State> _accept_states{};
    std::map<Condition, Action> _transitions{};

    // Run-time data
    size_t _counter = 0;
    std::queue<char> _input{};
    std::vector<char> _stack{};
    State _current_state{};
    bool _accept = false;
};

} // namespace fla
