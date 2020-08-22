#include "manageaccountdialog.h"
#include "ui_manageaccountdialog.h"
#include "accountform.h"
#include <QScopedPointer>
#include <QMessageBox>
#include <QMouseEvent>

manageAccountModel::manageAccountModel(QObject *parent)
    :QSqlTableModel(parent)
{

}

manageAccountModel::~manageAccountModel()
{

}

QVariant manageAccountModel::data(const QModelIndex &idx, int role) const
{
    switch (role) {
    case Qt::DisplayRole:
    {
        if(idx.column() == 3)
        {
            int t = idx.data(Qt::EditRole).toInt();
            if(t == 0)
            {
                return "否";
            }
            else
            {
                return "是";
            }
        }
        else
            return QSqlTableModel::data(idx,role);
    }
    default:
        return QSqlTableModel::data(idx,role);

    }
}

Qt::ItemFlags manageAccountModel::flags(const QModelIndex &index) const
{
    return  Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsEditable;
}

ManageAccountDialog::ManageAccountDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ManageAccountDialog)
{
    ui->setupUi(this);
    setWindowFlags(Qt::FramelessWindowHint | windowFlags());
    ui->widget->installEventFilter(this);
    setWindowTitle("管理账户");
    model.setTable("logImfor");
    model.setHeaderData(0,Qt::Horizontal,"账号");
    model.setHeaderData(1,Qt::Horizontal,"密码");
    model.setHeaderData(2,Qt::Horizontal,"名称");
    model.setHeaderData(3,Qt::Horizontal,"可分配账户");
    model.select();
    ui->tableView->setModel(&model);
    ui->tableView->setSelectionMode(QAbstractItemView::SingleSelection);
    ui->tableView->setSelectionBehavior(QAbstractItemView::SelectRows);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->closePushButton->setButtonStyle(":/image/toolbar_close.png",4);
}

ManageAccountDialog::~ManageAccountDialog()
{
    delete ui;
}

void ManageAccountDialog::on_addPushButton_clicked()
{
    AccountForm *dialog = new AccountForm(this,&model,-1);
    dialog->exec();
    delete dialog;
}

void ManageAccountDialog::on_deletePushButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    QString account = ui->tableView->model()->index(row,0).data().toString();
    if(account == "admin")
    {
        QMessageBox::warning(this,"提示","admin账户不可删除！");
        return;
    }
    int ret = QMessageBox::warning(this,"提示","请确认删除此账户",
                                   QMessageBox::Yes,
                                   QMessageBox::No);
    if(ret == QMessageBox::Yes)
    {
        model.removeRow(row);
        model.select();
    }
}

void ManageAccountDialog::on_editPushButton_clicked()
{
    int row = ui->tableView->currentIndex().row();
    if(row < 0)
    {
        QMessageBox::warning(this,"提示","请选中需要编辑的信息");
        return;
    }
    QScopedPointer<AccountForm> dialog(new AccountForm(this,&model,row));
    dialog->exec();
}

void ManageAccountDialog::on_closePushButton_clicked()
{
    reject();
    close();
}

bool ManageAccountDialog::eventFilter(QObject *obj, QEvent *event)
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
