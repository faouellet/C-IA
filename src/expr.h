#ifndef EXPR_H
#define EXPR_H

#include <memory>
#include <vector>

namespace llvm
{
    class Value;
}

namespace DistLang
{
    class Index;
    class Matrix;
    
    namespace impl
    {
        enum class Operation;
        
        class ExprIRBuilder;

        class ExprNode 
        {
        public:
            virtual ~ExprNode() = default;

        public:
            virtual llvm::Value* CodeGen(ExprIRBuilder& builder) const = 0;
        };

        class BinOpExprNode : public ExprNode 
        {
        public:
            BinOpExprNode(ExprNode* lhsNode, ExprNode* rhsNode, Operation op) :
                mLHSNode{ lhsNode }, mRHSNode{ rhsNode }, mOp{ op } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(ExprIRBuilder& builder) const override;

        private:
            std::shared_ptr<ExprNode> mLHSNode;
            std::shared_ptr<ExprNode> mRHSNode;
            Operation mOp;
        };

        class IndexExprNode : public ExprNode 
        {
        public:
            IndexExprNode(const std::string& name) : mName{ name } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(ExprIRBuilder& builder) const override;

        private:
            std::string mName;
        };

        class MatrixExprNode : public ExprNode
        {
        public:
            MatrixExprNode(const std::string& name) : mName{ name } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(ExprIRBuilder& builder) const override;

        private:
            std::string mName;
        };

        class IndexedExprNode : public ExprNode
        {
        public:
            IndexedExprNode(std::unique_ptr<ExprNode>&& matNode, std::unique_ptr<ExprNode>&& hIdxNode, std::unique_ptr<ExprNode>&& wIdxNode)
                : mMatNode{ std::move(matNode) }, mHIdxNode{ std::move(hIdxNode) }, mWIdxNode{ std::move(wIdxNode) } { }

        public: // Expr interface
            virtual llvm::Value* CodeGen(ExprIRBuilder& builder) const override;

        private:
            std::unique_ptr<ExprNode> mMatNode;
            std::unique_ptr<ExprNode> mHIdxNode;
            std::unique_ptr<ExprNode> mWIdxNode;
        };
    }
    
    class Expr
    {
    public: // Ctors
        Expr() = default;
        Expr(const Matrix& matrix, const Index& hIdx, const Index& wIdx);
        Expr(const Matrix& matrix);
        Expr(const Expr& lhsExpr, const Expr& rhsExpr, impl::Operation op);
        
    public:
        void CodeGen(impl::ExprIRBuilder& builder) const;

    private:
        std::unique_ptr<impl::ExprNode> mExprNode;
    };
}

#endif // EXPR_H