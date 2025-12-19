#pragma once

#include <functional>
#include <memory>
#include <string>


namespace sung {

    struct ICVarValue {
        virtual ~ICVarValue() = default;
        virtual const std::string& id() = 0;
        virtual const std::string& help() = 0;
    };


    template <typename T>
    struct TCVarValue : public ICVarValue {
        virtual const T& get() = 0;
        virtual bool set(const T& value) = 0;
    };

    using ICVarInt = TCVarValue<int64_t>;
    using ICVarFloat = TCVarValue<double>;
    using ICVarStr = TCVarValue<std::string>;


    struct ICVarVisitor {
        virtual ~ICVarVisitor() = default;
        virtual void visit(ICVarInt& cvar) = 0;
        virtual void visit(ICVarFloat& cvar) = 0;
        virtual void visit(ICVarStr& cvar) = 0;
    };


    struct ICVars {
        virtual ~ICVars() = default;

        virtual std::shared_ptr<ICVarInt> create(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(const int64_t&)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarFloat> create(
            const std::string& id,
            const std::string& help,
            double value,
            std::function<bool(const double&)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarStr> create(
            const std::string& id,
            const std::string& help,
            const std::string& value,
            std::function<bool(const std::string&)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarValue> get(const std::string& id) = 0;

        virtual void visit(ICVarVisitor& visitor) = 0;

        virtual std::string serialize_str() = 0;

        template <typename T>
        std::shared_ptr<TCVarValue<T>> t_create(
            const std::string& id,
            const std::string& help,
            const T& value,
            std::function<bool(const T&)> predicate = nullptr
        ) {
            return this->create(id, help, value, predicate);
        }

        std::shared_ptr<ICVarInt> geti(const std::string& id) {
            return std::dynamic_pointer_cast<ICVarInt>(this->get(id));
        }
        std::shared_ptr<ICVarFloat> getf(const std::string& id) {
            return std::dynamic_pointer_cast<ICVarFloat>(this->get(id));
        }
        std::shared_ptr<ICVarStr> gets(const std::string& id) {
            return std::dynamic_pointer_cast<ICVarStr>(this->get(id));
        }
    };


    std::shared_ptr<ICVars> create_cvars();

    std::shared_ptr<ICVars> get_cvars_global();
    void set_cvars_global(std::shared_ptr<ICVars> cvars);
    // Global CVars
    inline ICVars& gcvars() { return *get_cvars_global(); }


    template <typename T>
    class TAutoCVar : public TCVarValue<T> {

    public:
        TAutoCVar(
            const std::string& id,
            const std::string& help,
            const T& value,
            std::function<bool(const T&)> predicate,
            ICVars& cvars
        ) {
            cvar_ = cvars.create(id, help, value, predicate);
        }

        TAutoCVar(
            const std::string& id,
            const std::string& help,
            const T& value,
            std::function<bool(const T&)> predicate
        )
            : TAutoCVar(id, help, value, predicate, sung::gcvars()) {}

        TAutoCVar(
            const std::string& id,
            const std::string& help,
            const T& value,
            ICVars& cvars
        )
            : TAutoCVar(id, help, value, nullptr, cvars) {}

        TAutoCVar(
            const std::string& id, const std::string& help, const T& value
        )
            : TAutoCVar(id, help, value, nullptr, sung::gcvars()) {}

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        const T& get() override { return cvar_->get(); }
        bool set(const T& value) override { return cvar_->set(value); }

    private:
        std::shared_ptr<TCVarValue<T>> cvar_;
    };

    using AutoCVarInt = TAutoCVar<int64_t>;
    using AutoCVarFlt = TAutoCVar<double>;
    using AutoCVarStr = TAutoCVar<std::string>;

}  // namespace sung
