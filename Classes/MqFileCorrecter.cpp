#include "stdafx.h"
#include "MqFileCorrecter.h"

#include <QFile>
#include <QTextCodec>
#include <QDir>
#include <QDirIterator>
#include <memory>

#include "CorrecterFunction.h"
#include "CorrectFunctionsCore.h"

MqFileCorrecter::MqFileCorrecter(QObject* parent)
    : QObject(parent)
{
}

bool MqFileCorrecter::canRun() const noexcept
{
    bool canRun =
        StartCodec && EndCodec // Codecs
        ;

    return canRun;
}

void MqFileCorrecter::normalizeFileExtentions()
{
    for (auto& each : FileExtentions) {
        each = each.trimmed();
        if (each[0] == "*") continue;
        each = "*" + each;
    }
}

void MqFileCorrecter::Rewrite(const QString& fileName, CorrecterFunction& CorrectLineFunction)
{
    QFile file(fileName);
    QFile tmpFile(fileName + ".convert");

    // TODO: correct-tempFile

    file.open(QIODevice::ReadOnly);
    tmpFile.open(QIODevice::WriteOnly);
    while (!file.atEnd()) {
        QByteArray ss = file.readLine();
        QByteArray correct = CorrectLineFunction(ss);
        tmpFile.write(correct);
    }
    file.close();
    tmpFile.close();

    file.remove();
    tmpFile.rename(fileName);

    //emit ProgressStep();
}

bool MqFileCorrecter::isCorrectCodepage(const QString& fileName, QTextCodec* needCodepage)
{
    if (!canRun()) {
        throw std::exception("Can't run, check Codecs settings");
    }

    bool isUtf = true;
    QTextCodec* utfCodec = QTextCodec::codecForName("UTF-8");

    QFile file(fileName);

    file.open(QIODevice::ReadOnly);
    QByteArray bArray;
    QTextCodec::ConverterState codecState;
    while (!file.atEnd() && isUtf)
    {
        bArray = file.readLine();
        if (bArray.size() < 3 && bArray == "\r\n") continue;
        utfCodec->toUnicode(bArray.constData(), bArray.size(), &codecState);
        isUtf = codecState.invalidChars == 0;
    }
    file.close();

    if (needCodepage == utfCodec)
        return isUtf;
    else
        return !isUtf;
}

void MqFileCorrecter::addExtention(const QString& extention)
{
    FileExtentions.push_back(extention);
}

void MqFileCorrecter::resetExtentions()
{
    FileExtentions.clear();
}

int MqFileCorrecter::countFiles(const QString& dirPath)
{
    QDirIterator dir(dirPath, FileExtentions, getDirFilters(), QDirIterator::Subdirectories);
    int out = 0;

    while (dir.hasNext())
    {
        dir.next();
        if (dir.fileInfo().isDir()) continue;
        out++;
    }

    return out;
}

void MqFileCorrecter::setExtentions(const QString& extentionsStr, const QString& separator)
{
    FileExtentions = extentionsStr.split(separator, QString::SkipEmptyParts);
    normalizeFileExtentions();
}

void MqFileCorrecter::setStartCodec(const QString& codecName)
{
    QByteArray codePageName = codecName.toUtf8();
    StartCodec = QTextCodec::codecForName(codePageName);
}

void MqFileCorrecter::setEndCodec(const QString& codecName)
{
    QByteArray codePageName = codecName.toUtf8();
    EndCodec = QTextCodec::codecForName(codePageName);
}

void MqFileCorrecter::Run(const QString& sourcePath)
{
    if (sourcePath.isEmpty()) {
        emit ProgressMessage("Source Path is invalid!");
        return;
    }

    if (!canRun()) {
        emit ProgressMessage("Cannot run!");
        return;
    }

    //normalizeFileExtentions();

    emit ProgressMessage("Started");
    emit ProgressMax(countFiles(sourcePath));
    emit ProgressStep(0);
    int progress = 0;

    std::unique_ptr<CorrectFunctionsCore> pFunCoreInstance ( new CorrectFunctionsCore(EndCodec) );
    std::unique_ptr<CorrecterFunction> pBaseActions;
    {
        CorrecterFunction* pRawBaseActions = new CorrecterFunction(pFunCoreInstance.get());
        if (ReplaceTabs) pRawBaseActions = new CorrecterFunction(pRawBaseActions, &CorrectFunctionsCore::ReplaceTabs);
        if (CleanEndSpaces) pRawBaseActions = new CorrecterFunction(pRawBaseActions, &CorrectFunctionsCore::CleanEndSpaces);
        pBaseActions = std::make_unique<CorrecterFunction>(*pRawBaseActions);
        delete pRawBaseActions;
    }

    QDirIterator dir(sourcePath, FileExtentions, getDirFilters(), QDirIterator::Subdirectories);

    QFileInfo currFileInfo;
    CorrecterFunction* pRealActions;
    bool isAlreadyEndCode;
    while (dir.hasNext()) {
        currFileInfo = dir.next();
        if (currFileInfo.isDir()) continue;

        if (currFileInfo.suffix() == "convert") {
            emit ProgressMessage(currFileInfo.fileName() + " - Temp, deleted and passed");
            QFile tmp_file(currFileInfo.absoluteFilePath());
            tmp_file.remove();
            emit ProgressStep(++progress);
            continue;
        }

        QString filePath = currFileInfo.absoluteFilePath();
        QString workMessage = currFileInfo.absoluteFilePath() + "...";
        pRealActions = new CorrecterFunction(*pBaseActions);

        isAlreadyEndCode = isCorrectCodepage(filePath, EndCodec);

        if (!isAlreadyEndCode) {
            workMessage += "UTF-8";
            pRealActions = new CorrecterFunction(pRealActions, &CorrectFunctionsCore::ChangeCodepage);
        }
        else {
            workMessage += "Correct";
        }

        if (pRealActions->getActionsCount() > 0) Rewrite(filePath, *pRealActions);
        delete pRealActions;
        pRealActions = nullptr;

        emit ProgressStep(++progress);
        emit ProgressMessage(workMessage);
    }

    emit ProgressMessage("Ended");
}

void MqFileCorrecter::setNeedCorrectTabs(bool isNeed)
{
    ReplaceTabs = isNeed;
}

void MqFileCorrecter::setCleanEndSpaces(bool isNeed)
{
    CleanEndSpaces = isNeed;
}

