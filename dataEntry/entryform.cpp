#include "entryform.h"
#include "ui_entryform.h"
#include <QSqlRecord>
#include <QList>

QList<int> alarmTypeList = {0x1122,0x1123,0x1110,0x1151,0x1130};
EntryForm::EntryForm(QWidget *parent,QSqlTableModel *m,int n,FORM_TYPE t) :
    QDialog(parent),
    ui(new Ui::EntryForm),
    modle(m),
    num(n),
    type(t)
{
    ui->setupUi(this);
    ui->typeLabel->hide();
    ui->typeComboBox->hide();
    setWindowTitle("分区信息");
    setAttribute(Qt::WA_DeleteOnClose);
    initIdList();
    modle->select();
    mapper = new QDataWidgetMapper(this);
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(modle);
    mapper->addMapping(ui->numLineEdit,NUM);
    mapper->addMapping(ui->nameLineEdit,NAME);
    mapper->addMapping(ui->firtPepleLineEdit,PERSON_1_NAME);
    mapper->addMapping(ui->firstlPhoneLineEdit,PERSON_1_PHONE);
    mapper->addMapping(ui->secPepleLineEdit,PERSON_2_NAME);
    mapper->addMapping(ui->secPhonelineEdit,PERSON_2_PHONE);
    if(type == FORM_TYPE::ALARM_FORM)
    {
        ui->label->setText("防区号");
        ui->label_4->setText("防区名称");
        ui->typeLabel->show();
        ui->typeComboBox->show();
        this->setWindowTitle("防区信息");
    }

    if(num == -1)//表示新添加记录
    {
        ui->prePushButton->hide();
        ui->nextPushButton->hide();
        int row = modle->rowCount();
        modle->insertRow(row);
        mapper->setCurrentIndex(row);
    }
    else {
        mapper->setCurrentIndex(num);
        ui->numLineEdit->setEnabled(false);
    }
    connect(ui->prePushButton,SIGNAL(clicked()),mapper,SLOT(toPrevious()));
    connect(ui->nextPushButton,SIGNAL(clicked()),mapper,SLOT(toNext()));
}

EntryForm::~EntryForm()
{
    delete ui;
    modle->select();
}

void EntryForm::initIdList()
{
    for(int row = 0;row < modle->rowCount();++row)
    {
        QSqlRecord record = modle->record(row);
        int id = record.value(NUM).toInt();
        idList.append(id);
    }
}


void EntryForm::on_numLineEdit_editingFinished()
{
    int id = ui->numLineEdit->text().toInt();
    if(idList.contains(id))
    {
        if(type == FORM_TYPE::ALARM_FORM)
        {
            ui->explainLabel->setText("防区已存在！");
            ui->pushButton->setEnabled(false);
        }
        else
        {
            ui->explainLabel->setText("分区已存在！");
            ui->pushButton->setEnabled(false);
        }
    }
    else {
        ui->explainLabel->clear();
        ui->pushButton->setEnabled(true);
    }
}

void EntryForm::on_pushButton_clicked()
{
    if(num == -1)
    {
        int id = ui->numLineEdit->text().toInt();
        idList.append(id);
        int crow = mapper->currentIndex();
        mapper->submit();
        ui->numLineEdit->clear();
        //如果是防区，还要写入防区类型
        if(type == FORM_TYPE::ALARM_FORM)
        {
            int alarm_type = alarmTypeList.at(ui->typeComboBox->currentIndex());
            modle->setData(modle->index(crow,ALARM_TYPE),alarm_type);
            modle->setData(modle->index(crow,ON_SCENCE),0);
            modle->setData(modle->index(crow,ALARM_TYPE_DES),ui->typeComboBox->currentText());
            modle->submit();
        }
        modle->select();
        int row = modle->rowCount();
        modle->insertRow(row);
        mapper->setCurrentIndex(row);
    }
    else {
        int row = mapper->currentIndex();
        mapper->submit();
        if(type == FORM_TYPE::ALARM_FORM)
        {
            int alarm_type = alarmTypeList.at(ui->typeComboBox->currentIndex());
            modle->setData(modle->index(row,ALARM_TYPE),alarm_type);
            modle->setData(modle->index(row,ALARM_TYPE_DES),ui->typeComboBox->currentText());
            modle->submit();
        }
        modle->select();
        mapper->setCurrentIndex(row +1);
    }

}
