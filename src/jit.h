#ifndef JIT_H
#define JIT_H

#include <llvm/ExecutionEngine/Orc/CompileOnDemandLayer.h>
#include <llvm/ExecutionEngine/Orc/IRCompileLayer.h>
#include <llvm/ExecutionEngine/Orc/ObjectLinkingLayer.h>
#include <llvm/IR/DataLayout.h>

#include <memory>
#include <string>

namespace llvm
{
    class JITSymbol;
    class TargetMachine;

    namespace orc
    {
        class JITCompileCallbackManager;
    }
}

namespace DistLang
{
    class JITCompiler
    {
    public:
        static JITCompiler& GetInstance()
        {
            static JITCompiler instance;
            return instance;
        }

    private:
        JITCompiler();
        JITCompiler(const JITCompiler&) = delete;
        JITCompiler& operator=(const JITCompiler&) = delete;

    private:
        std::unique_ptr<llvm::TargetMachine> mTarget;
        const llvm::DataLayout mLayout;
        std::unique_ptr<llvm::orc::JITCompileCallbackManager> mCompileCallbackManager;
        llvm::orc::ObjectLinkingLayer<> mObjectLayer;
        llvm::orc::IRCompileLayer<decltype(mObjectLayer)> mCompilerLayer;
        llvm::orc::CompileOnDemandLayer<decltype(mCompilerLayer)> mCODLayer;

    public:
        using ModuleHandle = decltype(mCODLayer)::ModuleSetHandleT;

    public:
        ModuleHandle AddModule(std::unique_ptr<llvm::Module> module);
        llvm::JITSymbol FindSymbol(const std::string& name);

    };
}

#endif // JIT_H