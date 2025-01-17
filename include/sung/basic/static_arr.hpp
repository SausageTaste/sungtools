#pragma once

#include <array>
#include <cstdint>
#include <stdexcept>


namespace sung {

    template <typename T, size_t N>
    class StaticVector {

    public:
        StaticVector() {}
        ~StaticVector() { this->clear(); }

        bool push_back(const T& item) {
            if (top_ >= N)
                return false;

            auto ptr = &this->storage_at(top_);
            new (ptr) T(item);
            ++top_;
            return true;
        }
        bool push_back(T&& item) {
            if (top_ >= N)
                return false;

            auto ptr = &this->storage_at(top_);
            new (ptr) T(std::move(item));
            ++top_;
            return true;
        }
        bool pop_back() {
            if (top_ == 0)
                return false;

            auto& ptr = this->storage_at(top_ - 1);
            ptr.~T();
            --top_;
            return true;
        }

        void clear() {
            for (size_t i = 0; i < top_; ++i) {
                auto& ptr = this->storage_at(i);
                ptr.~T();
            }
            top_ = 0;
        }

        T& operator[](size_t item_index) {
            return this->storage_at(item_index);
        }
        T const& operator[](size_t item_index) const {
            return this->storage_at(item_index);
        }

        T& at(size_t item_index) {
            if (item_index >= top_)
                throw std::out_of_range("Index out of range");
            return this->storage_at(item_index);
        }
        T const& at(size_t item_index) const {
            if (item_index >= top_)
                throw std::out_of_range("Index out of range");
            return this->storage_at(item_index);
        }

        T* at_op(size_t item_index) {
            if (item_index >= top_)
                return nullptr;
            return &this->storage_at(item_index);
        }
        T const* at_op(size_t item_index) const {
            if (item_index >= top_)
                return nullptr;
            return &this->storage_at(item_index);
        }

        T& front() {
            if (top_ == 0)
                throw std::out_of_range("Empty array");
            return this->storage_at(0);
        }
        T const& front() const {
            if (top_ == 0)
                throw std::out_of_range("Empty array");
            return this->storage_at(0);
        }
        T* front_op() {
            if (top_ == 0)
                return nullptr;
            return &this->storage_at(0);
        }
        T const* front_op() const {
            if (top_ == 0)
                return nullptr;
            return &this->storage_at(0);
        }

        T& back() {
            if (top_ == 0)
                throw std::out_of_range("Empty array");
            return this->storage_at(top_ - 1);
        }
        T const& back() const {
            if (top_ == 0)
                throw std::out_of_range("Empty array");
            return this->storage_at(top_ - 1);
        }
        T* back_op() {
            if (top_ == 0)
                return nullptr;
            return &this->storage_at(top_ - 1);
        }
        T const* back_op() const {
            if (top_ == 0)
                return nullptr;
            return &this->storage_at(top_ - 1);
        }

        size_t size() const { return top_; }

    private:
        T& storage_at(size_t item_index) {
            return *reinterpret_cast<T*>(&data_[item_index * sizeof(T)]);
        }
        const T& storage_at(size_t item_index) const {
            return *reinterpret_cast<T*>(&data_[item_index * sizeof(T)]);
        }

        std::array<uint8_t, sizeof(T) * N> data_;
        size_t top_ = 0;  // Item index
    };

}  // namespace sung
