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

    std::vector<std::string> split(
        const std::string& str, const std::string& delim
    ) {
        std::vector<std::string> tokens;
        size_t start = 0, end = 0;
        while ((end = str.find(delim, start)) != std::string::npos) {
            tokens.push_back(str.substr(start, end - start));
            start = end + delim.size();
        }
        tokens.push_back(str.substr(start));
        return tokens;
    }

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

    std::string serialize_str(const std::string& str) {
        std::string result = "\"";
        for (const auto& c : str) {
            if (c == '\n') {
                result += "\\n";
            } else if (c == '\r') {
                result += "\\r";
            } else if (c == '\t') {
                result += "\\t";
            } else if (c == '\\') {
                result += "\\\\";
            } else if (c == '\"') {
                result += "\\\"";
            } else {
                result += c;
            }
        }
        result += "\"";
        return result;
    }

    std::string deserialize_str(const std::string& str) {
        std::string result;
        for (size_t i = 1; i < str.size() - 1; ++i) {
            if (str[i] == '\\') {
                if (str[i + 1] == 'n') {
                    result += '\n';
                } else if (str[i + 1] == 'r') {
                    result += '\r';
                } else if (str[i + 1] == 't') {
                    result += '\t';
                } else if (str[i + 1] == '\\') {
                    result += '\\';
                } else if (str[i + 1] == '\"') {
                    result += '\"';
                } else {
                    result += str[i + 1];
                }
                ++i;
            } else {
                result += str[i];
            }
        }
        return result;
    }

    sung::Optional<int> str2int(const std::string& str) {
        try {
            return std::stoi(str);
        } catch (const std::exception&) {
            return sung::nullopt;
        }
    }

}  // namespace sung
