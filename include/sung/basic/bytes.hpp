#pragma once

#include <array>
#include <cstdint>
#include <cstring>


namespace sung {

    using byte8 = uint8_t;


    bool is_big_endian() noexcept;
    bool is_little_endian() noexcept;


    template <typename T>
    T flip_byte_order(T value) {
        T result = 0;
        for (std::size_t i = 0; i < sizeof(T); ++i) {
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
    T assemble_be_data(const byte8* buf) {
        if (is_big_endian()) {
            return *reinterpret_cast<const T*>(buf);
        } else {
            byte8 tmp[sizeof(T)];
            for (std::size_t i = 0; i < sizeof(T); ++i)
                tmp[i] = buf[sizeof(T) - i - 1];
            return *reinterpret_cast<const T*>(tmp);
        }
    }

    template <typename T>
    T assemble_le_data(const byte8* buf) {
        if (is_little_endian()) {
            return *reinterpret_cast<const T*>(buf);
        } else {
            byte8 tmp[sizeof(T)];
            for (std::size_t i = 0; i < sizeof(T); ++i)
                tmp[i] = buf[sizeof(T) - i - 1];
            return *reinterpret_cast<const T*>(tmp);
        }
    }

    template <typename T>
    bool decompose_to_be(T src, byte8* dst, std::size_t dst_size) {
        if (dst_size < sizeof(T))
            return false;

        if (is_big_endian()) {
            std::memcpy(dst, &src, sizeof(T));
        } else {
            auto src_ptr = reinterpret_cast<const byte8*>(&src);
            for (size_t i = 0; i < sizeof(T); ++i)
                dst[i] = src_ptr[sizeof(T) - i - 1];
        }

        return true;
    }

    template <typename T>
    bool decompose_to_le(T src, byte8* dst, size_t dst_size) {
        if (dst_size < sizeof(T))
            return false;

        if (is_little_endian()) {
            std::memcpy(dst, &src, sizeof(T));
        } else {
            auto src_ptr = reinterpret_cast<const byte8*>(&src);
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

        const byte8* data() const { return data_.data(); }
        size_t size() const { return data_.size(); }

    private:
        std::array<byte8, sizeof(T)> data_ = { 0 };
    };


    template <typename T>
    class LEValue {

    public:
        LEValue() = default;

        LEValue(const T& value) {
            decompose_to_le(value, data_.data(), data_.size());
        }

        T get() const { return assemble_le_data<T>(data_.data()); }

        void set(T value) {
            decompose_to_le(value, data_.data(), data_.size());
        }

        const byte8* data() const { return data_.data(); }
        size_t size() const { return data_.size(); }

    private:
        std::array<byte8, sizeof(T)> data_ = { 0 };
    };

}  // namespace sung
