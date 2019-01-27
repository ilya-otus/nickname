#include <iostream>
#include "src/radix_trie.h"
#include <string>

using namespace std::string_literals;
int main(int argc, char **argv) {
    (void) argc;
    (void) argv;
    Patricia::RadixTrie<std::string, int> t;
    t.insert({"aleksey"s, 0});
    t.insert({"sasha"s, 0});
    t.insert({"aleks"s, 0});
    t.insert({"alek"s, 0});
    t.insert({"alesha"s, 0});
    t.insert({"maksim"s, 0});
    for (auto it = t.begin(); it != t.end(); ++it) {
        std::cout << it->first << " ";
        std::cout << it->first.substr(0, it->first.size() - it.node()->parent()->key().size() + 1) << std::endl;
    }
    t.dump();
    return 0;
}
