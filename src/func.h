#ifndef FUNC_H
#define FUNC_H

#include "expr.h"

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

    private:
        Expr mExpr;
    };
}

#endif // FUNC_H