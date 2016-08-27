#include "exprirbuilder.h"

#include "index.h"

#include <llvm/IR/Module.h>

using namespace DistLang;
using namespace DistLang::impl;
using namespace llvm;

ExprIRBuilder::ExprIRBuilder() : mContext{}, mBuilder{ mContext }, mNamedVariables{}
{
    mMod = std::make_unique<Module>("Mod", mContext);

    PointerType* arrType = PointerType::get(mBuilder.getDoubleTy(), 0);
    PointerType* multiArrType = PointerType::get(arrType, 0);
    mMatType = StructType::create(mContext, multiArrType, "struct.matrix");
        
    mDefBlock = BasicBlock::Create(mContext, "DefBlock");
    mExprBlock = BasicBlock::Create(mContext, "ExprBlock");
}

void ExprIRBuilder::CreateFunction(const std::string& name)
{
    FunctionType* fType = FunctionType::get(mMatType, false);
    Function* func = Function::Create(fType, Function::ExternalLinkage, name, mMod.get());

    assert(mDefBlock != nullptr);
    mDefBlock->insertInto(func);

    assert(mExprBlock != nullptr);
    mExprBlock->insertInto(func);

    mBuilder.SetInsertPoint(mDefBlock);
    mBuilder.CreateBr(mExprBlock);
}

Value* ExprIRBuilder::CreateIndex(const std::string& name)
{
    if (!IsVarDefined(name))
    {
        mBuilder.SetInsertPoint(mDefBlock);
        
        Value* allocaInst = mBuilder.CreateAlloca(llvm::Type::getInt32Ty(mBuilder.getContext()));
        mBuilder.CreateStore(llvm::ConstantInt::get(mBuilder.getContext(), llvm::APInt(32, 0)), allocaInst);

        mNamedVariables[name] = allocaInst;
    }

    return mNamedVariables[name];
}

void ExprIRBuilder::CreateLoop(const Index& idx)
{
    const std::string& idxName = idx.GetName();
    auto idxIt = mNamedVariables.find(idxName);

    if (idxIt == mNamedVariables.end())
    {
        return;
    }

    BasicBlock* headerBlock = BasicBlock::Create(mContext, "HeaderBlock");
    
    BasicBlock* incBlock = BasicBlock::Create(mContext, "IncBlock");
    
    BasicBlock* exitBlock = BasicBlock::Create(mContext, "ExitBlock");
}

Value* ExprIRBuilder::CreateMatrix(const std::string& name)
{
    if (!IsVarDefined(name))
    {
        mBuilder.SetInsertPoint(mDefBlock->getTerminator());
        // TODO
    }

    return mNamedVariables[name];
}

Value* ExprIRBuilder::CreateMatrixAccess(Value* matNode, Value* hIdxNode, Value* wIdxNode)
{
    mBuilder.SetInsertPoint(mExprBlock);
    Value* rowAccessVal = mBuilder.CreateGEP(matNode, hIdxNode);
    return mBuilder.CreateGEP(rowAccessVal, wIdxNode);
}

Value* ExprIRBuilder::CreateOp(Value* lhsVal, Value* rhsVal, Operation op)
{
    mBuilder.SetInsertPoint(mExprBlock);
    switch (op)
    {
    case Operation::ADD:
        return mBuilder.CreateAdd(lhsVal, rhsVal);
    case Operation::DIV:
        return mBuilder.CreateUDiv(lhsVal, rhsVal);
    case Operation::MUL:
        return mBuilder.CreateMul(lhsVal, rhsVal);
    case Operation::SUB:
        return mBuilder.CreateSub(lhsVal, rhsVal);
    default:
        assert(false && "Unsupported binary operation");
        return nullptr;
    }
}

Value* ExprIRBuilder::CreateReturn()
{
    mBuilder.SetInsertPoint(mExprBlock);
    return mBuilder.CreateRetVoid();
}