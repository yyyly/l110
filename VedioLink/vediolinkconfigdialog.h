#ifndef VEDIOLINKCONFIGDIALOG_H
#define VEDIOLINKCONFIGDIALOG_H

#include <QDialog>
#include <QMap>
#include "areaMapEditDiaLog/mymodel.h"
#include <QSqlTableModel>
#include <QSqlRelationalTableModel>
#include "ctablewidget.h"
#include "intlistmodle.h"
namespace Ui {
class VedioLinkConfigDialog;
}

class VedioLinkConfigDialog : public QDialog
{
    Q_OBJECT

public:
    explicit VedioLinkConfigDialog(QWidget *parent = nullptr);
    ~VedioLinkConfigDialog();

private:
    Ui::VedioLinkConfigDialog *ui;
    IntListModle *alarmModle;
    QSqlTableModel *nvrModel;
    QSqlTableModel *linkModle;
    CTableWidget *w;
    QMap<int,QString> chanmelNames;
    QPoint mousePos;
    bool mouseIsPress;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void paintEvent(QPaintEvent *event);
public slots:
    void updateChannel();
    void setItemWidget();
    void updateLinkView();
private slots:

    void on_addPushButton_clicked();
    void on_deletePushButton_clicked();
    void on_closePushButton_clicked();
};

#endif // VEDIOLINKCONFIGDIALOG_H
