#pragma once

#include <iostream>
#include <string>
#include <vector>
#include <array>

namespace C78Test {

    std::vector<std::string> split(const std::string& s, std::string seperator) {
        std::vector<std::string> output;
        std::string::size_type prev_pos = 0, pos = 0;

        while ((pos = s.find(seperator, pos)) != std::string::npos) {
            output.push_back(s.substr(prev_pos, pos - prev_pos));
            prev_pos = ++pos;
        }

        output.push_back(s.substr(prev_pos, pos - prev_pos)); // Last word
        return output;
    }

    int main() {
        std::array<std::string, 4> faces = {
            "v0 v1 v2",
            "v0/t0 v1/t1 v2/t2",
            "v0//n0 v1//n1 v2//n2",
            "v0/t0/n0 v1/t1/n1 v2/t2/n2",
        };

        for (std::string source : faces) {
            for (std::string s : split(source, " ")) {
                std::cout << s << std::endl;
                for (std::string i : split(s, "/")) {
                    if (!i.empty())
                        std::cout << i << std::endl;
                    else
                        std::cout << '_' << std::endl;

                }
            }
            std::cout << "--------------------------" << std::endl;
        }

        return 0;
    }

}