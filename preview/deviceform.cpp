#include "deviceform.h"
#include "ui_deviceform.h"
#include "listitemdelege.h"
#include "globdata.h"
#include "cameronet.h"
#include <QMap>


enum {
    SERIAL,
    MODE,
    NAME,
    IP,
    PORT,
    ACCOUNT,
    PASSWARD,
    TYPE,
    CHANNAL_AMOUNT,
    LINE_STATE
};

QStringList DeviceForm::modeStringList = {"海康","大华"};
QStringList DeviceForm::typeStringList = {"NVR","IPC"};

DeviceForm::DeviceForm(QWidget *parent,DeviceImforSqlTableModel *m,int n) :
    QDialog(parent),
    ui(new Ui::DeviceForm),
    model(m),
    num(n),
    mapper(new QDataWidgetMapper(this))
{
    setAttribute(Qt::WA_DeleteOnClose);
    ui->setupUi(this);
    ui->portSpinBox->setMaximum(100000);

    model->select();
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);
    mapper->setModel(model);
    //mapper->addMapping(ui->modeComboBox,MODE);
    //mapper->addMapping(ui->typeComboBox,TYPE);
    mapper->addMapping(ui->channalCountSpinBox,CHANNAL_AMOUNT);
    mapper->addMapping(ui->namelineEdit,NAME);
    mapper->addMapping(ui->ipLineEdit,IP);
    mapper->addMapping(ui->portSpinBox,PORT);
    mapper->addMapping(ui->accounLineEdit,ACCOUNT);
    mapper->addMapping(ui->passwordLineEdit,PASSWARD);
    //ui->modeComboBox->setItemDelegate(new ListItemDelege(this,modeStringList));
    //ui->typeComboBox->setItemDelegate(new ListItemDelege(this,typeStringList));
    if(num == -1)//表示新添加记录
    {
        int row = model->rowCount();
        model->insertRow(row);
        model->setData(model->index(row,MODE),-1);
        model->setData(model->index(row,TYPE),-1);
        mapper->setCurrentIndex(row);
        ui->portSpinBox->setRange(2,128);
    }
    else {
        mapper->setCurrentIndex(num);
        //ui->numLineEdit->setEnabled(false);
        int mode = model->data(model->index(num,MODE),Qt::EditRole).toInt();
        ui->modeComboBox->setCurrentIndex(mode);
        int type = model->data(model->index(num,TYPE),Qt::EditRole).toInt();
        ui->typeComboBox->setCurrentIndex(type);
    }

}

DeviceForm::~DeviceForm()
{
    delete ui;
}

void DeviceForm::on_submitButton_clicked()
{
    //提交前检测输入内容
    if(ui->namelineEdit->text().isEmpty())
    {
        ui->warnmingLable->setText("设备名不能为空");
        ui->warnmingLable->adjustSize();
        return;
    }
    if(ui->ipLineEdit->text().isEmpty())
    {
        ui->warnmingLable->setText("ip地址不能为空");
        ui->warnmingLable->adjustSize();
        return;
    }
    if(ui->portSpinBox->value() == 0)
    {
        ui->warnmingLable->setText("请设置正确的端口号");
        ui->warnmingLable->adjustSize();
        return;
    }
    int row = mapper->currentIndex();
    mapper->submit();
    model->select();
    mapper->setCurrentIndex(row);
    int mode = 0;
    if(ui->modeComboBox->currentText() == "海康")
    {
        mode = 0;
    }
    else {
        mode = 1;
    }
    model->setData(model->index(row,MODE),mode);
    //mode = model->data(model->index(row + 1,MODE),Qt::EditRole).toInt();
    //ui->modeComboBox->setCurrentIndex(mode);
    int type = 0;
    if(ui->typeComboBox->currentText() == "NVR")
    {
        type = 0;
    }
    else {
        type = 1;
    }
    model->setData(model->index(row,TYPE),type);
    model->submit();
    //type = model->data(model->index(row + 1,TYPE),Qt::EditRole).toInt();
    //ui->typeComboBox->setCurrentIndex(type);
    //将改变结果写入到全局变量中
    int serial = model->data(model->index(row,SERIAL),Qt::EditRole).toInt();
    QMap<int,CameraDeviceImf>::iterator it = deviceImf.find(serial);
    CameraDeviceImf &imf = deviceImf[serial];
    imf.ip = ui->ipLineEdit->text();
    imf.name = ui->namelineEdit->text();
    imf.port = ui->portSpinBox->value();
    imf.mold = (Camero::Mold)mode;
    imf.luserId = -1;
    imf.accoumt = ui->accounLineEdit->text();
    imf.passWord = ui->passwordLineEdit->text();
    imf.channalAmount = ui->channalCountSpinBox->value();
    //修改后将重新登陆
    CameroNet *cameroNet = CameroNet::getInstance();
    cameroNet->login(imf);
    if(it == deviceImf.end())
    {
        treeModle->addItem(imf);
    }
}

void DeviceForm::on_modeComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
    {
        ui->portSpinBox->setValue(8000);
    }
        break;
    case 1:
    {
        ui->portSpinBox->setValue(37777);
    }
        break;
    default:
        break;
    }
}

void DeviceForm::on_typeComboBox_currentIndexChanged(int index)
{
    switch (index) {
    case 0:
    {
        ui->channalCountSpinBox->setRange(2,128);
    }
        break;
    case 1:
    {
        ui->channalCountSpinBox->setRange(1,1);
    }
        break;
    default:
        break;
    }
}
