/**
 * @file app/main.cc
 * @author Han Jiarui
 * @brief The main file for the fla program.
 */

#include <fla/pda.h>
#include <fla/simulator.h>
#include <fla/tm.h>

#include <iostream>
#include <map>
#include <memory>
#include <string>
#include <vector>

void print_usage() {
    std::cerr << "Usage:\tfla [-h|--help]\n";
    std::cerr << "      \tfla [-v|--verbose] <pda> <input>\n";
    std::cerr << "      \tfla [-v|--verbose] <tm> <input>\n";
}

int main(int argc, const char *argv[]) {
    std::clog.setstate(std::ios_base::failbit);

    std::map<std::string, bool> options = {
        {"-h", false},
        {"--help", false},
        {"-v", false},
        {"--verbose", false},
    };

    std::vector<std::string> args;
    for (int i = 1; i < argc; ++i) {
        std::string arg = argv[i];
        if (arg[0] == '-') { // Check if arg is an option
            if (options.find(arg) != options.end()) {
                options[arg] = true;
            } else {
                std::cerr << "Unknown option: " << arg << std::endl;
                print_usage();
                return EXIT_FAILURE;
            }
        } else {
            args.push_back(arg);
        }
    }

    if (options["-h"] || options["--help"]) {
        print_usage();
        return EXIT_SUCCESS;
    }

    if (args.size() != 2) {
        print_usage();
        return EXIT_FAILURE;
    }

    std::string filepath = args[0];
    std::string input = args[1];

    size_t dot_pos = filepath.rfind(".");
    std::string extension{};
    if (dot_pos != std::string::npos) {
        extension = filepath.substr(dot_pos + 1);
    }

    std::unique_ptr<fla::Simulator> simulator{};
    if (extension == "pda") {
        simulator = std::make_unique<fla::PDASimulator>();
    } else if (extension == "tm") {
        simulator = std::make_unique<fla::TMSimulator>();
    } else {
        std::cerr << "Unknown file type: " << filepath << std::endl;
        std::cerr << "The file format must be '*.pda' or '*.tm'" << std::endl;
        return EXIT_FAILURE;
    }

    try {
        simulator->set_verbose(options["-v"] || options["--verbose"]);
        simulator->parse(filepath);
        simulator->run(input);
    } catch (const fla::Error &e) {
        return EXIT_FAILURE;
    }

    return EXIT_SUCCESS;
}
