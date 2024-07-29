#include "sung/general/stringtool.hpp"

#include <iomanip>
#include <regex>
#include <sstream>


namespace {

    std::string format_float(double x, std::streamsize precision = 2) {
        std::stringstream ss;
        ss << std::fixed << std::setprecision(precision) << x;
        return ss.str();
    }

}  // namespace


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

    std::string format_bytes(size_t bytes) {
        if (bytes < 1024) {
            return ::format_float(static_cast<double>(bytes)) + " B";
        } else if (bytes < 1024 * 1024) {
            return ::format_float(bytes / 1024.0) + " KiB";
        } else if (bytes < 1024 * 1024 * 1024) {
            return ::format_float(bytes / (1024.0 * 1024.0)) + " MiB";
        } else {
            return ::format_float(bytes / (1024.0 * 1024.0 * 1024.0)) + " GiB";
        }
    }

}  // namespace sung
