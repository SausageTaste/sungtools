#pragma once

#include <string>


namespace sung {

    std::string slugify(std::string str, const char* replacer = "");

    std::string format_bytes(size_t bytes);

}  // namespace sung
