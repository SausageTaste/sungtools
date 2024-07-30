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

    bool is_little_endian() noexcept { return !is_big_endian(); }

}  // namespace sung


// BytesBuilder
namespace sung {

    size_t BytesBuilder::size() const noexcept { return data_.size(); }

    uint8_t* BytesBuilder::data() noexcept { return data_.data(); }

    const uint8_t* BytesBuilder::data() const noexcept { return data_.data(); }

    const std::vector<uint8_t>& BytesBuilder::vector() const noexcept {
        return data_;
    }

    void BytesBuilder::enlarge(size_t size) {
        data_.resize(data_.size() + size);
    }

    std::pair<size_t, size_t> BytesBuilder::add_arr(
        const void* src, size_t size
    ) {
        const auto old_size = data_.size();
        data_.resize(old_size + size);
        std::memcpy(data_.data() + old_size, src, size);
        return { old_size, size };
    }

    // Add a null-terminated string
    void BytesBuilder::add_nt_str(const char* const str) {
        const auto len = std::strlen(str);
        this->add_arr(str, len);
        this->add_val('\0');
    }

}  // namespace sung


// BytesReader
namespace sung {

    BytesReader::BytesReader(const uint8_t* data, size_t size)
        : data_{ data }, size_{ size } {}

    bool BytesReader::is_eof() const noexcept { return pos_ == size_; }

    bool BytesReader::has_overflow() const noexcept { return pos_ > size_; }

    std::string BytesReader::read_nt_str() {
        std::string out = reinterpret_cast<const char*>(data_ + pos_);
        pos_ += out.size() + 1;
        return out;
    }

}  // namespace sung
