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


    struct ICVarInt : public ICVarValue {
        virtual int64_t get() = 0;
        virtual void set(int64_t value) = 0;
    };


    struct ICVarFloat : public ICVarValue {
        virtual double get() = 0;
        virtual void set(double value) = 0;
    };


    struct ICVars {
        virtual ~ICVars() = default;

        virtual bool has(const std::string& id) = 0;

        virtual std::shared_ptr<ICVarInt> create_int(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(int64_t)> predicate
        ) = 0;

        virtual std::shared_ptr<ICVarFloat> create_float(
            const std::string& id,
            const std::string& help,
            double value,
            std::function<bool(double)> predicate
        ) = 0;

        virtual std::shared_ptr<ICVarValue> get(const std::string& id) = 0;

        std::shared_ptr<ICVarInt> geti(const std::string& id) {
            return std::dynamic_pointer_cast<ICVarInt>(this->get(id));
        }
        std::shared_ptr<ICVarFloat> getf(const std::string& id) {
            return std::dynamic_pointer_cast<ICVarFloat>(this->get(id));
        }
    };


    std::shared_ptr<ICVars> create_cvars();

    std::shared_ptr<ICVars> get_cvars_global();
    void set_cvars_global(std::shared_ptr<ICVars> cvars);
    // Global CVars
    inline ICVars& gcvars() { return *get_cvars_global(); }


    class AutoCVarInt : public ICVarInt {

    public:
        AutoCVarInt(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(int64_t)> predicate,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_int(id, help, value, predicate);
        }

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        int64_t get() override { return cvar_->get(); }
        void set(int64_t value) override { cvar_->set(value); }

    private:
        std::shared_ptr<ICVarInt> cvar_;
    };


    class AutoCVarFloat : public ICVarFloat {

    public:
        AutoCVarFloat(
            const std::string& id,
            const std::string& help,
            double value,
            std::function<bool(double)> predicate,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_float(id, help, value, predicate);
        }

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        double get() override { return cvar_->get(); }
        void set(double value) override { cvar_->set(value); }

    private:
        std::shared_ptr<ICVarFloat> cvar_;
    };

}  // namespace sung
