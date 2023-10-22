#pragma once

#include <QDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class FileExceptionsDialogClass; };
QT_END_NAMESPACE

class FileExceptionsDialog : public QDialog
{
    Q_OBJECT

public:
    FileExceptionsDialog(QWidget *parent = nullptr);
    ~FileExceptionsDialog();

private:
    Ui::FileExceptionsDialogClass *ui;
};
