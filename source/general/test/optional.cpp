#include <iostream>
#include <string>

#include "sung/general/optional.hpp"


namespace {

    void try_print(const sung::Optional<std::string>& opt) {
        if (opt)
            std::cout << "opt has value: " << *opt << std::endl;
        else
            std::cout << "opt has no value" << std::endl;
    }

}  // namespace


int main() {
    sung::Optional<std::string> opt_int;
    ::try_print(opt_int);

    opt_int = "Hello, World!";
    ::try_print(opt_int);

    opt_int = sung::nullopt;
    ::try_print(opt_int);

    return 0;
}
