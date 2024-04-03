#include "sung/general/stringtool.hpp"

#include <regex>


namespace sung {

    std::string slugify(std::string str) {
        str = std::regex_replace(str, std::regex("[^\\w\\s$*_+~.()\'\"-]"), "");
        str = std::regex_replace(str, std::regex("^\\s+|\\s+$"), "");
        str = std::regex_replace(str, std::regex("[-\\s]+"), "-");
        str = std::regex_replace(str, std::regex("#-$"), "");
        return str;
    }

}  // namespace sung
