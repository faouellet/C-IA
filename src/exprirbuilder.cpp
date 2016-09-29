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
    mFunc = Function::Create(fType, Function::ExternalLinkage, name, mMod.get());

    assert(mDefBlock != nullptr);
    mDefBlock->insertInto(mFunc);

    assert(mExprBlock != nullptr);
    mExprBlock->insertInto(mFunc);
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
    // Getting the LLVM value associated with the index.
    // We want to bail out as soon as possible if there's not value.
    const std::string& idxName = idx.GetName();
    auto idxIt = mNamedVariables.find(idxName);

    if (idxIt == mNamedVariables.end())
    {
        return;
    }
    
    // The header block is responsible for deciding if we make another loop trip or if we're done
    BasicBlock* headerBlock = BasicBlock::Create(mContext, "HeaderBlock");
    assert(headerBlock != nullptr);
    headerBlock->insertInto(mFunc);

    // Once the definitions are made, we jump straight to the loop header
    mBuilder.SetInsertPoint(mDefBlock);
    mBuilder.CreateBr(headerBlock);
    
    // The increment block increments all indexes used in the computation.
    // It will then branch unconditionally to the header block.
    BasicBlock* incBlock = BasicBlock::Create(mContext, "IncBlock");
    assert(incBlock != nullptr);
    incBlock->insertInto(mFunc);
    mBuilder.SetInsertPoint(incBlock);
    mBuilder.CreateBr(headerBlock);

    // Once the computation is done, we branch to the incrementation block
    mBuilder.SetInsertPoint(mExprBlock);
    mBuilder.CreateBr(incBlock);

    // Block representing the end of the loop as well as the end of the JIT program
    BasicBlock* exitBlock = BasicBlock::Create(mContext, "ExitBlock");
    assert(exitBlock != nullptr);
    exitBlock->insertInto(mFunc);
    mBuilder.SetInsertPoint(exitBlock);
    mBuilder.CreateRetVoid();

    // From the header we can branch to the loop body or to the exit
    mBuilder.SetInsertPoint(headerBlock);
    //mBuilder.CreateICmpSLE(idxIt->second, )
    //mBuilder.CreateCondBr();
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
