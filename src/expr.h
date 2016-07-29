#ifndef EXPR_H
#define EXPR_H

#include "exprirbuilder.h"
#include "index.h"
#include "utils.h"

#include <memory>
#include <vector>

namespace DistLang
{
    template <typename T> class Matrix;
    
    namespace impl
    {
        enum class Operation { ADD, DIV, MUL, SUB };

        class ExprNode 
        {
        public:
            virtual ~ExprNode() = default;

        public:
            virtual void CodeGen(ExprIRBuilder& builder) const = 0;
        };

        class BinOpExprNode : public ExprNode 
        {
        public:
            BinOpExprNode(ExprNode* lhsNode, ExprNode* rhsNode, Operation op) :
                mLHSNode{ lhsNode }, mRHSNode{ rhsNode }, mOp{ op } { }

        public: // Expr interface
            virtual void CodeGen(ExprIRBuilder& builder) const override;

        private:
            std::shared_ptr<ExprNode> mLHSNode;
            std::shared_ptr<ExprNode> mRHSNode;
            Operation mOp;
        };

        template <typename T>
        class ConstantExprNode : public ExprNode 
        {
        public:
            ConstantExprNode(const Matrix<T>& tensor) : mConstant{ tensor } { }

        public: // Expr interface
            virtual void CodeGen(ExprIRBuilder& builder) const override
            {
                builder.CreateAlloca(llvm::Type::getInt32Ty(builder.getContext()));
            }

        private:
            Matrix<T>& mConstant;
        };

        template <typename T>
        class IndexedExprNode : public ExprNode
        {
        public:
            template <typename T, typename... Indexes>
            IndexedExprNode(const Matrix<T>& tensor, const Indexes&... indexes) : mVar{ tensor }, mIndexes{ indexes... } { }

        public: // Expr interface
            virtual void CodeGen(ExprIRBuilder& builder) const override
            {
                std::vector<llvm::Value*> indexValues;
                for (int iDim = 0; iDim < mIndexes.size(); ++iDim)
                {
                    indexValues.emplace_back(builder.CreateAlloca(llvm::Type::getInt32Ty(builder.getContext())));
                    builder.CreateStore(indexValues.back(), llvm::ConstantInt::get(builder.getContext(), llvm::APInt(32, 0)));
                }
            }

        private:
            const Matrix<T>& mVar;
            std::vector<Index> mIndexes;
        };
    }
    
    class Expr
    {
    public: // Ctors
        Expr() = default;

        template <typename T, typename... Indexes>
        Expr(const Matrix<T>& matrix, const Indexes&... indexes)
        {
            static_assert(impl::is_all_same<Index, Indexes...>::value, "Index objects are required for indexing");
            static_assert(std::is_integral<T>::value, "Only integral matrices are allowed");

            mExprNode.reset(new impl::IndexedExprNode<T>{ matrix, indexes... });
        }

        template <typename T>
        Expr(const Matrix<T>& matrix)
        {
            static_assert(std::is_integral<T>::value, "Only integral matrices are allowed");

            mExprNode.reset(new impl::ConstantExprNode<T>{ matrix });
        }

        Expr(const Expr& lhsExpr, const Expr& rhsExpr, impl::Operation op)
        {
            mExprNode.reset(new impl::BinOpExprNode{ lhsExpr.mExprNode.get(), rhsExpr.mExprNode.get(), op });
        }
        
    public:
        std::unique_ptr<llvm::Module> GetIR() const;

    private:
        std::shared_ptr<impl::ExprNode> mExprNode;
    };
}

#endif // EXPR_H