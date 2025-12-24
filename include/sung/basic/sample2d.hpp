#pragma once

#include <array>

#include "sung/basic/img2d.hpp"
#include "sung/basic/mamath.hpp"


namespace sung {

    template <typename T, size_t S>
    std::array<T, S> lerp_arr(
        const std::array<T, S>& a, const std::array<T, S>& b, double t
    ) {
        std::array<T, S> result;
        for (size_t i = 0; i < S; ++i) {
            result[i] = static_cast<T>(
                static_cast<double>(a[i]) * (1.0 - t) +
                static_cast<double>(b[i]) * t
            );
        }
        return result;
    }


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

    template <size_t Channels, typename T>
    std::array<T, Channels> sample_bilinear_clamp(
        const TImageView2D<T>& view, double x, double y
    ) {
        const auto x_texel = x * static_cast<double>(view.x_size());
        const auto y_texel = y * static_cast<double>(view.y_size());

        const auto x_tex = std::floor(x_texel);
        const auto y_tex = std::floor(y_texel);
        const auto x_frac = x_texel - x_tex;
        const auto y_frac = y_texel - y_tex;

        const auto p00 = fetch_clamp<Channels, T>(view, x_tex, y_tex);
        const auto p10 = fetch_clamp<Channels, T>(view, x_tex + 1, y_tex);
        const auto p01 = fetch_clamp<Channels, T>(view, x_tex, y_tex + 1);
        const auto p11 = fetch_clamp<Channels, T>(view, x_tex + 1, y_tex + 1);

        const auto p0 = lerp_arr<T, Channels>(p00, p10, x_frac);
        const auto p1 = lerp_arr<T, Channels>(p01, p11, x_frac);
        return lerp_arr<T, Channels>(p0, p1, y_frac);
    }

}  // namespace sung
