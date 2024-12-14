#pragma once

#include <algorithm>
#include <string>

namespace fla {

static inline std::string &ltrim(std::string &s) {
    s.erase(s.begin(),
            std::find_if(s.begin(), s.end(), [](unsigned char ch) { return !std::isspace(ch); }));
    return s;
}

static inline std::string &rtrim(std::string &s) {
    s.erase(std::find_if(s.rbegin(), s.rend(), [](unsigned char ch) { return !std::isspace(ch); })
                .base(),
            s.end());
    return s;
}

// Function to trim whitespace from both ends of a string
static inline std::string &strip(std::string &s) { return ltrim(rtrim(s)); }

static inline void ignore_after(std::string &s, char ch) {
    size_t pos = s.find(ch);
    if (pos != std::string::npos) {
        s.erase(pos);
    }
}

} // namespace fla
