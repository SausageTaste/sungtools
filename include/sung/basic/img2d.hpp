#pragma once

#include <vector>

#include "sung/basic/bytes.hpp"


namespace sung {

    enum class ImageScalarType {
        uint8,
        float16,
        float32,
        float64,
    };

    size_t get_image_scalar_type_bytes(ImageScalarType type);


    class Image2D {

    public:
        void clear();
        void resize_data_to_fit();

        size_t scalar_bytes() const;
        size_t pixel_bytes() const;
        size_t row_bytes() const;

        byte8* pixel_ptr(size_t idx);
        byte8* pixel_ptr(size_t x, size_t y);
        const byte8* pixel_ptr(size_t idx) const;
        const byte8* pixel_ptr(size_t x, size_t y) const;

        template <typename T>
        T* pixel_ptr(size_t idx) {
            return reinterpret_cast<T*>(this->pixel_ptr(idx));
        }

        template <typename T>
        T* pixel_ptr(size_t x, size_t y) {
            return reinterpret_cast<T*>(this->pixel_ptr(x, y));
        }

        template <typename T>
        const T* pixel_ptr(size_t idx) const {
            return reinterpret_cast<const T*>(this->pixel_ptr(idx));
        }

        template <typename T>
        const T* pixel_ptr(size_t x, size_t y) const {
            return reinterpret_cast<const T*>(this->pixel_ptr(x, y));
        }

    public:
        std::vector<byte8> data_;
        ImageScalarType scalar_type_ = ImageScalarType::uint8;
        size_t channels_ = 0;
        size_t x_size_ = 0;
        size_t y_size_ = 0;
    };

}  // namespace sung
