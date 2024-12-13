#include <fla/tm.h>

#include <iostream>
#include <string>

namespace fla {

void TMSimulator::parse(const std::string &filepath) {
    std::clog << "Parsing PDM from file: " << filepath << std::endl;
    // TODO: Implement TM parse
}

void TMSimulator::run(const std::string &input) {
    std::clog << "Running PDM with input: " << input << std::endl;
    // TODO: Implement TM run
}

} // namespace fla
