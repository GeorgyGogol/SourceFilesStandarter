#pragma once

#include <QDialog>
#include <map>

QT_BEGIN_NAMESPACE
namespace Ui { class FileExtensionsDialog; };
QT_END_NAMESPACE

class FileExtensionsDialog  : public QDialog
{
    Q_OBJECT

public:
    FileExtensionsDialog(QWidget* parent);
    ~FileExtensionsDialog();

private:
	Ui::FileExtensionsDialog* ui;
    void UpdateSwitchButtonText(QPushButton* pButton, bool state);
    std::map<QCheckBox*, QString> definedExtensionsMap;

    void fillDefinedExtensionsMap();

public slots:
    void on_bb_Control_accepted();
    void on_pb_Cpp_clicked(bool checked);

signals:
    void ApplyExtentionsString(const QString& extentions /*, const QString& spliter*/);
	
};
