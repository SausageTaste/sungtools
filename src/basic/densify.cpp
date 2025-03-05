#include "sung/basic/densify.hpp"


namespace {

    sung::Optional<size_t> find_next_non_zero(
        const size_t idx,
        const std::vector<sung::DenseDataBuilder::Record>& records
    ) {
        for (size_t i = idx + 1; idx < records.size(); ++i) {
            if (records[i].count_ != 0)
                return i;
        }

        return sung::nullopt;
    }

    sung::Optional<size_t> find_prev_non_zero(
        const size_t idx,
        const std::vector<sung::DenseDataBuilder::Record>& records
    ) {
        for (size_t i = 0; i < idx; ++i) {
            const auto curr_idx = idx - i - 1;
            if (records[curr_idx].count_ != 0)
                return curr_idx;
        }

        return sung::nullopt;
    }

}  // namespace


// ValArrayAnalyzer
namespace sung {

    ValArrayAnalyzer::ValArrayAnalyzer()
        : count_(0), last_value_(0), last_value_set_(false) {}

    void ValArrayAnalyzer::reset() {
        value_range_.reset();
        diff_range_.reset();
        count_ = 0;
        last_value_ = 0;
        last_value_set_ = false;
    }

    void ValArrayAnalyzer::notify(double value) {
        value_range_.set_or_expand(value);

        if (last_value_set_) {
            const auto diff = value - last_value_;
            const auto diff_abs = std::abs(diff);
            if (diff_theshold_ <= diff_abs)
                diff_range_.set_or_expand(diff);
        }

        last_value_ = value;
        last_value_set_ = true;
        ++count_;
    }

    void ValArrayAnalyzer::notify_abs(double value) {
        value_range_.set_or_expand(value);

        if (last_value_set_) {
            const auto diff = value - last_value_;
            const auto diff_abs = std::abs(diff);
            if (diff_theshold_ <= diff_abs)
                diff_range_.set_or_expand(diff_abs);
        }

        last_value_ = value;
        last_value_set_ = true;
        ++count_;
    }

    void ValArrayAnalyzer::do_array(const double* data, size_t count) {
        this->reset();
        for (size_t i = 0; i < count; ++i) {
            this->notify(*(data + i));
        }
    }

    void ValArrayAnalyzer::do_array(
        const byte8* data, size_t count, size_t size, size_t offset
    ) {
        this->reset();
        for (size_t i = 0; i < count; ++i) {
            const auto curr_value = *(const double*)(data + i * size + offset);
            this->notify(curr_value);
        }
    }

    size_t ValArrayAnalyzer::count() const { return count_; }

    void ValArrayAnalyzer::set_count(size_t v) { count_ = v; }

    double ValArrayAnalyzer::maxi() const { return value_range_.maxi(); }

    double ValArrayAnalyzer::mini() const { return value_range_.mini(); }

    double ValArrayAnalyzer::at(double idx) const {
        if (count_ <= 1)
            return mini();
        else
            return idx / double(count_ - 1) * (maxi() - mini()) + mini();
    }

    double ValArrayAnalyzer::at_inv(double value) const {
        if (count_ <= 1)
            return 0;
        else
            return (value - mini()) / (maxi() - mini()) * (count_ - 1);
    }

    std::vector<double> ValArrayAnalyzer::make_arr() const {
        if (count_ == 0)
            return {};
        else if (count_ == 1)
            return { mini() };

        std::vector<double> out(count_);
        for (size_t i = 0; i < count_; ++i) {
            out[i] = this->at(static_cast<double>(i));
        }
        return out;
    }

}  // namespace sung


// DenseDataBuilder
namespace sung {

    DenseDataBuilder::DenseDataBuilder(size_t size) : records_(size) {}

    bool DenseDataBuilder::empty() const { return records_.empty(); }

    size_t DenseDataBuilder::size() const { return records_.size(); }

    void DenseDataBuilder::clear() { records_.clear(); }

    void DenseDataBuilder::free_mem() { records_ = {}; }

    void DenseDataBuilder::resize(size_t size) { records_.resize(size); }

    void DenseDataBuilder::add_val(double val, size_t idx) {
        if (idx >= records_.size())
            return;
        auto& r = records_[idx];
        r.val_ += val;
        r.count_ += 1;
    }

