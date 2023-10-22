#pragma once

#include <QString>
#include <QByteArray>

class CorrectFunctionsCore
{
public:
    CorrectFunctionsCore(QTextCodec* pCodec);
    QTextCodec* StartCodec = nullptr;
    QTextCodec* EndCodec = nullptr;

private:
    //QString& Line;
    QByteArray getOneIndentInSpaces();

public:
    void Pass(QByteArray&);
    void ChangeCodepage(QByteArray& str);

    void ReplaceTabs(QByteArray& str);
    void CleanEndSpaces(QByteArray& str);


};

typedef void (CorrectFunctionsCore::*TyCorrectFunction)(QByteArray&);

