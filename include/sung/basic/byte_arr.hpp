#pragma once

#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "sung/basic/bytes.hpp"
#include "sung/basic/optional.hpp"


namespace sung {

    // Little endian
    class BytesBuilder {

    public:
        size_t size() const noexcept;
        byte8* data() noexcept;
        const byte8* data() const noexcept;
        const std::vector<byte8>& vector() const noexcept;

        void clear() noexcept { data_.clear(); }

        // Once you call this, it is safe to call this->clear() before calling
        // any other methods
        std::vector<byte8>&& release() noexcept { return std::move(data_); }

        void enlarge(size_t size);

        std::pair<size_t, size_t> add_arr(const void* src, size_t size);

        template <typename T>
        std::pair<size_t, size_t> add_val_arr(const T* src, size_t size) {
            if (is_little_endian()) {
                return this->add_arr(src, sizeof(T) * size);
            } else {
                const auto start_pos = data_.size();
                for (size_t i = 0; i < size; ++i) {
                    this->add_val(src[i]);
                }
                return { start_pos, sizeof(T) * size };
            }
        }

        template <typename T>
        std::pair<size_t, size_t> add_std_arr(const T& src) {
            return this->add_val_arr(src.data(), src.size());
        }

        // Add a null-terminated string
        void add_nt_str(const char* const str);

        template <typename T>
        void add_val(T val) {
            this->enlarge(sizeof(T));
            decompose_to_le(
                val, data_.data() + data_.size() - sizeof(T), sizeof(T)
            );
        }

        void add_int8(int8_t val) { this->add_val(val); }
        void add_int16(int16_t val) { this->add_val(val); }
        void add_int32(int32_t val) { this->add_val(val); }
        void add_int64(int64_t val) { this->add_val(val); }

        void add_uint8(uint8_t val) { this->add_val(val); }
        void add_uint16(uint16_t val) { this->add_val(val); }
        void add_uint32(uint32_t val) { this->add_val(val); }
        void add_uint64(uint64_t val) { this->add_val(val); }

        void add_float32(float val) { this->add_val(val); }
        void add_float64(double val) { this->add_val(val); }

    private:
        std::vector<byte8> data_;
    };


    // Little endian
    class BytesReader {

    public:
        BytesReader(const byte8* data, size_t size);

        size_t size() const noexcept { return size_; }
        size_t remaining() const noexcept { return size_ - pos_; }
        const byte8* data() const noexcept { return data_; }
        const byte8* head() const noexcept { return data_ + pos_; }

        bool is_eof() const noexcept;
        bool has_overflow() const noexcept;

        void advance(size_t size) { pos_ += size; }

        std::string read_nt_str();

        template <typename T>
        sung::Optional<T> read_val() {
            if (pos_ + sizeof(T) > size_)
                return sung::nullopt;

            const auto out = assemble_le_data<T>(data_ + pos_);
            pos_ += sizeof(T);
            return out;
        }

        bool read_raw_arr(void* dst, size_t count);

        template <typename T>
        bool read_val_arr(T* dst, size_t count) {
            const auto read_size = sizeof(T) * count;
            if (pos_ + read_size > size_)
                return false;

            if (is_little_endian()) {
                std::memcpy(dst, data_ + pos_, read_size);
                pos_ += read_size;
            } else {
                for (size_t i = 0; i < count; ++i) {
                    dst[i] = assemble_le_data<T>(data_ + pos_);
                    pos_ += sizeof(T);
                }
            }

            return true;
        }

        template <typename T, size_t TSize>
        sung::Optional<std::array<T, TSize>> read_val_arr() {
            std::array<T, TSize> out;
            if (this->read_val_arr<T>(out.data(), TSize))
                return out;
            return sung::nullopt;
        }

        sung::Optional<int8_t> read_bool() { return read_val<int8_t>() != 0; }

        sung::Optional<int32_t> read_int32() { return read_val<int32_t>(); }
        sung::Optional<int64_t> read_int64() { return read_val<int64_t>(); }
        sung::Optional<uint32_t> read_uint32() { return read_val<uint32_t>(); }
        sung::Optional<uint64_t> read_uint64() { return read_val<uint64_t>(); }

        sung::Optional<float> read_float32() { return read_val<float>(); }
        sung::Optional<double> read_float64() { return read_val<double>(); }

        bool read_int32_arr(int32_t* dst, size_t count) {
            return read_val_arr<int32_t>(dst, count);
        }
        bool read_float32_arr(float* dst, size_t count) {
            return read_val_arr<float>(dst, count);
        }

        template <size_t TSize>
        sung::Optional<std::array<float, TSize>> read_float32_arr() {
            std::array<float, TSize> out;
            if (this->read_val_arr(out.data(), TSize))
                return out;
            return sung::nullopt;
        }

    private:
        const byte8* const data_;
        const size_t size_;
        size_t pos_ = 0;
    };

}  // namespace sung
