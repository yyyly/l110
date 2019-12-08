#include "messageview.h"
#include "QVBoxLayout"
#include <QPalette>

MessageView::MessageView(QWidget *parent)
    : QWidget(parent),
      view(new QTableView)
{
    QVBoxLayout *vLayout = new QVBoxLayout;
    vLayout->addWidget(view);
    vLayout->setMargin(0);
    setLayout(vLayout);
}
