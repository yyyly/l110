#include "ctablewidget.h"
#include "ui_ctablewidget.h"
#include<QPalette>

CTableWidget::CTableWidget(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::CTableWidget)
{
    ui->setupUi(this);
    QPalette pal = palette();
    pal.setColor(QPalette::Background, QColor(0x00,0xff,0x00,0x00));
    setPalette(pal);
}

CTableWidget::~CTableWidget()
{
    delete ui;
}
