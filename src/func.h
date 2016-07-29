#ifndef FUNC_H
#define FUNC_H

#include "expr.h"
#include "jit.h"

namespace DistLang
{
    class Func
    {
    public:
        ~Func() = default;

    public:
        template <typename... Indexes>
        Expr& operator()(const Indexes&... indexes)
        {
            return mExpr;
        }

        template <typename T>
        void Execute(Matrix<T>& tensor) 
        {
            if (mModule == nullptr)
            {
                mModule.reset(mExpr.GetIR().release());
            }

            assert(mModule != nullptr);

            mModule->dump();
            //JITCompiler::GetInstance().FindSymbol()
        }

    private:
        Expr mExpr;
        std::unique_ptr<llvm::Module> mModule;
    };
}

#endif // FUNC_H