#include "matrix.h"

#include "expr.h"
#include "index.h"

using namespace DistLang;

Matrix::Matrix(const std::string& name) : mName{ name }
{
    if (name.empty())
    {
        mName = "Matrix";
    }
}

Expr Matrix::operator()(const Index& hIdx, const Index& wIdx) const
{
    return Expr(*this, hIdx, wIdx);
}
