#pragma once

#include <QtWidgets/QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class MqFileCorrecter;
class FileExtensionsDialog;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindowClass *ui;
    MqFileCorrecter* CorrecterCore;
    FileExtensionsDialog* FileExtensionsSetting;

public:

public slots:
    //void on_bSetFileExtention_clicked();
    void on_bOpenSourceDirDialog_clicked();
    void on_action_Save_triggered();
    void on_action_Load_triggered();

    void setExtentions(const QString& extentionsStr);

};

