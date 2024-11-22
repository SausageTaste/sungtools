#include "sung/general/cvar.hpp"

#include <stdexcept>
#include <unordered_map>


namespace {

    static std::shared_ptr<sung::ICVars> g_cvars;


    class CVarInt : public sung::ICVarInt {

    public:
        const std::string& id() override { return id_; }
        const std::string& help() override { return help_text_; }
        int64_t get() override { return value_; }
        void set(int64_t value) override { value_ = value; }

        std::string id_;
        std::string help_text_;
        std::function<bool(int64_t)> predicate_;
        int64_t value_;
    };


    class CVarFloat : public sung::ICVarFloat {

    public:
        const std::string& id() override { return id_; }
        const std::string& help() override { return help_text_; }
        double get() override { return value_; }
        void set(double value) override { value_ = value; }

        std::string id_;
        std::string help_text_;
        std::function<bool(double)> predicate_;
        double value_;
    };


    class CVars : public sung::ICVars {

    public:
        bool has(const std::string& id) override {
            return dat_.find(id) != dat_.end();
        }

        std::shared_ptr<sung::ICVarInt> create_int(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(int64_t)> predicate
        ) override {
            auto cvar = std::make_shared<CVarInt>();
            cvar->id_ = id;
            cvar->help_text_ = help;
            cvar->predicate_ = predicate;
            cvar->value_ = value;
            this->try_insert(id, cvar);
            return cvar;
        }

        std::shared_ptr<sung::ICVarFloat> create_float(
            const std::string& id,
            const std::string& help,
            double value,
            std::function<bool(double)> predicate
        ) override {
            auto cvar = std::make_shared<CVarFloat>();
            cvar->id_ = id;
            cvar->help_text_ = help;
            cvar->predicate_ = predicate;
            cvar->value_ = value;
            this->try_insert(id, cvar);
            return cvar;
        }

        std::shared_ptr<sung::ICVarValue> get(const std::string& id) {
            auto it = dat_.find(id);
            if (it == dat_.end())
                return nullptr;
            else
                return it->second;
        }

    private:
        void try_insert(
            const std::string& id, std::shared_ptr<sung::ICVarValue> cvar
        ) {
            if (dat_.find(id) == dat_.end())
                dat_[id] = cvar;
            else
                throw std::runtime_error("CVar already exists: " + id);
        }

        std::unordered_map<std::string, std::shared_ptr<sung::ICVarValue>> dat_;
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
