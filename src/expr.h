#ifndef EXPR_H
#define EXPR_H

#include "index.h"
#include "utils.h"

#include <llvm/IR/IRBuilder.h>
#include <llvm/IR/Value.h>

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
            MUL,
            SUB
        };

        class ExprNode 
        {
        public:
            virtual ~ExprNode() = default;

        public:
            virtual llvm::Value* CodeGen(llvm::IRBuilder<>& builder) const = 0;
        };

        class BinOpExprNode : public ExprNode 
        {
        public:
            BinOpExprNode(ExprNode* lhsNode, ExprNode* rhsNode, Operation op) :
                mLHSNode{ lhsNode }, mRHSNode{ rhsNode }, mOp{ op } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(llvm::IRBuilder<>& builder) const override;

        private:
            std::shared_ptr<ExprNode> mLHSNode;
            std::shared_ptr<ExprNode> mRHSNode;
            Operation mOp;
        };

        template <typename T>
        class ConstantExprNode : public ExprNode 
        {
            static_assert(std::is_integral<T>::value, "Only integral constants are allowed");

        public:
            ConstantExprNode(const Tensor<T>& tensor) : mConstant{ tensor } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(llvm::IRBuilder<>& builder) const override
            {
                return builder.CreateAlloca(llvm::Type::getInt32Ty(builder.getContext()));
            }

        private:
            Tensor<T> mConstant;
        };

        template <typename T>
        class IndexedExprNode : public ExprNode
        {
            static_assert(std::is_integral<T>::value, "Only integral tensors are allowed");

        public:
            template <typename T, typename... Indexes>
            IndexedExprNode(const Tensor<T>& tensor, const Indexes&... indexes) : mVar{ tensor }, mIndexes{ indexes... } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(llvm::IRBuilder<>& builder) const override
            {
                // TODO: Actual indexing
                return builder.CreateAlloca(llvm::Type::getInt32Ty(builder.getContext()));
            }

        private:
            Tensor<T> mVar;
            std::vector<Index> mIndexes;
        };
    }
    
    class Expr
    {
    public: // Ctors
        template <typename T, typename... Indexes>
        Expr(const Tensor<T>& tensor, const Indexes&... indexes) : mVar{ var }, mIndexes { indexes }
        {
            static_assert(impl::is_all_same<Index, Indexes...>::value, "Only Index objects are allowed");
            static_assert(std::is_integral<T>::value, "Only integral constants are allowed");

            mExprNode.reset(new impl::IndexedExprNode<T>{ tensor, indexes });
        }

        Expr(const Expr& lhsExpr, const Expr& rhsExpr, impl::Operation op)
        {
            mExprNode.reset(new impl::BinOpExprNode{ lhsExpr.mExprNode.get(), rhsExpr.mExprNode.get(), op });
        }
        
    public:
        llvm::Value* GetIR() const;

    private:
        std::shared_ptr<impl::ExprNode> mExprNode;
    };
}

#endif // EXPR_H