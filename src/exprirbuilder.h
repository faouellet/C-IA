#ifndef EXPR_IR_BUILDER
#define EXPR_IR_BUILDER

#include "matrix.h"

#include <llvm/IR/IRBuilder.h>

#include <unordered_map>

namespace DistLang
{
    namespace impl
    {
        enum class Operation { ADD, DIV, MUL, SUB };
        
        class ExprIRBuilder
        {
        public:
            ExprIRBuilder();

        public:
            void CreateFunction(const std::string& name);
            void CreateLoop(const Index& idx);

            llvm::Value* CreateIndex(const std::string& name);
            llvm::Value* CreateMatrix(const std::string& name);
            llvm::Value* CreateMatrixAccess(llvm::Value* matNode, llvm::Value* hIdxNode, llvm::Value* wIdxNode);
            llvm::Value* CreateOp(llvm::Value* lhsVal, llvm::Value* rhsVal, Operation op);

        public:
            llvm::Module* ReleaseModule() { return mMod.release(); }

        private:
            bool IsVarDefined(const std::string& name) { return mNamedVariables.find(name) != mNamedVariables.end(); }

        private:
            std::unordered_map<std::string, llvm::Value*> mNamedVariables;
            llvm::LLVMContext mContext;
            llvm::IRBuilder<> mBuilder;
            llvm::BasicBlock* mDefBlock;
            llvm::BasicBlock* mExprBlock;
            llvm::Function* mFunc;
            llvm::StructType* mMatType;
            std::unique_ptr<llvm::Module> mMod;
        };
    }
}

#endif // EXPR_IR_BUILDER
