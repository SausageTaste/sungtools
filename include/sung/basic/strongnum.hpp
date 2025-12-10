#pragma once


namespace sung {

    template <typename T, typename Tag>
    class StrongNumber {

    public:
        constexpr StrongNumber() = default;

        constexpr StrongNumber(const StrongNumber& rhs) : value_(rhs.value_) {}

        explicit constexpr StrongNumber(const T& value) : value_(value) {}

        constexpr StrongNumber& operator=(const StrongNumber& rhs) {
            value_ = rhs.value_;
            return *this;
        }

        constexpr bool operator==(const StrongNumber& rhs) const {
            return value_ == rhs.value_;
        }

        constexpr bool operator!=(const StrongNumber& rhs) const {
            return value_ != rhs.value_;
        }

        constexpr bool operator<(const StrongNumber& rhs) const {
            return value_ < rhs.value_;
        }

        constexpr operator T() const { return value_; }

        constexpr T get() const { return value_; }
        constexpr void set(const T& value) { value_ = value; }

    private:
        T value_;
    };

}  // namespace sung
