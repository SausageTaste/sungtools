#include "sung/gameengine/data_central.hpp"

#include <vector>


namespace sung {

    class InternalDataSharedPtr {

    public:
        size_t id() const { return reinterpret_cast<size_t>(data_.get()); }

        std::shared_mutex mut_;
        std::unique_ptr<IDataAdapter> data_;
    };

}  // namespace sung


namespace {

    class DataCentral : public sung::IDataCentral {

    public:
        sung::DataReader add(std::unique_ptr<sung::IDataAdapter> data
        ) override {
            auto data_ptr = std::make_shared<sung::InternalDataSharedPtr>();
            data_ptr->data_ = std::move(data);
            data_list_.push_back(data_ptr);
            return sung::DataReader{ data_ptr };
        }

        std::optional<sung::DataReader> get_reader_of(
            const sung::DataWriter& accessor
        ) override {
            const auto id = accessor.id();

            for (auto& x : data_list_) {
                if (x->id() == id)
                    return sung::DataReader{ x };
            }

            return std::nullopt;
        }

        std::optional<sung::DataWriter> get_writer_of(
            const sung::DataReader& accessor
        ) override {
            const auto id = accessor.id();

            for (auto& x : data_list_) {
                if (x->id() == id)
                    return sung::DataWriter{ x };
            }

            return std::nullopt;
        }

    private:
        std::vector<std::shared_ptr<sung::InternalDataSharedPtr>> data_list_;
    };

}  // namespace


// DataReader
namespace sung {

    DataReader::DataReader(DataPtr data)
        : data_(data), lock_(data->mut_, std::defer_lock) {}

    DataReader::~DataReader() {}

    DataReader::DataReader(DataReader&& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(lock_, rhs.lock_);
    }

    DataReader& DataReader::operator=(DataReader&& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(lock_, rhs.lock_);
        return *this;
    }

    std::optional<DataReader::ResPair> DataReader::get() {
        if (data_.expired())
            return std::nullopt;

        auto data = data_.lock();
        if (!data) {
            data_.reset();
            return std::nullopt;
        }

        try {
            if (!lock_.try_lock())
                return std::nullopt;

            ResPair out{ std::move(lock_), data->data_.get() };
            lock_ = Lock{ data->mut_, std::defer_lock };
            return out;
        } catch (const std::system_error&) {
            return std::nullopt;
        }
    }

    size_t DataReader::id() const { return data_.lock()->id(); }

    bool DataReader::is_dangling() const { return data_.expired(); }

}  // namespace sung


// WriterReader
namespace sung {

    DataWriter::DataWriter(DataPtr data)
        : data_(data), lock_(data->mut_, std::defer_lock) {}

    DataWriter::~DataWriter() {}

    DataWriter::DataWriter(DataWriter&& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(lock_, rhs.lock_);
    }

    DataWriter& DataWriter::operator=(DataWriter&& rhs) noexcept {
        std::swap(data_, rhs.data_);
        std::swap(lock_, rhs.lock_);
        return *this;
    }

    std::optional<DataWriter::ResPair> DataWriter::get() {
        if (data_.expired())
            return std::nullopt;

        auto data = data_.lock();
        if (!data) {
            data_.reset();
            return std::nullopt;
        }

        try {
            if (!lock_.try_lock())
                return std::nullopt;

            ResPair out{ std::move(lock_), data->data_.get() };
            lock_ = Lock{ data->mut_, std::defer_lock };
            return out;
        } catch (const std::system_error&) {
            return std::nullopt;
        }
    }

    size_t DataWriter::id() const { return data_.lock()->id(); }

    bool DataWriter::is_dangling() const { return data_.expired(); }

}  // namespace sung


namespace sung {

    std::unique_ptr<IDataCentral> create_data_central() {
        return std::make_unique<::DataCentral>();
    }

}  // namespace sung
