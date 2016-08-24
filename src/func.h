#ifndef FUNC_H
#define FUNC_H

#include "expr.h"
#include "index.h"

namespace llvm
{
    class Module;
}

namespace DistLang
{
    class Func
    {
    public:
        Func(const std::string& mName = "");
        ~Func();

    public: // Access
        template <typename... Indexes>
        Expr& operator()(const Indexes&... indexes)
        {
            mIndexes = { indexes... };
            return mExpr;
        }

    public: // Execution
        void Execute(Matrix& mat);

    public: // Scheduling
        Func& Distribute(const Index& idx);
        Func& Interchange();

    private:
        void Compile();

    private:
        class ScheduleAction
        {
        public:
            enum class Action
            {
                DISTRIBUTE,
                INTERCHANGE
            };

        public:
            explicit ScheduleAction(Action a) : mAction{ a } { }
            ScheduleAction(Action a, const Index& idx) : ScheduleAction(a) { mIndexes.push_back(idx); }

        private:
            std::vector<Index> mIndexes;
            Action mAction;
        };

    private:
        std::string mName;
        Expr mExpr;
        std::unique_ptr<llvm::Module> mModule;
        std::vector<Index> mIndexes;
        std::vector<ScheduleAction> mSchedule;

        static size_t mID;
    };
}

#endif // FUNC_H