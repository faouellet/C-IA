#include "matrix.h"

#include "expr.h"
#include "index.h"

using namespace DistLang;

size_t Matrix::mID = 0;

Matrix::Matrix(const std::string& name) : mName{ name }
{
    if (name.empty())
    {
        mName = "Matrix" + std::to_string(mID++);
    }
}

Expr Matrix::operator()(const Index& hIdx, const Index& wIdx) const
{
    return Expr(*this, hIdx, wIdx);
}
