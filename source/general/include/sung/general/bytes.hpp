#pragma once

#include <cstring>
#include <vector>


namespace sung {

    bool is_big_endian() noexcept;
    bool is_little_endian() noexcept;


    template <typename T>
    T assemble_from_big_endian(const uint8_t* buf) {
        if (is_big_endian()) {
            return *reinterpret_cast<const T*>(buf);
        } else {
            uint8_t tmp[sizeof(T)];
            for (size_t i = 0; i < sizeof(T); ++i)
                tmp[i] = buf[sizeof(T) - i - 1];
            return *reinterpret_cast<const T*>(tmp);
        }
    }

    template <typename T>
    T assemble_from_little_endian(const uint8_t* buf) {
        if (is_little_endian()) {
            return *reinterpret_cast<const T*>(buf);
        } else {
            uint8_t tmp[sizeof(T)];
            for (size_t i = 0; i < sizeof(T); ++i)
                tmp[i] = buf[sizeof(T) - i - 1];
            return *reinterpret_cast<const T*>(tmp);
        }
    }

    template <typename T>
    bool decompose_to_big_endian(T src, uint8_t* dst, size_t dst_size) {
        if (dst_size < sizeof(T))
            return false;

        if (is_big_endian()) {
            std::memcpy(dst, &src, sizeof(T));
        } else {
            auto src_ptr = reinterpret_cast<const uint8_t*>(&src);
            for (size_t i = 0; i < sizeof(T); ++i)
                dst[i] = src_ptr[sizeof(T) - i - 1];
        }

        return true;
    }

    template <typename T>
    bool decompose_to_little_endian(T src, uint8_t* dst, size_t dst_size) {
        if (dst_size < sizeof(T))
            return false;

        if (is_little_endian()) {
            std::memcpy(dst, &src, sizeof(T));
        } else {
            auto src_ptr = reinterpret_cast<const uint8_t*>(&src);
            for (size_t i = 0; i < sizeof(T); ++i)
                dst[i] = src_ptr[sizeof(T) - i - 1];
        }

        return true;
    }


    class BytesBuilder {

    public:
        size_t size() const noexcept;
        uint8_t* data() noexcept;
        const uint8_t* data() const noexcept;
        const std::vector<uint8_t>& vector() const noexcept;

        void enlarge(size_t size);

        std::pair<size_t, size_t> add_arr(const void* src, size_t size);

        template <typename T>
        std::pair<size_t, size_t> add_arr(const T& src) {
            return this->add_arr(src.data(), src.size());
        }

        // Add a null-terminated string
        void add_nt_str(const char* const str);

        void add_int64(int64_t val);
        void add_uint64(uint64_t val);

    private:
        template <typename T>
        void add_val(T val) {
            this->enlarge(sizeof(T));
            decompose_to_little_endian(
                val, data_.data() + data_.size() - sizeof(T), sizeof(T)
            );
        }

        std::vector<uint8_t> data_;
    };

}  // namespace sung
