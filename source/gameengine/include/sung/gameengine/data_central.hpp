#pragma once

#include <memory>
#include <mutex>

#include "sung/general/threading.hpp"


namespace sung {

    struct IDataAdapter {
        virtual ~IDataAdapter() = default;
    };


    struct IDataCentral {
        virtual ~IDataCentral() = default;
    };

    std::unique_ptr<IDataCentral> create_data_central(HTaskSche task_sche);

}  // namespace sung
