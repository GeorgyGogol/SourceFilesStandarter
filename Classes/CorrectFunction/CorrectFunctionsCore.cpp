#include "stdafx.h"
#include "CorrectFunctionsCore.h"

#include <QTextCodec>

CorrectFunctionsCore::CorrectFunctionsCore(QTextCodec* pCodec)
    : EndCodec(pCodec)
{
    StartCodec = QTextCodec::codecForName("UTF-8");
}

QByteArray CorrectFunctionsCore::getOneIndentInSpaces()
{
    return QByteArray("    ");
}

void CorrectFunctionsCore::Pass(QByteArray&)
{
}

void CorrectFunctionsCore::ReplaceTabs(QByteArray& str)
{
    int pos = 0;
    while (str.at(pos) == '\t')
    {
        ++pos;
    }

    if (pos < 1) return;

    QByteArray spaces = getOneIndentInSpaces();

    if (pos > 1) {
        QByteArray oneSpace = spaces;
        for (int i = 1; i < pos; ++i) spaces += oneSpace;
    }

    str.replace(0, pos, spaces);
}

void CorrectFunctionsCore::CleanEndSpaces(QByteArray& str)
{
    const int ending = 2;
    if (str.size() < ending + 1) return;
    if (str.at(str.size() - ending - 1) != ' ') return;
    
    int lastCharacter = str.size() - ending - 1;

    while (lastCharacter >= 0 && str.at(lastCharacter) == ' ')
    {
        str.remove(lastCharacter, 1);
        lastCharacter--;
    }
}

void CorrectFunctionsCore::ChangeCodepage(QByteArray& str)
{
    QString tmp = StartCodec->toUnicode(str);
    str = EndCodec->fromUnicode(tmp);
}

