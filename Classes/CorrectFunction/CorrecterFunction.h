#pragma once

#include "CorrectFunctionsCore.h"

class CorrecterFunction
{
public:
    CorrecterFunction(CorrectFunctionsCore* pKeeper);
    CorrecterFunction(CorrectFunctionsCore* pKeeper, TyCorrectFunction function);
    CorrecterFunction(CorrecterFunction* pCorrecterFunction, TyCorrectFunction function);
    ~CorrecterFunction();

    CorrecterFunction(const CorrecterFunction& source);
    CorrecterFunction(CorrecterFunction&& from) noexcept;

    QByteArray operator()(QByteArray& line);

private:
    TyCorrectFunction StoredFunction;
    CorrectFunctionsCore* CoreFunctionsInstance;
    CorrecterFunction* PNextCorrectFunction = nullptr;

    int calcActions() const;

public:
    int getActionsCount() const;

};

