#pragma once

#include <QObject>

class CorrecterFunction;

class MqFileCorrecter : public QObject
{
    Q_OBJECT

public:
    MqFileCorrecter(QObject* parent = nullptr);

private:
    //QString SourcePath = "";
    QStringList FileExtentions = QStringList();
    QTextCodec* StartCodec = nullptr;
    QTextCodec* EndCodec = nullptr;
    bool ReplaceTabs = false;
    bool CleanEndSpaces = false;

    bool canRun() const noexcept;
    
    [[deprecated("I think, that this method is unnecessary")]]
    void normalizeFileExtentions();
    
    inline QDir::Filters getDirFilters() const noexcept {
        return QDir::Files | QDir::NoSymLinks | QDir::AllDirs | QDir::NoDotAndDotDot;
    }

    void Rewrite(const QString& fileName, CorrecterFunction& CorrectLineFunction);

public:
    bool isCorrectCodepage(const QString& fileName, QTextCodec* needCodepage);
    void addExtention(const QString& extention);
    void resetExtentions();
    int countFiles(const QString& dirPath);

public slots:
    void setExtentions(const QString& extentionsStr, const QString& separator = ",");
    void setStartCodec(const QString& codecName);
    void setEndCodec(const QString& codecName);
    void Run(const QString& sourcePath /* = QString()*/ );
    void setNeedCorrectTabs(bool isNeed);
    void setCleanEndSpaces(bool isNeed);

signals:
    void ProgressMessage(const QString& message);
    //void sendProcessMessage(const QString& message, bool isNewProcess = false);
    void ProgressMax(int value);
    void ProgressStep(int value);

};

