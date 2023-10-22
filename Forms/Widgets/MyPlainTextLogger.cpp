#include "stdafx.h"
#include "MyPlainTextLogger.h"

MyPlainTextLogger::MyPlainTextLogger(QWidget *parent)
    : QPlainTextEdit(parent)
{}

MyPlainTextLogger::~MyPlainTextLogger()
{}

void MyPlainTextLogger::LogProcessMessage(const QString & message, bool isNewProcess)
{
    if (isNewProcess) {
        this->appendPlainText(QDateTime::currentDateTime().toString("hh:mm:ss.zzz"));
    }
    this->moveCursor(QTextCursor::End);
    this->insertPlainText(" " + message);
}

void MyPlainTextLogger::LogMessage(const QString& message)
{
    QString logMes =
        QDateTime::currentDateTime().toString("hh:mm:ss.zzz") +
        " " +
        message;
    this->appendPlainText(logMes);
}

