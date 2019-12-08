#include "contralpanel.h"
#include <QHBoxLayout>

ContralPanel::ContralPanel(QWidget *parent)
    : QWidget(parent),
      defenceButton(new QPushButton("布防")),
      disDefenceButton(new QPushButton("撤防")),
      setButton(new QPushButton("设置"))
{
    setFixedHeight(30);
    defenceButton->setFixedSize(40,20);
    disDefenceButton->setFixedSize(40,20);
    setButton->setFixedSize(32,20);
    defenceButton->setDisabled(true);
    disDefenceButton->setDisabled(true);
    //setButton->setDisabled(true);
    QHBoxLayout *hLayout = new QHBoxLayout(this);
    hLayout->setMargin(0);
    hLayout->addWidget(defenceButton);
    hLayout->addWidget(disDefenceButton);
    hLayout->addWidget(setButton);
    connect(setButton,SIGNAL(clicked()),this,SLOT(on_setButton_clicked()));
    connect(defenceButton,SIGNAL(clicked()),this,SLOT(on_defenceButton_clicked()));
    connect(disDefenceButton,SIGNAL(clicked()),this,SLOT(on_disDefenceButton_clicked()));
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

void ContralPanel::updateState(bool enble)
{
    defenceButton->setEnabled(enble);
    disDefenceButton->setEnabled(enble);
}
