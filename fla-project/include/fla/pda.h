#pragma once

#include <fla/simulator.h>
#include <fla/util.h>

#include <map>
#include <queue>
#include <tuple>
#include <vector>

namespace fla {

class PDASimulator final : public Simulator {
  public:
    PDASimulator() = default;
    ~PDASimulator() override = default;

    void parse(const std::string &filepath) override;
    void run(const std::string &input) override;

  private:
    // Parsing
    void parse_states(const std::string &line);
    void parse_input_alphabet(const std::string &line);
    void parse_stack_alphabet(const std::string &line);
    void parse_start_state(const std::string &line);
    void parse_stack_start_symbol(const std::string &line);
    void parse_accept_states(const std::string &line);
    void parse_transitions(const std::string &line);

    // Running
    void step();
    void halt() override;

    // Logging
    void print_state() const;
    void print_stack() const;
    void error_handler() override;

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
