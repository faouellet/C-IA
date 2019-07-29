#ifndef FUNC_H
#define FUNC_H

#include "expr.h"
#include "exprirbuilder.h"
#include "index.h"
#include "jit.h"

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
        template <typename... IndexesT>
        Expr& operator()(const IndexesT&... indexes)
        {
            mIndexes = { indexes... };
            return mExpr;
        }

    public: // Execution
        template <typename... DataT>
        Matrix Execute(const DataT&... matrices)
        {
            if (mModule == nullptr)
            {
                Compile();
            }

            assert(mModule != nullptr);

            JITCompiler::GetInstance().FindSymbol(mName);

            return{};
        }

    private:
        void Compile();

    private:
        std::string mName;
        Expr mExpr;
        std::unique_ptr<llvm::Module> mModule;
        std::vector<Index> mIndexes;
    };
}

#endif // FUNC_H