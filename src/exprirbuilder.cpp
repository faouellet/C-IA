#include "exprirbuilder.h"

#include "index.h"

using namespace DistLang::impl;
using namespace llvm;

ExprIRBuilder::ExprIRBuilder() : mContext{}, mBuilder{ mContext }, mNamedVariables{}
{
    mMod = std::make_unique<Module>("Mod", mContext);

    PointerType* arrType = PointerType::get(mBuilder.getDoubleTy(), 0);
    PointerType* multiArrType = PointerType::get(arrType, 0);
    StructType* tensorType = StructType::create(mContext, multiArrType, "struct.matrix");

    FunctionType* fType = FunctionType::get(tensorType, false);
    Function* func = Function::Create(fType, Function::ExternalLinkage, "Func", mMod.get());
    mDefBlock = BasicBlock::Create(mContext, "DefBlock", func);
    mExprBlock = BasicBlock::Create(mContext, "ExprBlock", func);
}

void ExprIRBuilder::AddIndex(const Index& i)
{
    if (!IsVarDefined(i.GetName()))
    {
        mBuilder.SetInsertPoint(mDefBlock->getTerminator());
        // TODO
    }
}
