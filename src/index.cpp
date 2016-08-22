#include "index.h"

using namespace DistLang;

size_t Index::mID = 0;

Index::Index(const std::string& name) : mName{ name }
{
    if (mName.empty())
    {
        mName = "Index" + std::to_string(mID++);
    }
}
