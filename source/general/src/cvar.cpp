#include "sung/general/cvar.hpp"


namespace {

    static std::shared_ptr<sung::ICVars> g_cvars;


    class CVars : public sung::ICVars {

    public:
        bool has(const std::string& key) override { return false; }
    };

}  // namespace


namespace sung {

    std::shared_ptr<ICVars> create_cvars() { return std::make_shared<CVars>(); }

    std::shared_ptr<ICVars> get_cvars_global() {
        if (!g_cvars)
            g_cvars = create_cvars();
        return g_cvars;
    }

    void set_cvars_global(std::shared_ptr<ICVars> cvars);

}  // namespace sung
