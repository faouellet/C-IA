#ifndef FUNC_H
#define FUNC_H

#include "expr.h"
#include "jit.h"

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

        }

    private:
        Expr mExpr;
    };
}

#endif // FUNC_H