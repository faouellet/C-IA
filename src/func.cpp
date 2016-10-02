#include "func.h"

#include <llvm/IR/Module.h>

using namespace DistLang;
using namespace DistLang::impl;

Func::Func(const std::string& name) : mName{ name }, mModule{ nullptr }
{
    if (mName.empty())
    {
        mName = "Func";
    }
}

Func::~Func() { }

void Func::Compile()
{
    ExprIRBuilder builder;
    mExpr.CodeGen(builder);

    builder.CreateFunction(mName);

    // Going from the innermost loop to the outermost
    for (auto idxIt = mIndexes.rbegin(), idxEnd = mIndexes.rend(); idxIt != idxEnd; ++idxIt)
    {
        builder.CreateLoop(*idxIt);
    }
 
    mModule.reset(builder.ReleaseModule());
 
    mModule->dump();
}
