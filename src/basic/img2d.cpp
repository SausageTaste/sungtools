#include "sung/basic/img2d.hpp"

#include "sung/basic/mamath.hpp"


// ImageMetadata2D
namespace sung {

    void ImageMetadata2D::set_scalar_bytes(const size_t scalar_bytes) {
        scalar_bytes_ = scalar_bytes;
    }

    void ImageMetadata2D::set_channels(const size_t channels) {
        channels_ = channels;
    }

    void ImageMetadata2D::set_x_size(const size_t x) { x_size_ = x; }

    void ImageMetadata2D::set_y_size(const size_t y) { y_size_ = y; }

    void ImageMetadata2D::set_xy_size(const size_t x, const size_t y) {
        this->set_x_size(x);
        this->set_y_size(y);
    }

    void ImageMetadata2D::set_xy_size(const size_t xy) {
        this->set_x_size(xy);
        this->set_y_size(xy);
    }

    void ImageMetadata2D::set_metadata(
        const size_t scalar_bytes,
        const size_t channels,
        const size_t x,
        const size_t y,
        const size_t row_padding
    ) {
        scalar_bytes_ = scalar_bytes;
        channels_ = channels;
        x_size_ = x;
        y_size_ = y;
        row_padding_ = row_padding;
    }

    size_t ImageMetadata2D::channels() const { return channels_; }

    size_t ImageMetadata2D::x_size() const { return x_size_; }

    size_t ImageMetadata2D::y_size() const { return y_size_; }

    size_t ImageMetadata2D::scalar_bytes() const { return scalar_bytes_; }

    size_t ImageMetadata2D::padding_bytes() const { return row_padding_; }

    size_t ImageMetadata2D::pixel_bytes() const {
        return channels_ * scalar_bytes_;
    }

    size_t ImageMetadata2D::row_bytes() const {
        return x_size_ * this->pixel_bytes() + row_padding_;
    }

    size_t ImageMetadata2D::size_bytes() const {
        return y_size_ * this->row_bytes();
    }

    bool ImageMetadata2D::check_idx(size_t x, size_t y) const {
        if (x >= x_size_)
            return false;
        if (y >= y_size_)
            return false;
        return true;
    }

    void ImageMetadata2D::clear_metadata() {
        scalar_bytes_ = 0;
        channels_ = 0;
        x_size_ = 0;
        y_size_ = 0;
        row_padding_ = 0;
    }

    size_t ImageMetadata2D::make_texel_idx(size_t x, size_t y) const {
        const auto pixel_bytes = scalar_bytes_ * channels_;
        const auto row_bytes = pixel_bytes * x_size_ + row_padding_;
        return y * row_bytes + x * pixel_bytes;
    }

    size_t ImageMetadata2D::make_texel_idx_clamp(size_t x, size_t y) const {
        return this->make_texel_idx(
            sung::clamp<size_t>(x, 0, x_size_ - 1),
            sung::clamp<size_t>(y, 0, y_size_ - 1)
        );
    }

}  // namespace sung


namespace sung {

    void Image2D::clear() {
        data_.clear();
        this->clear_metadata();
    }

    void Image2D::resize_data_to_fit() { data_.resize(this->size_bytes()); }

    byte8* Image2D::pixel_ptr(size_t x, size_t y) {
        return data_.data() + this->make_texel_idx(x, y);
    }

    const byte8* Image2D::pixel_ptr(size_t x, size_t y) const {
        return data_.data() + this->make_texel_idx(x, y);
    }

}  // namespace sung
