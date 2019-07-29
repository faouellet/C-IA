#include "index.h"

using namespace DistLang;

Index::Index(const std::string& name) : mName{ name }
{
    if (mName.empty())
    {
        mName = "Index";
    }
}
