#include "sung/general/stringtool.hpp"

#include <regex>


namespace sung {

    std::string slugify(std::string str, const char* r) {
        // Replace invalid characters with replacer
        str = std::regex_replace(str, std::regex("[^\\w\\s$*_+~.()\'\"-]"), r);

        // Trim whitespaces
        str = std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");

        // Merge multiple whitespaces into one
        str = std::regex_replace(str, std::regex("[\\s]+"), " ");

        // Replace spaces with replacer
        str = std::regex_replace(str, std::regex("#-$"), r);

        return str;
    }

}  // namespace sung