    void DenseDataBuilder::finalize_1d() {
        if (this->empty())
            return;

        // Perform divisions
        for (auto& x : records_) {
            if (x.count_ > 1) {
                x.val_ = x.val_ / x.count_;
                x.count_ = 1;
            }
        }

        // For the case the first record is empty
        if (0 == records_[0].count_) {
            auto next_idx = ::find_next_non_zero(0, records_);
            if (!next_idx)
                return;
            auto& next_r = records_[*next_idx];
            for (size_t i = 0; i < *next_idx; ++i) {
                records_[i] = next_r;
            }
        }

        // For the case the last record is empty
        if (0 == records_.back().count_) {
            auto prev_idx = ::find_prev_non_zero(records_.size() - 1, records_);
            if (!prev_idx)
                return;
            auto& prev_r = records_[*prev_idx];
            for (size_t i = *prev_idx + 1; i < records_.size(); ++i) {
                records_[i] = prev_r;
            }
        }

        // Fill in the gaps
        for (size_t i = 0; i < records_.size(); ++i) {
            if (records_[i].count_ == 0) {
                // Since we've already handled the first and last records,
                // this should always find a non-zero record
                const auto next_idx = ::find_next_non_zero(i, records_);
                const auto& next_r = records_[*next_idx];
                const auto& prev_r = records_[i - 1];

                for (size_t j = i; j < *next_idx; ++j) {
                    const auto t = static_cast<double>(j - i + 1) /
                                   (*next_idx - i + 1);
                    records_[j].val_ = prev_r.val_ +
                                       (next_r.val_ - prev_r.val_) * t;
                    records_[j].count_ = 1;
                }

                continue;
            }
        }
    }

    void DenseDataBuilder::copy(std::vector<float>& out) const {
        out.resize(records_.size());
        for (size_t i = 0; i < records_.size(); ++i) {
            out[i] = static_cast<float>(records_[i].val_);
        }
    }

}  // namespace sung


// AaddHeader
namespace sung {

    void AaddHeader::Dimension::init(
        double min, double max, size_t count, const char* name
    ) {
        min_.set(min);
        max_.set(max);
        count_.set(count);

        const auto copy_size = std::min(name_.size() - 1, std::strlen(name));
        std::memcpy(name_.data(), name, copy_size);
        name_[copy_size] = '\0';
    }

    double AaddHeader::Dimension::mini() const { return min_.get(); }

    double AaddHeader::Dimension::maxi() const { return max_.get(); }

    size_t AaddHeader::Dimension::count() const { return count_.get(); }

    double AaddHeader::Dimension::calc_precision() const {
        return (max_.get() - min_.get()) / count_.get();
    }


    void AaddHeader::init(
        const size_t dim_count,
        const size_t data_count,
        const DataType data_type,
        const CompMethod comp_method
    ) {
        std::memcpy(magic_.data(), "AADD\0\0\0\0", 8);

        dim_count_.set(dim_count);
        data_count_.set(data_count);
        data_type_.set(static_cast<int>(data_type));
        comp_method_.set(static_cast<int>(comp_method));
    }

    size_t AaddHeader::dim_count() const { return dim_count_.get(); }

    size_t AaddHeader::data_count() const { return data_count_.get(); }

    AaddHeader::DataType AaddHeader::data_type() const {
        return static_cast<DataType>(data_type_.get());
    }

    AaddHeader::CompMethod AaddHeader::comp_method() const {
        return static_cast<CompMethod>(comp_method_.get());
    }

    size_t AaddHeader::mem_size() const {
        const auto type_size = this->get_data_type_size(this->data_type());
        return data_count_.get() * type_size;
    }

    bool AaddHeader::is_magic_valid() const {
        return std::memcmp(magic_.data(), "AADD\0\0\0\0", 8) == 0;
    }

    size_t AaddHeader::get_data_type_size(DataType type) {
        switch (type) {
            case DataType::int8:
                return sizeof(int8_t);
            case DataType::uint8:
                return sizeof(uint8_t);
            case DataType::float32:
                return sizeof(float);
            case DataType::float64:
                return sizeof(double);
            default:
                return 0;
        }
    }

}  // namespace sung
