#include "sung/general/stringtool.hpp"

#include <iomanip>
#include <regex>
#include <sstream>


namespace {

    std::string format_float(double x, std::streamsize precision = 2) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << x;

        // Remove trailing zeros
        auto str = ss.str();
        str.erase(str.find_last_not_of('0') + 1, std::string::npos);
        if (str.back() == '.') {
            str.pop_back();
        }

        return str;
    }

}  // namespace


namespace sung {

    std::string lstrip(std::string str, const std::string& prefix) {
        if (str.find(prefix) == 0) {
            str.erase(0, prefix.size());
        }
        return str;
    }

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

    std::string format_bytes(size_t bytes) {
        if (bytes < 1024) {
            return std::to_string(bytes) + " B";
        } else if (bytes < 1024 * 1024) {
            return ::format_float(bytes / 1024.0) + " KiB";
        } else if (bytes < 1024 * 1024 * 1024) {
            return ::format_float(bytes / (1024.0 * 1024.0)) + " MiB";
        } else {
            return ::format_float(bytes / (1024.0 * 1024.0 * 1024.0)) + " GiB";
        }
    }

}  // namespace sung
