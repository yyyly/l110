#include "contralpanel.h"
#include <QHBoxLayout>
#include <QStyleOption>
#include <QPainter>
#include <QSizePolicy>
#include "globdata.h"

ContralPanel::ContralPanel(QWidget *parent)
    : QWidget(parent),
      defenceButton(new QPushButton("布防")),
      disDefenceButton(new QPushButton("撤防")),
      setButton(new CPushButton(this)),
      edit(new SerchEdit(this))
{
    setFixedHeight(40);
    setObjectName("panel");
    setFixedWidth(200);
    defenceButton->setFixedSize(55,30);
    defenceButton->setIcon(QIcon(":/image/bf.png"));
    //defenceButton->setObjectName("cButton");
    disDefenceButton->setFixedSize(55,30);
    disDefenceButton->setIcon(QIcon(":/image/cf.png"));
    //disDefenceButton->setObjectName("cButton");
    setButton->setFixedSize(32,32);
    setButton->setButtonStyle(":/image/Setting.png",4);
    //defenceButton->setDisabled(true);
    //disDefenceButton->setDisabled(true);
    //setButton->setDisabled(true);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    if(mType == MachineType::KS_Z801A)
    {
        edit->hide();
        hLayout->addWidget(defenceButton);
        hLayout->addWidget(disDefenceButton);
        hLayout->addWidget(setButton);
    }
    else
    {
        edit->setFixedWidth(170);
        hLayout->addWidget(edit);
        hLayout->addWidget(setButton);
    }
    setLayout(hLayout);
    connect(setButton,SIGNAL(clicked()),this,SLOT(on_setButton_clicked()));
    connect(defenceButton,SIGNAL(clicked()),this,SLOT(on_defenceButton_clicked()));
    connect(disDefenceButton,SIGNAL(clicked()),this,SLOT(on_disDefenceButton_clicked()));
    connect(edit,&SerchEdit::serchStr,this,&ContralPanel::on_serch);
}

void ContralPanel::on_setButton_clicked()
{
    emit setDevice();
}

void ContralPanel::on_defenceButton_clicked()
{
    emit defence();
}

void ContralPanel::on_disDefenceButton_clicked()
{
    emit disDefence();
}

void ContralPanel::on_serch(const QString str)
{
    emit serchTest(str);
}

void ContralPanel::updateState(bool enble)
{
    defenceButton->setEnabled(enble);
    disDefenceButton->setEnabled(enble);
}

void ContralPanel::paintEvent(QPaintEvent *event)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}
