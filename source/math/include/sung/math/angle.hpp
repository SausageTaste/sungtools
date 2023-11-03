#pragma once


namespace sung {

    constexpr double PI = 3.14159265358979323846;


    /*
    Strong type for angles
    This class eliminates the confusion between radians and degrees.

    Just be careful and choose correct unit when using `from_*`, `set_*` functions.
    For instance, calling `set_deg` with radians parameter will cause unexpected result.

    The class uses radians internally.
    So using any methods that deals with radians is recommended.
    By that you can avoid unnecessary conversion and precision problem.

    Comparing two angles are not strictly defined.
    For instance, 0 deg and 360 deg can be treated as a same angle, but sometimes not.
    So, you may use `normalize_*` function to normalize the angles before comparing.
    Or you can call `rad` to get numeric value and compare them.
    */
    class Angle {

    public:
        Angle() = default;

        static Angle from_deg(double degrees);
        static Angle from_rad(double radians) { return Angle{ radians }; }

        Angle operator+(const Angle& rhs) const;
        Angle operator-(const Angle& rhs) const;

        // Multiply an angle by a scalar, not an angle.
        // Product of two angles are usually not an angle.
        // You can use `rad` or `deg` functions to get numeric value to use it in your folmulas.
        Angle operator*(double rhs) const;
        Angle operator/(double rhs) const;

        bool is_equivalent(const Angle& rhs, double epsilon = 0.0) const;

        double deg() const;
        double rad() const { return radians_; }

        void set_deg(double degrees);
        void set_rad(double radians) { radians_ = radians; }

        // Normalize to [0, 2pi), retaining the phase
        Angle normalize_pos() const;
        // Normalize to [-pi, pi), retaining the phase
        Angle normalize_neg() const;

        // https://gist.github.com/shaunlebron/8832585
        // Calculate the shortest angular distance from this to rhs
        Angle calc_short_diff(Angle rhs) const;
        Angle lerp(Angle rhs, double t) const;

    private:
        Angle(double radians) : radians_(radians) {}

        double radians_ = 0;

    };

}
