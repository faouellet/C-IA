#ifndef MATRIX_H
#define MATRIX_H

#include <string>
#include <vector>

namespace DistLang
{
    class Expr;
    class Index;

    class Matrix
    {
    public:
        Matrix(const std::string& name = "");

    public:
        Expr operator()(const Index& hIdx, const Index& wIdx) const;

    public:
        size_t GetHeight() const { return mData.size(); }
        size_t GetWidth() const { return mData.front().size(); }
        const std::string& GetName() const { return mName; }

    private:
        static size_t mID;
        std::string mName;
        std::vector<std::vector<double>> mData;
    };
}

#endif // MATRIX_H