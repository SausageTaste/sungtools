#include <iostream>
#include <string>

#include "sung/general/expected.hpp"
#include "sung/general/os_detect.hpp"


namespace {

    sung::Expected<double, std::string> divide(double a, double b) {
        sung::Expected<double, std::string> output;
        if (b == 0.0)
            output = sung::unexpected<std::string>("Division by zero");
        else
            output = a / b;
        return output;
    }

}  // namespace


int main() {
    const auto result0 = ::divide(1.0, 0.0);
    if (result0.has_value())
        std::cout << "Result: " << result0.value() << std::endl;
    else
        std::cout << "Error: " << result0.error() << std::endl;

    const auto result1 = ::divide(1.0, 2.0);
    if (result1.has_value())
        std::cout << "Result: " << result1.value() << std::endl;
    else
        std::cout << "Error: " << result1.error() << std::endl;

    return 0;
}
