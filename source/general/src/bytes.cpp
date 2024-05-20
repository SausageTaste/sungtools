#include "sung/general/bytes.hpp"


namespace sung {

    // https://stackoverflow.com/a/1001373/6819534
    bool is_big_endian() noexcept {
        union {
            uint32_t i;
            char c[4];
        } bint = { 0x01020304 };

        return bint.c[0] == 1;
    }

    bool is_little_endian() noexcept {
        return !is_big_endian();
    }

}  // namespace sung
