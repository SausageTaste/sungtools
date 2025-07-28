#pragma once

#include <array>
#include <string>
#include <vector>

#include "sung/basic/aabb.hpp"
#include "sung/basic/bytes.hpp"


namespace sung {

    class ValArrayAnalyzer {

    public:
        ValArrayAnalyzer();

        void reset();

        void notify(double value);
        // Apply abs to diff
        void notify_abs(double value);

        void do_array(const double* data, size_t count);
        void do_array(
            const byte8* data, size_t count, size_t size, size_t offset
        );

        size_t count() const;
        void set_count(size_t v);

        double maxi() const;
        double mini() const;

        // [0, count - 1] -> [mini, maxi]
        double at(double idx) const;
        // [mini, maxi] -> [0, count - 1]
        double at_inv(double value) const;

        std::vector<double> make_arr() const;

        double diff_theshold_ = 0;

    private:
        sung::Aabb1DLazyInit<double> value_range_;
        sung::Aabb1DLazyInit<double> diff_range_;
        size_t count_;
        double last_value_;
        bool last_value_set_;
    };


    class DenseDataBuilder {

    public:
        struct Record {
            double val_ = 0;
            int count_ = 0;
        };

        DenseDataBuilder() = default;

        DenseDataBuilder(size_t size);

        bool empty() const;
        size_t size() const;

        void clear();
        void free_mem();
        void resize(size_t size);
        void add_val(double val, size_t idx);

        void finalize_1d();

        void copy(std::vector<float>& out) const;

    private:
        std::vector<Record> records_;
    };


    class AaddHeader {

    public:
        class Dimension {

        public:
            void init(double min, double max, uint64_t count, const char* name);

            double mini() const;
            double maxi() const;
            uint64_t count() const;
            std::string name() const;

            double calc_precision() const;

        private:
            LEValue<double> min_;
            LEValue<double> max_;
            LEValue<uint64_t> count_;
            std::array<int8_t, 40> name_;
        };

        enum class DataType {
            int8,
            int16,
            int32,
            int64,

            uint8,
            uint16,
            uint32,
            uint64,

            float16,
            float32,
            float64,
        };

        enum class CompMethod {
            none,
            z,
        };

        void init(
            const uint64_t dim_count,
            const uint64_t desc_len,
            const uint64_t data_count,
            const DataType data_type,
            const CompMethod comp_method
        );

        uint64_t dim_count() const;
        uint64_t desc_len() const;
        uint64_t data_count() const;
        DataType data_type() const;
        CompMethod comp_method() const;

        uint64_t mem_size() const;
        bool is_magic_valid() const;

        static uint64_t get_data_type_size(DataType type);

    private:
        std::array<uint8_t, 8> magic_;
        LEValue<uint64_t> dim_count_;
        LEValue<uint64_t> desc_len_;
        LEValue<uint64_t> data_count_;
        LEValue<uint64_t> data_type_;
        LEValue<uint64_t> comp_method_;

        // array<Dimension, dim_count_>
        // array<int8_t, desc_len_>
        // array<data_type_, data_count_>
    };

}  // namespace sung
