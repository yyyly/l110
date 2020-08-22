#include "learndialog.h"
#include "ui_learndialog.h"
#include "comboxdatamodel.h"


LearnDialog::LearnDialog(QWidget *parent, Z1801 *z1801) :
    QDialog(parent),
    ui(new Ui::LearnDialog),
    d(z1801)
{
    ui->setupUi(this);
    ComboxDataModel *model = new ComboxDataModel(this,0);
    ui->numComboBox->setModel(model);
    connect(z1801,&Z1801::codeMessing,this,&LearnDialog::handelCodeMessing);
}

LearnDialog::~LearnDialog()
{
    if(d && d->isLogin())
    {
        d->quiteLearnCode();
    }
    delete ui;
}

void LearnDialog::on_comboBox_currentIndexChanged(int index)
{
    ComboxDataModel *model = new ComboxDataModel(this,index);
    ui->numComboBox->setModel(model);
}

void LearnDialog::on_learnPushButton_clicked()//对码
{
    if(!d)
    {
        return;
    }
    if(!d->isLogin())
    {
        return;
    }
    int type = ui->comboBox->currentIndex();
    int add = ui->numComboBox->currentData().toInt();
    d->learnCode(type,add);
}

void LearnDialog::on_deltePushButton_clicked()
{
    if(!d)
    {
        return;
    }
    if(!d->isLogin())
    {
        return;
    }
    int type = ui->comboBox->currentIndex();
    int add = ui->numComboBox->currentData().toInt();
    d->deleteCode(type,add);
}

void LearnDialog::handelCodeMessing(CodeImfor imf)
{
    ui->explainLabel->setText(imf.messing);
}
