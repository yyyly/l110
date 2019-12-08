#include "serchedit.h"
#include <QHBoxLayout>
#include <QMargins>

SerchEdit::SerchEdit(QWidget *parent)
    :QLineEdit (parent),
      button(new QPushButton)
{
    button->setDisabled(true);
    button->setFixedSize(22,22);
    button->setStyleSheet("QPushButton{border-image:url(:/image/serch_able); background:transparent;} \
                          QPushButton:hover{border-image:url(:/image/serch_able)} \
                          QPushButton:pressed{border-image:url(:/image/serch_disable)} \
                          QPushButton::disabled{border-image:url(:/image/serch_disable)}");
    //限定text范围
    QMargins m = this->textMargins();
    setTextMargins(m.left(),m.top(),button->width(),m.bottom());
    setPlaceholderText("搜索。。");
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addStretch();
    hlayout->addWidget(button);
    hlayout->setMargin(0);
    hlayout->setSpacing(0);
    setLayout(hlayout);
    connect(button,SIGNAL(clicked()),this,SLOT(button_clicked()));
    connect(this,SIGNAL(textChanged(const QString)),this,SLOT(checkText(const QString)));
}

void SerchEdit::button_clicked()
{
    str = this->text();
    if(str.isEmpty())
        return;
    emit serchStr(str);
}

void SerchEdit::checkText(const QString &text)
{
    if(text.isEmpty())
        button->setEnabled(false);
    else {
        button->setEnabled(true);
    }
}
