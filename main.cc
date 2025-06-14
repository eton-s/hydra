#include "view.h"
#include <memory>
#include <string>
#include <iostream>

int main(int argc, char **argv) {
    View v{std::cin, std::cout};
    if (argc > 1 && std::string(argv[1]) == "-testing") {
        // testing mode on
        v.toggleTesting();
    }
    if (argc > 1 && std::string(argv[1]) == "-cheats") {
        // cheats are on
        v.toggleTesting();
        v.toggleCheats();
    }
    try {
        v.playGame();
    } catch (std::logic_error & e) {
        // thrown if EOF or input fail bit occurs
    }
}
