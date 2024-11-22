#pragma once

#include <memory>
#include <string>


namespace sung {

    struct ICVars {
        virtual ~ICVars() = default;

        virtual bool has(const std::string& key) = 0;
    };


    std::shared_ptr<ICVars> create_cvars();

    std::shared_ptr<ICVars> get_cvars_global();
    void set_cvars_global(std::shared_ptr<ICVars> cvars);

    // Global CVars
    inline ICVars& gcvars() { return *get_cvars_global(); }

}  // namespace sung
