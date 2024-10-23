#pragma once

#include <string>
#include <vector>

#include "sung/general/optional.hpp"


namespace sung {

    std::vector<std::string> split(
        const std::string& str, const std::string& delim
    );

    std::string lstrip(std::string str, const std::string& prefix);

    std::string slugify(std::string str, const char* replacer = "");

    std::string format_bytes(size_t bytes);

    sung::Optional<int> str2int(const std::string& str);

}  // namespace sung
