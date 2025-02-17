#pragma once

#include <array>

namespace sung {

    bool is_big_endian() noexcept;
    bool is_little_endian() noexcept;


    template <typename T>
    T flip_byte_order(T value) {
        T result = 0;
        for (size_t i = 0; i < sizeof(T); ++i) {
            result <<= 8;
            result |= value & 0xFF;
            value >>= 8;
        }
        return result;
    }

    template <>
    inline float flip_byte_order(float value) {
        uint32_t result = flip_byte_order(*reinterpret_cast<uint32_t*>(&value));
        return *reinterpret_cast<float*>(&result);
    }

    template <>
    inline double flip_byte_order(double value) {
        uint64_t result = flip_byte_order(*reinterpret_cast<uint64_t*>(&value));
        return *reinterpret_cast<double*>(&result);
    }


    template <typename T>
    T assemble_be_data(const uint8_t* buf) {
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
    T assemble_le_data(const uint8_t* buf) {
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
    bool decompose_to_be(T src, uint8_t* dst, size_t dst_size) {
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
    bool decompose_to_le(T src, uint8_t* dst, size_t dst_size) {
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


    template <typename T>
    class BEValue {

    public:
        BEValue() = default;

        BEValue(const T& value) {
            decompose_to_be(value, data_.data(), data_.size());
        }

        T get() const { return assemble_be_data<T>(data_.data()); }

        void set(T value) {
            decompose_to_be(value, data_.data(), data_.size());
        }

        const uint8_t* data() const { return data_.data(); }
        size_t size() const { return data_.size(); }

    private:
        std::array<uint8_t, sizeof(T)> data_ = { 0 };
    };

}  // namespace sung
