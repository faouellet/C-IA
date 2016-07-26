#include "expr.h"

using namespace DistLang;
using namespace DistLang::impl;
using namespace llvm;

////////// BinOpExprNode //////////

Value* BinOpExprNode::CodeGen(IRBuilder<>& builder) const
{
    Value* lhsVal = mLHSNode->CodeGen(builder);
    Value* rhsVal = mRHSNode->CodeGen(builder);

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
        assert(false && "Unrecognized binary operation");
    }
 
    return binOpVal;
}

////////// Expr //////////

llvm::Value* Expr::GetIR() const
{
    llvm::LLVMContext context;
    llvm::IRBuilder<> builder(context);

    // TODO: Create function
    // TODO: Create loop

    return mExprNode->CodeGen(builder);
}