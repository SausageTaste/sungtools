#pragma once

#include <vector>

#include "sung/basic/bytes.hpp"


namespace sung {

    class ImageMetadata3D {

    public:
        size_t channels() const { return channels_; }
        size_t width() const { return x_size_; }
        size_t height() const { return y_size_; }
        size_t depth() const { return z_size_; }

        bool check_idx(size_t x, size_t y, size_t z) const {
            if (x >= x_size_)
                return false;
            if (y >= y_size_)
                return false;
            if (z >= z_size_)
                return false;
            return true;
        }

    protected:
        size_t make_texel_idx(size_t x, size_t y, size_t z) const {
            const auto texel_size = channels_;
            const auto row_size = texel_size * x_size_;
            const auto slice_size = row_size * y_size_;
            return z * slice_size + y * row_size + x * texel_size;
        }

        size_t make_texel_idx_clamp(size_t x, size_t y, size_t z) const {
            return this->make_texel_idx(
                sung::clamp<size_t>(x, 0, x_size_ - 1),
                sung::clamp<size_t>(y, 0, y_size_ - 1),
                sung::clamp<size_t>(z, 0, z_size_ - 1)
            );
        }

        void set_size(size_t channels, size_t x, size_t y, size_t z) {
            channels_ = channels;
            x_size_ = x;
            y_size_ = y;
            z_size_ = z;
        }

        void clear() {
            channels_ = 0;
            x_size_ = 0;
            y_size_ = 0;
            z_size_ = 0;
        }

    private:
        size_t channels_ = 0;
        size_t x_size_ = 0;
        size_t y_size_ = 0;
        size_t z_size_ = 0;
    };


    template <typename DataType>
    class TImage3D : public ImageMetadata3D {

    public:
        bool alloc(size_t channels, size_t x, size_t y = 1, size_t z = 1) {
            this->set_size(channels, x, y, z);
            data_.resize(channels_ * x * y * z);
            return true;
        }

        void clear() {
            data_.clear();
            this->ImageMetadata3D::clear();
        }

        size_t value_type_size() const { return sizeof(DataType); }

        DataType* texel_ptr(size_t x, size_t y = 0, size_t z = 0) {
            const auto index = this->make_texel_idx_clamp(x, y, z);
            return data_.data() + index;
        }

        const DataType* texel_ptr(size_t x, size_t y = 0, size_t z = 0) const {
            const auto index = this->make_texel_idx_clamp(x, y, z);
            return data_.data() + index;
        }

        const byte8* buffer_data() const {
            return reinterpret_cast<const byte8*>(data_.data());
        }

        std::vector<DataType> data_;
        size_t channels_ = 0;
        size_t x_size_ = 0;
        size_t y_size_ = 0;
        size_t z_size_ = 0;
    };


    template <typename DataType>
    class TImageView3D : public ImageMetadata3D {

    public:
        void set(
            const DataType* data, size_t channels, size_t x, size_t y, size_t z
        ) {
            data_ = data;
            this->set_size(channels, x, y, z);
        }

        void clear() {
            data_ = nullptr;
            this->ImageMetadata3D::clear();
        }

        size_t value_type_size() const { return sizeof(DataType); }

        const DataType* texel_ptr(size_t x, size_t y = 0, size_t z = 0) const {
            const auto index = this->make_texel_idx_clamp(x, y, z);
            return data_ + index;
        }

        const byte8* buffer_data() const {
            return reinterpret_cast<const byte8*>(data_);
        }

        const DataType* data_ = nullptr;
    };

}  // namespace sung
