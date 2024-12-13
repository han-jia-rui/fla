#pragma once

#include <fla/simulator.h>

namespace fla {

class TMSimulator : public Simulator {
  public:
    TMSimulator() = default;
    ~TMSimulator() = default;
    void parse(const std::string &filepath) override;
    void run(const std::string &input) override;
};

} // namespace fla
