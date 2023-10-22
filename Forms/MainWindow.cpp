#include "stdafx.h"
#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFileDialog>
#include <QSettings>

#include "MqFileCorrecter.h"
#include "FileExtensionsDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindowClass())
    , CorrecterCore(new MqFileCorrecter(this))
    , FileExtensionsSetting(new FileExtensionsDialog(this))
{
    ui->setupUi(this);

    CorrecterCore->setStartCodec("UTF-8");
    CorrecterCore->setEndCodec("CP-1251");

    connect(ui->cb_replaceTabs, &QCheckBox::clicked, CorrecterCore, &MqFileCorrecter::setNeedCorrectTabs);
    connect(ui->cb_cleanEndSpaces, &QCheckBox::clicked, CorrecterCore, &MqFileCorrecter::setCleanEndSpaces);
    connect(CorrecterCore, &MqFileCorrecter::ProgressMessage, ui->LogTextEdit, &MyPlainTextLogger::LogMessage);
    connect(CorrecterCore, &MqFileCorrecter::ProgressMax, ui->progressBar, &QProgressBar::setMaximum);
    connect(CorrecterCore, &MqFileCorrecter::ProgressStep, ui->progressBar, &QProgressBar::setValue);
    connect(
        ui->bRun, &QPushButton::clicked, 
        this, [=] {
            CorrecterCore->Run(ui->EditSourcePath->text());
        }
    );

    connect(ui->bSetFileExtention, &QPushButton::clicked, FileExtensionsSetting, &FileExtensionsDialog::exec);
    connect(FileExtensionsSetting, &FileExtensionsDialog::ApplyExtentionsString, this, &MainWindow::setExtentions);

    

}

MainWindow::~MainWindow()
{
    delete ui;
    delete CorrecterCore;
    delete FileExtensionsSetting;
}

void MainWindow::on_bOpenSourceDirDialog_clicked()
{
    QString dir = QFileDialog::getExistingDirectory(this, "Открыть папку проекта");
    if (dir.isEmpty()) return;
    ui->EditSourcePath->setText(dir);
}

void MainWindow::on_action_Save_triggered()
{
    QString saveFile = QFileDialog::getSaveFileName(this, "Сохранить", QString(), "Conf (*.ini)");
    if (saveFile.isEmpty()) return;

    QSettings settings(saveFile, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QTextCodec* utf = QTextCodec::codecForName("UTF-8");
    
    settings.clear();
    
    settings.beginGroup("General");
    if (!ui->FileExtentionsLine->text().isEmpty()) {
        settings.setValue("Extentions", ui->FileExtentionsLine->text());
    }
    if (!ui->EditSourcePath->text().isEmpty()) {
        settings.setValue("DirPath", utf->fromUnicode(ui->EditSourcePath->text()).constData());
    }
    settings.endGroup();

    settings.beginGroup("Additional");
    settings.setValue("ReplaceTabs", ui->cb_replaceTabs->isChecked());
    settings.setValue("CleanEndSpaces", ui->cb_cleanEndSpaces->isChecked());
    settings.endGroup();
    
    ui->LogTextEdit->LogMessage("Saved: " + saveFile);
}

void MainWindow::on_action_Load_triggered()
{
    QString saveFile = QFileDialog::getOpenFileName(this, "Открыть", QString(), "Conf (*.ini)");
    if (saveFile.isEmpty()) return;

    QSettings settings(saveFile, QSettings::IniFormat);
    settings.setIniCodec("UTF-8");
    QTextCodec* utf = QTextCodec::codecForName("UTF-8");
    QVariant tmpValue;

    settings.beginGroup("General");
    if (settings.contains("Extentions")) {
        tmpValue = settings.value("Extentions");
        setExtentions(tmpValue.toString());
    }

    if (settings.contains("DirPath")) {
        tmpValue = settings.value("DirPath");
        ui->EditSourcePath->setText(utf->toUnicode(tmpValue.toByteArray()));
    }

    settings.endGroup();

    settings.beginGroup("Additional");
    ui->cb_replaceTabs->setChecked(settings.value("ReplaceTabs").toBool());
    CorrecterCore->setNeedCorrectTabs(settings.value("ReplaceTabs").toBool());

    ui->cb_cleanEndSpaces->setChecked(settings.value("CleanEndSpaces").toBool());
    CorrecterCore->setCleanEndSpaces(settings.value("CleanEndSpaces").toBool());
    settings.endGroup();

    ui->LogTextEdit->LogMessage("Loaded: " + saveFile);
}

void MainWindow::setExtentions(const QString& extentionsStr)
{
    ui->FileExtentionsLine->setText(extentionsStr);
    CorrecterCore->setExtentions(extentionsStr);
}



