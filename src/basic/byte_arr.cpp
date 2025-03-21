#include "sung/basic/byte_arr.hpp"


// BytesBuilder
namespace sung {

    size_t BytesBuilder::size() const noexcept { return data_.size(); }

    byte8* BytesBuilder::data() noexcept { return data_.data(); }

    const byte8* BytesBuilder::data() const noexcept { return data_.data(); }

    const std::vector<byte8>& BytesBuilder::vector() const noexcept {
        return data_;
    }

    void BytesBuilder::reserve(size_t size) { data_.reserve(size); }

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

    BytesReader::BytesReader(const byte8* data, size_t size)
        : data_{ data }, size_{ size } {}

    bool BytesReader::is_eof() const noexcept { return pos_ == size_; }

    bool BytesReader::has_overflow() const noexcept { return pos_ > size_; }

    std::string BytesReader::read_nt_str() {
        std::string out = reinterpret_cast<const char*>(data_ + pos_);
        pos_ += out.size() + 1;
        return out;
    }

    bool BytesReader::read_raw_arr(void* dst, size_t count) {
        if (pos_ + count > size_)
            return false;

        std::memcpy(dst, data_ + pos_, count);
        pos_ += count;
        return true;
    }

}  // namespace sung
