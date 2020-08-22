#include "handledialog.h"
#include "ui_handledialog.h"

HandleDialog::HandleDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::HandleDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("警情处理");
    ui->contactComboBox->setEditable(true);
    data = nullptr;
}

HandleDialog::~HandleDialog()
{
    delete ui;
    if(data)
    {
        delete data;
    }
}

void HandleDialog::setData(QString p1, QString ph1, QString p2, QString ph2)
{
    people1 = p1;
    people2 = p2;
    phone1 = ph1;
    phone2 = ph2;
    if(!p1.isEmpty())
    {
        ui->contactComboBox->addItem(people1);
    }
    if(!p2.isEmpty())
    {
        ui->contactComboBox->addItem(people2);
    }
    //ui->contactComboBox->addItem("电话未打通");
    ui->phoneLineEdit->setText(phone1);
    ui->reMarklineEdit->setPlaceholderText("备注");
}

void HandleDialog::on_pushButton_clicked()
{
    data = new HandleDialogData();
    data->contackPeople = ui->contactComboBox->lineEdit()->text();
    data->contackPhone = ui->phoneLineEdit->text();
    data->alarmTypeString = ui->comboBox->currentText();
    data->remarks = ui->reMarklineEdit->text();
    accept();
}
