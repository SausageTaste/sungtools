#pragma once


namespace sung {

    // This class must allow integers as T
    template <typename T>
    class AABB2 {

    public:
        AABB2() = default;

        AABB2(T x0, T x1, T y0, T y1) {
            this->set(x0, x1, y0, y1);
        }

        T x_min() const { return x_min_; }
        T y_min() const { return y_min_; }
        T x_max() const { return x_max_; }
        T y_max() const { return y_max_; }

        T width() const { return x_max_ - x_min_; }
        T height() const { return y_max_ - y_min_; }
        T area() const {
            return this->width() * this->height();
        }

        // Returns true if the point is inside the volume, not on the surface of it
        bool is_inside(T x, T y) const {
            return x > x_min_ && x < x_max_ && y > y_min_ && y < y_max_;
        }

        // Returns true if the point is inside the volume or on the surface of it
        bool is_contacting(T x, T y) const {
            return x >= x_min_ && x <= x_max_ && y >= y_min_ && y <= y_max_;
        }

        bool make_intersection(const AABB2& other, AABB2& output) const {
            if (x_max_ < other.x_min_ || x_min_ > other.x_max_ || y_max_ < other.y_min_ || y_min_ > other.y_max_)
                return false;

            output.x_min_ = std::max(x_min_, other.x_min_);
            output.x_max_ = std::min(x_max_, other.x_max_);
            output.y_min_ = std::max(y_min_, other.y_min_);
            output.y_max_ = std::min(y_max_, other.y_max_);
            return true;
        }

        double sharing_area(const AABB2& other) const {
            AABB2 intersection;
            if (!this->make_intersection(other, intersection))
                return 0;

            return intersection.area();
        }

        // Yes it makes the volume 0
        void set(T x, T y) {
            x_min_ = x;
            x_max_ = x;
            y_min_ = y;
            y_max_ = y;
        }

        void set(T x0, T x1, T y0, T y1) {
            if (x0 < x1) {
                x_min_ = x0;
                x_max_ = x1;
            }
            else {
                x_min_ = x1;
                x_max_ = x0;
            }

            if (y0 < y1) {
                y_min_ = y0;
                y_max_ = y1;
            }
            else {
                y_min_ = y1;
                y_max_ = y0;
            }
        }

        void resize_to_include(T x, T y) {
            if (x < x_min_)
                x_min_ = x;
            if (x > x_max_)
                x_max_ = x;

            if (y < y_min_)
                y_min_ = y;
            if (y > y_max_)
                y_max_ = y;
        }

    private:
        T x_min_ = 0;
        T y_min_ = 0;
        T x_max_ = 0;
        T y_max_ = 0;

    };

}
