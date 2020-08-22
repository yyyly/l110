#include "aboutdialog.h"
#include "ui_aboutdialog.h"
#include <QMouseEvent>

aboutDialog::aboutDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::aboutDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->widget->installEventFilter(this);
    ui->closePushButton->setButtonStyle(":/image/toolbar_close.png",4);
}

aboutDialog::~aboutDialog()
{
    delete ui;
}

bool aboutDialog::eventFilter(QObject *obj, QEvent *event)
{
    if(obj == ui->widget && !this->isMaximized())
    {
        QMouseEvent *mouseEvent = static_cast<QMouseEvent *>(event);
        switch(mouseEvent->type())
        {
        case QEvent::MouseButtonPress:
            {
                if(mouseEvent->button() == Qt::LeftButton)
                {
                    mouseIsPress = true;
                    mousePos = mouseEvent->pos();
                }
            }
                break;
        case QEvent::MouseMove:
            {
                move(mouseEvent->pos() - mousePos  + pos());
            }
                break;
        case QEvent::MouseButtonRelease:
            {
                mouseIsPress = false;
            }
                break;
        default:
            break;
        }
    }
    return false;
}

void aboutDialog::on_closePushButton_clicked()
{
    this->close();
}

