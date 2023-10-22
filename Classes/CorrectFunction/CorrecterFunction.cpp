#include "stdafx.h"
#include "CorrecterFunction.h"

#include "CorrectFunctionsCore.h"

CorrecterFunction::CorrecterFunction(CorrectFunctionsCore* pKeeper)
    : CoreFunctionsInstance(pKeeper), StoredFunction(&CorrectFunctionsCore::Pass)
{
}

CorrecterFunction::CorrecterFunction(CorrectFunctionsCore* pKeeper, TyCorrectFunction function)
    : CoreFunctionsInstance(pKeeper), StoredFunction(function)
{
}

CorrecterFunction::CorrecterFunction(CorrecterFunction* pCorrecterFunction, TyCorrectFunction function)
    : CoreFunctionsInstance(pCorrecterFunction->CoreFunctionsInstance), StoredFunction(function), PNextCorrectFunction(pCorrecterFunction)
{
}

CorrecterFunction::~CorrecterFunction()
{
    if (PNextCorrectFunction) {
        delete PNextCorrectFunction;
        PNextCorrectFunction = nullptr;
    }
}

CorrecterFunction::CorrecterFunction(const CorrecterFunction& source)
{
    StoredFunction = source.StoredFunction;
    CoreFunctionsInstance = source.CoreFunctionsInstance;
    if (source.PNextCorrectFunction) {
        PNextCorrectFunction = new CorrecterFunction(*source.PNextCorrectFunction);
    }
}

CorrecterFunction::CorrecterFunction(CorrecterFunction&& from) noexcept
{
    StoredFunction = from.StoredFunction;
    CoreFunctionsInstance = from.CoreFunctionsInstance;
    if (from.PNextCorrectFunction) {
        PNextCorrectFunction = from.PNextCorrectFunction;
        from.PNextCorrectFunction = nullptr;
    }
}

QByteArray CorrecterFunction::operator()(QByteArray& line)
{
    (CoreFunctionsInstance->*StoredFunction)(line);
    if (PNextCorrectFunction) (*PNextCorrectFunction)(line);
    return line;
}

int CorrecterFunction::calcActions() const
{
    if (PNextCorrectFunction == nullptr) {
        return StoredFunction == &CorrectFunctionsCore::Pass ? 0 : 1;
    }
    if (StoredFunction == &CorrectFunctionsCore::Pass) return PNextCorrectFunction->calcActions();
    return PNextCorrectFunction->calcActions() + 1;
}

int CorrecterFunction::getActionsCount() const
{
    return calcActions();
}

