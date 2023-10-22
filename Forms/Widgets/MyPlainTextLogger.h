#pragma once

#include <QPlainTextEdit>

class MyPlainTextLogger  : public QPlainTextEdit
{
    Q_OBJECT

public:
    MyPlainTextLogger(QWidget *parent);
    ~MyPlainTextLogger();

public slots:
    void LogMessage(const QString& message);
    void LogProcessMessage(const QString& message, bool isNewProcess = false);

};
