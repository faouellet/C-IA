#ifndef TENSOR_H
#define TENSOR_H

#include "expr.h"

#include <cassert>

namespace DistLang
{
    template <typename T>
    class Tensor
    {
    public:
        Tensor() = default;
        explicit Tensor(int dim) : mDim{ dim } { assert(dim > -1); }

        template <typename... Indexes>
        Expr operator()(const Indexes&... indexes) const
        {
            return Expr{ *this, indexes };
        }

        int GetDim() const { return mDim; }

    private:
        int mDim;
    };
}

#endif // TENSOR_H