#include "stdafx.h"
#include "FileExtensionsDialog.h"
#include "ui_FileExtensionsDialog.h"

FileExtensionsDialog::FileExtensionsDialog(QWidget* parent)
    : QDialog(parent)
	, ui(new Ui::FileExtensionsDialog())
{
	ui->setupUi(this);
    fillDefinedExtensionsMap();
}

FileExtensionsDialog::~FileExtensionsDialog()
{
	delete ui;
}

void FileExtensionsDialog::UpdateSwitchButtonText(QPushButton* pButton, bool state)
{
    if (state) {
        pButton->setText("Исключить");
    }
    else {
        pButton->setText("Включить");
    }
}

void FileExtensionsDialog::fillDefinedExtensionsMap()
{
    definedExtensionsMap[ui->ext_h] = "*.h";
    definedExtensionsMap[ui->ext_cpp] = "*.cpp";
    definedExtensionsMap[ui->ext_txt] = "*.txt";
}

void FileExtensionsDialog::on_bb_Control_accepted()
{
    QStringList extIncluded = QStringList();

    for (auto preDefined : definedExtensionsMap)
    {
        if (!preDefined.first->isChecked()) continue;
        extIncluded.append(preDefined.second);
    }

    if (!ui->le_CustomExtentions->text().isEmpty()) {
        QStringList customExtList = ui->le_CustomExtentions->text().split(',');
        for (auto& each : customExtList) {
            if (each.length() < 1) continue;
            each = each.trimmed();
            if (each[0] != "*") each = "*" + each;

            if (!extIncluded.contains(each, Qt::CaseInsensitive)) extIncluded.append(each);
        }
    }

    emit ApplyExtentionsString(extIncluded.join(", "));
}

void FileExtensionsDialog::on_pb_Cpp_clicked(bool checked)
{
    UpdateSwitchButtonText(ui->pb_Cpp, checked);
}

