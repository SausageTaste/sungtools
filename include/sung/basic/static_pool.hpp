#pragma once

#include <array>
#include <atomic>
#include <type_traits>


namespace sung {

    template <typename T, size_t N>
    class StaticPool {

    public:
        StaticPool() = default;

        ~StaticPool() noexcept { this->clear(); }

        StaticPool(const StaticPool&) = delete;
        StaticPool& operator=(const StaticPool&) = delete;
        StaticPool(StaticPool&&) = delete;
        StaticPool& operator=(StaticPool&&) = delete;

        template <typename... Args>
        [[nodiscard]] T* alloc(Args&&... args) {
            static_assert(std::is_nothrow_destructible_v<T>);
            static_assert(std::is_constructible_v<T, Args...>);

            for (size_t i = 0; i < N; ++i) {
                bool expected = false;
                const auto changed = used_[i].compare_exchange_strong(
                    expected, true, std::memory_order_acquire
                );

                if (changed)
                    return new (data_ + i) T(std::forward<Args>(args)...);
            }

            return nullptr;
        }

        void free(T* ptr) {
            const auto index = this->make_index(ptr);
            if (!this->is_valid(index))
                return;

            ptr->~T();
            used_[index].store(false, std::memory_order_release);
        }

        void clear() {
            for (size_t i = 0; i < N; ++i) {
                if (used_[i]) {
                    used_[i].store(false, std::memory_order_release);
                    reinterpret_cast<T*>(data_ + i)->~T();
                }
            }
        }

        bool is_valid(const T* ptr) const {
            const auto index = this->make_index(ptr);
            return this->is_valid(index);
        }

        size_t capacity() const { return N; }

        size_t active_count() const {
            return std::count(used_.begin(), used_.end(), true);
        }

    private:
        size_t make_index(const T* ptr) const {
            const auto i_begin = reinterpret_cast<uintptr_t>(data_);
            const auto i_ptr = reinterpret_cast<uintptr_t>(ptr);
            return (i_ptr - i_begin) / sizeof(T);
        }

        bool is_valid(const size_t index) const {
            if (index < N)
                return used_[index].load(std::memory_order_acquire);
            else
                return false;
        }

        std::aligned_storage_t<sizeof(T), alignof(T)> data_[N];
        std::array<std::atomic_bool, N> used_;
    };

}  // namespace sung
