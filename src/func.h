#ifndef FUNC_H
#define FUNC_H

#include "expr.h"
#include "jit.h"

#include <llvm/IR/Module.h>

namespace DistLang
{
    class Func
    {
    public:
        template <typename... Indexes>
        Expr& operator()(const Indexes&... indexes)
        {
            mExpr = { indexes... };
            return mExpr;
        }

        template <typename T>
        void Execute(Tensor<T>& tensor) 
        {
            if (mModule == nullptr)
            {
                mModule.reset(mExpr.GetIR());
            }

            JITCompiler::GetInstance().FindSymbol()
        }

    private:
        Expr mExpr;
        std::unique_ptr<llvm::Module> mModule;
    };
}

#endif // FUNC_H