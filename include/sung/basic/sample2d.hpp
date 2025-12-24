#pragma once

#include <array>

#include "sung/basic/img2d.hpp"
#include "sung/basic/mamath.hpp"


namespace sung {

    template <size_t Channels, typename T>
    std::array<T, Channels> fetch_clamp(
        const TImageView2D<T>& view, double x, double y
    ) {
        const auto x_nearest = sung::clamp<double>(x, 0, view.x_size() - 1);
        const auto y_nearest = sung::clamp<double>(y, 0, view.y_size() - 1);
        const auto pixel_ptr = view.pixel_ptr(
            static_cast<size_t>(x_nearest), static_cast<size_t>(y_nearest)
        );

        std::array<T, Channels> result;
        for (size_t c = 0; c < Channels; ++c) {
            result[c] = pixel_ptr[c];
        }
        return result;
    }

    template <size_t Channels, typename T>
    std::array<T, Channels> sample_nearest_clamp(
        const TImageView2D<T>& view, double x, double y
    ) {
        const auto x_texel = x * static_cast<double>(view.x_size());
        const auto y_texel = y * static_cast<double>(view.y_size());

        return fetch_clamp<Channels, T>(
            view, std::floor(x_texel), std::floor(y_texel)
        );
    }

}  // namespace sung
