#pragma once

#include <memory>
#include <optional>
#include <shared_mutex>

#include "sung/general/threading.hpp"


namespace sung {

    class InternalDataSharedPtr;


    struct IDataAdapter {
        virtual ~IDataAdapter() = default;
        virtual void load() = 0;
        virtual void finalize() = 0;
    };


    class DataReader {

    public:
        using Lock = std::shared_lock<std::shared_mutex>;
        using ResPair = std::pair<Lock, const IDataAdapter*>;
        using DataPtr = std::shared_ptr<InternalDataSharedPtr>;

        DataReader() = default;
        DataReader(DataPtr data);
        ~DataReader();

        DataReader(DataReader&&) noexcept;
        DataReader& operator=(DataReader&&) noexcept;

        std::optional<ResPair> get();
        size_t id() const;
        bool is_dangling() const;

    private:
        std::weak_ptr<InternalDataSharedPtr> data_;
        Lock lock_;
    };


    class DataWriter {

    public:
        using Lock = std::unique_lock<std::shared_mutex>;
        using ResPair = std::pair<Lock, IDataAdapter*>;
        using DataPtr = std::shared_ptr<InternalDataSharedPtr>;

        DataWriter() = default;
        DataWriter(DataPtr data);
        ~DataWriter();

        DataWriter(DataWriter&&) noexcept;
        DataWriter& operator=(DataWriter&&) noexcept;

        std::optional<ResPair> get();
        size_t id() const;
        bool is_dangling() const;

    private:
        std::weak_ptr<InternalDataSharedPtr> data_;
        Lock lock_;
    };


    struct IDataCentral {
        virtual ~IDataCentral() = default;

        virtual DataReader add(std::unique_ptr<IDataAdapter> data) = 0;

        virtual std::optional<DataReader> get_reader_of(
            const DataWriter& accessor
        ) = 0;

        virtual std::optional<DataWriter> get_writer_of(
            const DataReader& accessor
        ) = 0;
    };

    std::unique_ptr<IDataCentral> create_data_central(HTaskSche task_sche);

}  // namespace sung
