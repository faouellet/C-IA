#include "expr.h"

#include "exprirbuilder.h"
#include "index.h"

#include <llvm/IR/Module.h>

using namespace DistLang;
using namespace DistLang::impl;
using namespace llvm;

////////// BinOpExprNode //////////

Value* BinOpExprNode::CodeGen(ExprIRBuilder& builder) const
{
    Value* lhsVal = mLHSNode->CodeGen(builder);
    Value* rhsVal = mRHSNode->CodeGen(builder);
    
    return builder.CreateOp(lhsVal, rhsVal, mOp);
}

////////// IndexExprNode //////////

Value* IndexExprNode::CodeGen(ExprIRBuilder& builder) const 
{
    return builder.CreateIndex(mName); 
}

////////// MatrixExprNode //////////

Value* MatrixExprNode::CodeGen(ExprIRBuilder& builder) const 
{ 
    return builder.CreateIndex(mName); 
}

////////// IndexedExprNode //////////

Value* IndexedExprNode::CodeGen(ExprIRBuilder& builder) const
{
    Value* matValue = mMatNode->CodeGen(builder);
    Value* hIdxValue = mHIdxNode->CodeGen(builder);
    Value* wIdxValue = mWIdxNode->CodeGen(builder);

    return builder.CreateMatrixAccess(matValue, hIdxValue, wIdxValue);
}

////////// Expr //////////

Expr::Expr(const Matrix& matrix, const Index& hIdx, const Index& wIdx)
{
    std::unique_ptr<ExprNode> matNode{ new impl::IndexExprNode{ matrix.GetName() } };
    std::unique_ptr<ExprNode> hIdxNode{ new impl::IndexExprNode{ hIdx.GetName() } };
    std::unique_ptr<ExprNode> wIdxNode{ new impl::IndexExprNode{ wIdx.GetName() } };

    mExprNode.reset(new impl::IndexedExprNode{ std::move(matNode), std::move(hIdxNode), std::move(wIdxNode) });
}

Expr::Expr(const Matrix& matrix)
{
    mExprNode.reset(new impl::MatrixExprNode{ matrix.GetName() });
}

Expr::Expr(const Expr& lhsExpr, const Expr& rhsExpr, Operation op)
{
    mExprNode.reset(new impl::BinOpExprNode{ lhsExpr.mExprNode.get(), rhsExpr.mExprNode.get(), op });
}

void Expr::CodeGen(ExprIRBuilder& builder) const
{
    if (mExprNode != nullptr)
    {
        mExprNode->CodeGen(builder);
    }
}

Expr DistLang::operator+(const Expr& lhs, const Expr& rhs) { return{ lhs, rhs, impl::Operation::ADD }; }
Expr DistLang::operator/(const Expr& lhs, const Expr& rhs) { return{ lhs, rhs, impl::Operation::DIV }; }
Expr DistLang::operator*(const Expr& lhs, const Expr& rhs) { return{ lhs, rhs, impl::Operation::MUL }; }
Expr DistLang::operator-(const Expr& lhs, const Expr& rhs) { return{ lhs, rhs, impl::Operation::SUB }; }
