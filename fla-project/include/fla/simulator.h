#pragma once

#include <set>
#include <string>
#include <vector>

namespace fla {

enum class Error {
    None,

    SyntaxError,

    InputError,

    OtherError,
};

class State {
  public:
    State() = default;
    explicit State(const std::string &name) : _name(name) {}
    ~State() = default;

    void set_name(const std::string &name) { _name = name; };

    static bool is_valid(std::string name);

    bool operator==(const State &rhs) const { return _name == rhs._name; }
    bool operator<(const State &rhs) const { return _name < rhs._name; }

    const std::string &name() const { return _name; };

    bool empty() const { return _name.empty(); };

  private:
    std::string _name{};
};

class Alphabet {
  public:
    Alphabet() = default;
    ~Alphabet() = default;

    static bool is_valid(std::string s);

    void add(std::string s) { _alphabet.insert(s); };
    bool contains(const std::string &s) const { return _alphabet.find(s) != _alphabet.end(); };
    bool empty() const { return _alphabet.empty(); };

  private:
    std::set<std::string> _alphabet{};
};

class Simulator {
  public:
    Simulator() = default;
    virtual ~Simulator() = default;

    virtual void parse(const std::string &filepath) = 0;
    virtual void run(const std::string &input) = 0;

    void set_verbose(bool verbose);

    friend class SimulatorTest;

  protected:
    virtual void halt() = 0;
    virtual void error_handler() = 0;

    bool _verbose = false;

    std::vector<std::string> _error_logs{};
    Error _error = Error::None;

    bool _halted = false;
};

} // namespace fla
