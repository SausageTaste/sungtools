#include "sung/general/cvar.hpp"

#include <sstream>
#include <stdexcept>
#include <unordered_map>

#include "sung/general/stringtool.hpp"


namespace {

    static std::shared_ptr<sung::ICVars> g_cvars;


    bool is_valid_id(const std::string& id) {
        if (id.empty())
            return false;

        const auto serialized = sung::serialize_str(id);
        if (id != serialized.substr(1, serialized.size() - 2))
            return false;

        return true;
    }

    void assert_valid_id(const std::string& id) {
        if (!is_valid_id(id))
            throw std::runtime_error("Invalid CVar ID: " + id);
    }


    class CVarInt : public sung::ICVarInt {

    public:
        const std::string& id() override { return id_; }
        const std::string& help() override { return help_text_; }
        int64_t get() override { return value_; }
        bool set(int64_t value) override {
            if (predicate_ && !predicate_(value))
                return false;

            value_ = value;
            return true;
        }

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
        bool set(double value) override {
            if (predicate_ && !predicate_(value))
                return false;

            value_ = value;
            return true;
        }

        std::string id_;
        std::string help_text_;
        std::function<bool(double)> predicate_;
        double value_;
    };


    class CVarStr : public sung::ICVarStr {

    public:
        const std::string& id() override { return id_; }
        const std::string& help() override { return help_text_; }
        const std::string& get() override { return value_; }
        bool set(const std::string& value) override {
            if (predicate_ && !predicate_(value))
                return false;

            value_ = value;
            return true;
        }

        std::string id_;
        std::string help_text_;
        std::function<bool(const std::string&)> predicate_;
        std::string value_;
    };


    class CVars : public sung::ICVars {

    public:
        std::shared_ptr<sung::ICVarInt> create_int(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(int64_t)> predicate
        ) override {
            ::assert_valid_id(id);

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
            ::assert_valid_id(id);

            auto cvar = std::make_shared<CVarFloat>();
            cvar->id_ = id;
            cvar->help_text_ = help;
            cvar->predicate_ = predicate;
            cvar->value_ = value;
            this->try_insert(id, cvar);
            return cvar;
        }

        std::shared_ptr<sung::ICVarStr> create_str(
            const std::string& id,
            const std::string& help,
            const std::string& value,
            std::function<bool(const std::string&)> predicate
        ) override {
            ::assert_valid_id(id);

            auto cvar = std::make_shared<CVarStr>();
            cvar->id_ = id;
            cvar->help_text_ = help;
            cvar->predicate_ = predicate;
            cvar->value_ = value;
            this->try_insert(id, cvar);
            return cvar;
        }

        std::shared_ptr<sung::ICVarValue> get(const std::string& id) override {
            auto it = dat_.find(id);
            if (it == dat_.end())
                return nullptr;
            else
                return it->second;
        }

        void visit(sung::ICVarVisitor& visitor) override {
            for (const auto& it : dat_) {
                auto cvar = it.second.get();
                if (auto cvar_int = dynamic_cast<::CVarInt*>(cvar))
                    visitor.visit(*cvar_int);
                else if (auto cvar_float = dynamic_cast<::CVarFloat*>(cvar))
                    visitor.visit(*cvar_float);
                else if (auto cvar_str = dynamic_cast<::CVarStr*>(cvar))
                    visitor.visit(*cvar_str);
                else
                    throw std::runtime_error("Unknown CVar type");
            }
        }

        std::string serialize_str() override {
            std::stringstream ss;
            for (const auto& it : dat_) {
                ss << it.first;

                auto cvar = it.second.get();
                if (auto cvar_int = dynamic_cast<::CVarInt*>(cvar))
                    ss << ": int = " << cvar_int->get();
                else if (auto cvar_float = dynamic_cast<::CVarFloat*>(cvar))
                    ss << ": float = " << std::fixed << cvar_float->get();
                else if (auto cvar_str = dynamic_cast<::CVarStr*>(cvar))
                    ss << ": str = " << sung::serialize_str(cvar_str->get());
                else
                    throw std::runtime_error("Unknown CVar type");

                ss << "\n";
            }
            return ss.str();
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
