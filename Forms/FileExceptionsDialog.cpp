#include "stdafx.h"
#include "FileExceptionsDialog.h"
#include "ui_FileExceptionsDialog.h"

FileExceptionsDialog::FileExceptionsDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::FileExceptionsDialogClass())
{
    ui->setupUi(this);
}

FileExceptionsDialog::~FileExceptionsDialog()
{
    delete ui;
}
