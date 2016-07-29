#include "expr.h"

#include <llvm/IR/Module.h>

using namespace DistLang;
using namespace DistLang::impl;
using namespace llvm;

////////// BinOpExprNode //////////

void BinOpExprNode::CodeGen(IRBuilder<>& builder) const
{
    mLHSNode->CodeGen(builder);
    Value* lhsVal = &*builder.GetInsertPoint();
    mRHSNode->CodeGen(builder);
    Value* rhsVal = &*builder.GetInsertPoint();

    BasicBlock* currentBlock = builder.GetInsertBlock();
    BasicBlock* binOpBlock = BasicBlock::Create(builder.getContext(), "BinOp", currentBlock->getParent());
    builder.CreateBr(binOpBlock);
    builder.SetInsertPoint(binOpBlock);

    Value* binOpVal = nullptr;
    switch (mOp)
    {
    case Operation::ADD:
        binOpVal = builder.CreateAdd(lhsVal, rhsVal);
        break;
    case Operation::DIV:
        binOpVal = builder.CreateUDiv(lhsVal, rhsVal);
        break;
    case Operation::MUL:
        binOpVal = builder.CreateMul(lhsVal, rhsVal);
        break;
    case Operation::SUB:
        binOpVal = builder.CreateSub(lhsVal, rhsVal);
        break;
    default:
        assert(false && "Unsupported binary operation");
    }
}

////////// Expr //////////

std::unique_ptr<Module> Expr::GetIR() const
{
    ExprIRBuilder builder;

    auto mod = std::make_unique<Module>(builder.GetIR());

    mod->dump();
    
    if (mExprNode != nullptr)
    {
        // TODO: Create loop
        mExprNode->CodeGen(builder);
        return mod;
    }

    return nullptr;
}