#pragma once

#include <vector>

#include "sung/basic/bytes.hpp"


namespace sung {

    class ImageMetadata2D {

    public:
        void set_scalar_bytes(const size_t scalar_bytes);
        void set_channels(const size_t channels);
        void set_x_size(const size_t x);
        void set_y_size(const size_t y);
        void set_xy_size(const size_t x, const size_t y);
        void set_xy_size(const size_t xy);

        void set_metadata(
            const size_t scalar_bytes,
            const size_t channels,
            const size_t x,
            const size_t y,
            const size_t row_padding = 0
        );

        template <typename T>
        void set_metadata(
            const size_t channels,
            const size_t x,
            const size_t y,
            const size_t row_padding = 0
        ) {
            this->set_metadata(sizeof(T), channels, x, y, row_padding);
        }

        size_t channels() const;
        size_t x_size() const;
        size_t y_size() const;

        size_t scalar_bytes() const;
        size_t padding_bytes() const;

        size_t pixel_bytes() const;
        size_t row_bytes() const;
        size_t size_bytes() const;

        bool check_idx(size_t x, size_t y) const;

    protected:
        void clear_metadata();
        size_t make_texel_idx(size_t x, size_t y) const;
        size_t make_texel_idx_clamp(size_t x, size_t y) const;

    private:
        size_t scalar_bytes_ = 0;
        size_t channels_ = 0;
        size_t x_size_ = 0;
        size_t y_size_ = 0;
        size_t row_padding_ = 0;
    };


    template <typename DataType>
    class TImageView2D : public ImageMetadata2D {

    public:
        void set(
            const DataType* data,
            size_t channels,
            size_t x,
            size_t y,
            size_t row_padding = 0
        ) {
            data_ = data;
            this->set_metadata<DataType>(channels, x, y, row_padding);
        }

        void clear() {
            data_ = nullptr;
            this->clear_metadata();
        }

        const DataType* pixel_ptr(size_t x, size_t y) const {
            const auto index = this->make_texel_idx_clamp(x, y);
            return reinterpret_cast<const DataType*>(
                reinterpret_cast<const byte8*>(data_) + index
            );
        }

    private:
        const DataType* data_ = nullptr;
    };


    class Image2D : public ImageMetadata2D {

    public:
        void clear();
        void resize_data_to_fit();

        byte8* pixel_ptr(size_t x, size_t y);
        const byte8* pixel_ptr(size_t x, size_t y) const;

        template <typename T>
        T* pixel_ptr(size_t x, size_t y) {
            return reinterpret_cast<T*>(this->pixel_ptr(x, y));
        }

        template <typename T>
        const T* pixel_ptr(size_t x, size_t y) const {
            return reinterpret_cast<const T*>(this->pixel_ptr(x, y));
        }

        template <typename T>
        TImageView2D<T> make_view() const {
            TImageView2D<T> view;
            view.set(
                reinterpret_cast<const T*>(data_.data()),
                this->channels(),
                this->x_size(),
                this->y_size(),
                this->padding_bytes()
            );
            return view;
        }

    public:
        std::vector<byte8> data_;
    };

}  // namespace sung
