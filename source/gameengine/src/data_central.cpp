#include "sung/gameengine/data_central.hpp"


namespace {

    class DataCentral : public sung::IDataCentral {

    public:
        DataCentral(sung::HTaskSche task_sche) : task_sche_{ task_sche } {}

    private:
        sung::HTaskSche task_sche_;
    };

}  // namespace


namespace sung {

    std::unique_ptr<IDataCentral> create_data_central(HTaskSche task_sche) {
        return std::make_unique<::DataCentral>(task_sche);
    }

}  // namespace sung
