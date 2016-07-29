#ifndef EXPR_IR_BUILDER
#define EXPR_IR_BUILDER

#include "matrix.h"

#include <llvm/IR/IRBuilder.h>

#include <unordered_map>

namespace DistLang
{
    class Index;

    namespace impl
    {
        class ExprIRBuilder
        {
        public:
            ExprIRBuilder();

        public:
            void AddIndex(const Index& i);
            
            template <typename T>
            void AddMatrix(const Matrix<T>& mat)
            {
                if (!IsVarDefined(mat.GetName()))
                {
                    mBuilder.SetInsertPoint(mDefBlock->getTerminator());
                    // TODO
                }
            }

            llvm::Module* GetIR() { return mMod.release(); }

        private:
            bool IsVarDefined(const std::string& name) { return mNamedVariables.find(name) != mNamedVariables.end(); }

        private:
            std::unordered_map<std::string, llvm::Value*> mNamedVariables;
            llvm::LLVMContext mContext;
            llvm::IRBuilder<> mBuilder;
            llvm::BasicBlock* mDefBlock;
            llvm::BasicBlock* mExprBlock;
            std::unique_ptr<llvm::Module> mMod;
        };
    }
}

#endif // EXPR_IR_BUILDER
