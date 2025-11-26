#pragma once

#include <vector>

#include "sung/basic/bytes.hpp"


namespace sung {

    template <typename DataType>
    class TImage3D {

    public:
        bool alloc(size_t channels, size_t x, size_t y = 1, size_t z = 1) {
            channels_ = channels;
            x_size_ = x;
            y_size_ = y;
            z_size_ = z;
            data_.resize(channels_ * width_ * height_ * depth_);
            return true;
        }

        void clear() {
            data_.clear();
            channels_ = 0;
            x_size_ = 0;
            y_size_ = 0;
            z_size_ = 0;
        }

        size_t channels() const override { return channels_; }
        size_t width() const override { return x_size_; }
        size_t height() const override { return y_size_; }
        size_t depth() const override { return z_size_; }
        size_t value_type_size() const override { return sizeof(DataType); }

        DataType* texel_ptr(size_t x, size_t y = 0, size_t z = 0) {
            const auto index = this->make_texel_idx(x, y, z);
            return data_.data() + index;
        }

        const DataType* texel_ptr(size_t x, size_t y = 0, size_t z = 0) const {
            const auto index = this->make_texel_idx(x, y, z);
            return data_.data() + index;
        }

        const byte8* buffer_data() const {
            return reinterpret_cast<const byte8*>(data_.data());
        }

        size_t buffer_size() const { return data_.size() * sizeof(T); }

        size_t make_texel_idx(size_t x, size_t y, size_t z) const {
            const auto texel_size = channels_;
            const auto row_size = texel_size * x_size_;
            const auto slice_size = row_size * y_size_;
            return z * slice_size + y * row_size + x * texel_size;
        }

        std::vector<DataType> data_;
        size_t channels_ = 0;
        size_t x_size_ = 0;
        size_t y_size_ = 0;
        size_t z_size_ = 0;
    };

}  // namespace sung
