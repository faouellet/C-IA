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
        using Data = std::vector<std::vector<double>>;

    public:
        Matrix(const std::string& name = "");

    public:
        Expr operator()(const Index& hIdx, const Index& wIdx) const;

    public:
        size_t GetHeight() const { return mData.size(); }
        size_t GetWidth() const { return mData.front().size(); }
        const std::string& GetName() const { return mName; }
        const Data& GetData() const { return mData; }

    private:
        std::string mName;
        Data mData;
    };
}

#endif // MATRIX_H