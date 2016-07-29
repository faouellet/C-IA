#ifndef TENSOR_H
#define TENSOR_H

#include "expr.h"

#include <cassert>

namespace DistLang
{
    template <typename T>
    class Matrix
    {
    public:
        Matrix(const std::string& name = "") : mName{ name }
        {
            if (name.empty())
            {
                mName = "Matrix" + mID++;
            }
        }

        template <typename... Indexes>
        Expr operator()(const Indexes&... indexes) const
        {
            return Expr{ *this, indexes... };
        }

        size_t GetHeight() const { return mData.size(); }
        size_t GetWidth() const { return mData.front().size(); }
        const std::string& GetName() const { return mName; }

    private:
        static size_t mID;
        std::string mName;
        std::vector<std::vector<T>> mData;
    };

    template <typename T> size_t Matrix<T>::mID = 0;
}

#endif // TENSOR_H