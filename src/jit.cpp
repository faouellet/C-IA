#include "jit.h"

#include <llvm/ExecutionEngine/ExecutionEngine.h>
#include <llvm/ExecutionEngine/Orc/CompileUtils.h>
#include <llvm/ExecutionEngine/Orc/IndirectionUtils.h>
#include <llvm/ExecutionEngine/Orc/JITSymbol.h>
#include <llvm/ExecutionEngine/Orc/LambdaResolver.h>
#include <llvm/ExecutionEngine/SectionMemoryManager.h>
#include <llvm/IR/Mangler.h>
#include <llvm/Support/DynamicLibrary.h>
#include <llvm/Support/raw_ostream.h>
#include <llvm/Target/TargetMachine.h>

#include <set>

using namespace DistLang;
using namespace llvm;
using namespace llvm::orc;

JITCompiler::JITCompiler() : mTarget{ EngineBuilder().selectTarget() }, 
                             mLayout{ mTarget->createDataLayout() }, 
                             mCompileCallbackManager{ createLocalCompileCallbackManager(mTarget->getTargetTriple(), 0) },
                             mCompilerLayer{ mObjectLayer, SimpleCompiler{*mTarget} },
                             mCODLayer{ mCompilerLayer, 
                                        [this](Function& func) { return std::set<Function*>{&func}; }, 
                                        *mCompileCallbackManager, 
                                        createLocalIndirectStubsManagerBuilder(mTarget->getTargetTriple()) } 
{
    sys::DynamicLibrary::LoadLibraryPermanently(nullptr);
}


JITCompiler::ModuleHandle JITCompiler::AddModule(std::unique_ptr<Module> module)
{
    auto resolver = createLambdaResolver(
        [&](const std::string& name) 
        {
            if (auto sym = mCODLayer.findSymbol(name, false))
            {
                return sym.toRuntimeDyldSymbol();
            }
            return RuntimeDyld::SymbolInfo(nullptr);
        },
        [](const std::string &name) 
        {
            if (auto symAddr = RTDyldMemoryManager::getSymbolAddressInProcess(name))
            {
                return RuntimeDyld::SymbolInfo(symAddr, JITSymbolFlags::Exported);
            }
            return RuntimeDyld::SymbolInfo(nullptr);
        });

    std::vector<std::unique_ptr<Module>> modules;
    modules.push_back(std::move(module));

    return mCODLayer.addModuleSet(std::move(modules), make_unique<SectionMemoryManager>(), std::move(resolver));
}

JITSymbol JITCompiler::FindSymbol(const std::string& name)
{
    std::string mangledName;
    raw_string_ostream mangledNameStream(mangledName);
    Mangler::getNameWithPrefix(mangledNameStream, mangledName, mLayout);
    return mCODLayer.findSymbol(mangledNameStream.str(), true);
}
