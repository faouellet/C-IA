#include "func.h"

#include "exprirbuilder.h"
#include "jit.h"

#include <llvm/IR/Module.h>

using namespace DistLang;
using namespace DistLang::impl;

size_t Func::mID = 0;

Func::Func(const std::string& name) : mName{ name }, mModule{ nullptr }, mSchedule{ }
{
    if (mName.empty())
    {
        mName = "Func" + std::to_string(mID++);
    }
}

Func::~Func() { }

void Func::Execute(Matrix& mat)
{
    if (mModule == nullptr)
    {
        Compile();
    }

    assert(mModule != nullptr);

    JITCompiler::GetInstance().FindSymbol(mName);
}

Func& Func::Distribute(const Index& idx)
{
    mSchedule.emplace_back(ScheduleAction::Action::DISTRIBUTE, idx);
    return *this;
}

Func& Func::Interchange()
{
    mSchedule.emplace_back(ScheduleAction::Action::INTERCHANGE);
    return *this;
}

void Func::Compile()
{
    ExprIRBuilder builder;
    mExpr.CodeGen(builder);

    builder.CreateFunction(mName);


    //for (const auto& sched : mSchedule)
    //{
    //}

    // Going from the innermost loop to the outermost
    for (auto idxIt = mIndexes.rbegin(), idxEnd = mIndexes.rend(); idxIt != idxEnd; ++idxIt)
    {
        builder.CreateLoop(*idxIt);
    }
 
    mModule.reset(builder.ReleaseModule());
 
    mModule->dump();
}
