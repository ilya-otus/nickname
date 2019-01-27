#include <iostream>
#include "src/radix_trie.h"
#include <string>

using namespace std::string_literals;
int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    Patricia::RadixTrie<std::string, int> t;
    for (std::string line; getline(std::cin, line);) {
        t.insert({line, 0});
    }
    for (auto it = t.begin(); it != t.end(); ++it) {
        std::cout << it->first << " ";
        std::cout << it->first.substr(0, it->first.size() - it.node()->parent()->key().size() + 1) << std::endl;
    }
    t.dump();
    return 0;
}
