#include "sung/basic/img2d.hpp"


namespace sung {

    void Image2D::clear() {
        data_.clear();
        x_size_ = 0;
        y_size_ = 0;
        channels_ = 0;
        scalar_type_ = ImageScalarType::uint8;
    }

    void Image2D::resize_data_to_fit() {
        const auto size = y_size_ * this->row_bytes();
        data_.resize(size);
    }

    size_t Image2D::scalar_bytes() const {
        return get_image_scalar_type_bytes(scalar_type_);
    }

    size_t Image2D::pixel_bytes() const {
        return channels_ * this->scalar_bytes();
    }

    size_t Image2D::row_bytes() const { return x_size_ * this->pixel_bytes(); }

    byte8* Image2D::pixel_ptr(uint32_t idx) {
        return data_.data() + idx * this->pixel_bytes();
    }

    byte8* Image2D::pixel_ptr(uint32_t x, uint32_t y) {
        return this->pixel_ptr(y * x_size_ + x);
    }

    const byte8* Image2D::pixel_ptr(uint32_t idx) const {
        return data_.data() + idx * this->pixel_bytes();
    }

    const byte8* Image2D::pixel_ptr(uint32_t x, uint32_t y) const {
        return this->pixel_ptr(y * x_size_ + x);
    }

}  // namespace sung


namespace sung {

    size_t get_image_scalar_type_bytes(ImageScalarType type) {
        switch (type) {
            case ImageScalarType::uint8:
                return 1;
            case ImageScalarType::float16:
                return 2;
            case ImageScalarType::float32:
                return 4;
            case ImageScalarType::float64:
                return 8;
            default:
                return 0;
        }
    }

}  // namespace sung
