#pragma once

#include <chrono>
#include <random>


namespace sung {

    template <typename T>
    class RandomIntegerGenerator {

    public:
        RandomIntegerGenerator(T lower_bound, T upper_bound)
            : unif_(lower_bound, upper_bound)
            , lower_bound_(lower_bound)
            , upper_bound_(upper_bound) {
            const auto now = std::chrono::high_resolution_clock::now();
            const auto time_seed = now.time_since_epoch().count();
            std::seed_seq ss{ uint32_t(time_seed & 0xffffffff),
                                    uint32_t(time_seed >> 32) };

            rng_.seed(ss);
        }

        T lower_bound() const { return lower_bound_; }
        T upper_bound() const { return upper_bound_; }

        T gen() { return unif_(rng_); }

    private:
        std::mt19937_64 rng_;
        std::uniform_int_distribution<T> unif_;
        T lower_bound_;
        T upper_bound_;
    };


    template <typename T>
    class RandomRealNumGenerator {

    public:
        RandomRealNumGenerator(T lower_bound, T upper_bound)
            : unif_(lower_bound, upper_bound)
            , lower_bound_(lower_bound)
            , upper_bound_(upper_bound) {
            const auto now = std::chrono::high_resolution_clock::now();
            const auto time_seed = now.time_since_epoch().count();
            std::seed_seq ss{ uint32_t(time_seed & 0xffffffff),
                                    uint32_t(time_seed >> 32) };

            rng_.seed(ss);
        }

        T lower_bound() const { return lower_bound_; }
        T upper_bound() const { return upper_bound_; }

        T gen() { return unif_(rng_); }

    private:
        std::mt19937_64 rng_;
        std::uniform_real_distribution<T> unif_;
        T lower_bound_;
        T upper_bound_;
    };

}  // namespace sung
