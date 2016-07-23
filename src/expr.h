#ifndef EXPR_H
#define EXPR_H

#include "index.h"
#include "utils.h"

#include <memory>
#include <vector>

namespace DistLang
{
    template <typename T> class Tensor;
    
    namespace impl
    {
        enum class Operation
        {
            ADD,
            DIV,
            MULT,
            SUB
        };

        class ExprNode { };

        class BinOpExprNode : public ExprNode 
        {
        public:
            BinOpExprNode(ExprNode* lhsNode, ExprNode* rhsNode, Operation op) :
                mLHSNode{ lhsNode }, mRHSNode{ rhsNode }, mOp{ op } { }

        private:
            std::shared_ptr<ExprNode> mLHSNode;
            std::shared_ptr<ExprNode> mRHSNode;
            Operation mOp;
        };

        template <typename T>
        class ConstantExprNode : public ExprNode 
        {
        public:
            ConstantExprNode(const Tensor<T>& tensor) : mConstant{ tensor } { }

        private:
            Tensor<T> mConstant;
        };

        template <typename T>
        class IndexedExprNode : public ExprNode
        {
        public:
            template <typename T, typename... Indexes>
            IndexedExprNode(const Tensor<T>& tensor, const Indexes&... indexes) : mVar{ tensor }, mIndexes{ indexes... } { }

        private:
            Tensor<T> mVar;
            std::vector<Index> mIndexes;
        };
    }
    
    class Expr
    {
    public:
        template <typename T, typename... Indexes>
        Expr(const Tensor<T>& tensor, const Indexes&... indexes) : mVar{ var }, mIndexes { indexes }
        {
            static_assert(impl::is_all_same<Index, Indexes...>::value, "Only Index objects are allowed");
            mExprNode.reset(new impl::IndexedExprNode<T>{ tensor, indexes });
        }

        Expr(const Expr& lhsExpr, const Expr& rhsExpr, impl::Operation op)
        {
            mExprNode.reset(new impl::BinOpExprNode{ lhsExpr.mExprNode.get(), rhsExpr.mExprNode.get(), op });
        }

    private:
        std::shared_ptr<impl::ExprNode> mExprNode;
    };
}

#endif // EXPR_H