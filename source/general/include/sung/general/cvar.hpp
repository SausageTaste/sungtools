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
        virtual bool set(int64_t value) = 0;
    };


    struct ICVarFloat : public ICVarValue {
        virtual double get() = 0;
        virtual bool set(double value) = 0;
    };


    struct ICVarStr : public ICVarValue {
        virtual const std::string& get() = 0;
        virtual bool set(const std::string& value) = 0;
    };


    struct ICVars {
        virtual ~ICVars() = default;

        virtual std::shared_ptr<ICVarInt> create_int(
            const std::string& id,
            const std::string& help,
            int64_t value,
            std::function<bool(int64_t)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarFloat> create_float(
            const std::string& id,
            const std::string& help,
            double value,
            std::function<bool(double)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarStr> create_str(
            const std::string& id,
            const std::string& help,
            const std::string& value,
            std::function<bool(const std::string&)> predicate = nullptr
        ) = 0;

        virtual std::shared_ptr<ICVarValue> get(const std::string& id) = 0;

        virtual std::string serialize_str() = 0;

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

        AutoCVarInt(
            const std::string& id,
            const std::string& help,
            int64_t value,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_int(id, help, value);
        }

        AutoCVarInt(
            const std::string& id, const std::string& help, int64_t value
        ) {
            cvar_ = sung::gcvars().create_int(id, help, value);
        }

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        int64_t get() override { return cvar_->get(); }
        bool set(int64_t value) override { return cvar_->set(value); }

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

        AutoCVarFloat(
            const std::string& id,
            const std::string& help,
            double value,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_float(id, help, value);
        }

        AutoCVarFloat(
            const std::string& id, const std::string& help, double value
        ) {
            cvar_ = sung::gcvars().create_float(id, help, value);
        }

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        double get() override { return cvar_->get(); }
        bool set(double value) override { return cvar_->set(value); }

    private:
        std::shared_ptr<ICVarFloat> cvar_;
    };


    class AutoCVarStr : public ICVarStr {

    public:
        AutoCVarStr(
            const std::string& id,
            const std::string& help,
            const std::string& value,
            std::function<bool(const std::string&)> predicate,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_str(id, help, value, predicate);
        }

        AutoCVarStr(
            const std::string& id,
            const std::string& help,
            const std::string& value,
            ICVars& cvars
        ) {
            cvar_ = cvars.create_str(id, help, value);
        }

        AutoCVarStr(
            const std::string& id,
            const std::string& help,
            const std::string& value
        ) {
            cvar_ = sung::gcvars().create_str(id, help, value);
        }

        const std::string& id() override { return cvar_->id(); }
        const std::string& help() override { return cvar_->help(); }
        const std::string& get() override { return cvar_->get(); }
        bool set(const std::string& value) override {
            return cvar_->set(value);
        }

    private:
        std::shared_ptr<ICVarStr> cvar_;
    };

}  // namespace sung
