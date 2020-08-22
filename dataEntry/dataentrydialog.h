#ifndef DATAENTRYDIALOG_H
#define DATAENTRYDIALOG_H

#include <QDialog>
#include <QSqlTableModel>
#include "entryform.h"

namespace Ui {
class DataEntryDialog;
}

class DataEntryDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DataEntryDialog(QWidget *parent = nullptr);
    ~DataEntryDialog();

private slots:


    void on_partDelePushButton_clicked();

    void on_partAddPushButton_clicked();

    void on_partSaveButton_clicked();

    void on_alarmAddpushButton_clicked();

    void on_alarmEditPushButton_clicked();

    void on_alarmDelPushButton_clicked();

    void on_pushButton_clicked();

    void on_closePushButton_clicked();

signals:
    void deleNumList(const QList<int> list);
    void dataChange();
private:
    Ui::DataEntryDialog *ui;
    QSqlTableModel deviceModel;
    QDataWidgetMapper mapper;
    QSqlTableModel partModel;
    QSqlTableModel alarmModel;
    QPoint mousePos;
    bool mouseIsPress;
    QPoint dragPosition;   //鼠标拖动的位置
    int    edgeMargin;     //鼠标检测的边缘距离
    enum {nodir,
          top = 0x01,
          bottom = 0x02,
          left = 0x04,
          right = 0x08,
          topLeft = 0x01 | 0x04,
          topRight = 0x01 | 0x08,
          bottomLeft = 0x02 | 0x04,
          bottomRight = 0x02 | 0x08} resizeDir;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
    void mousePressEvent(QMouseEvent*event);
    void mouseMoveEvent(QMouseEvent*event);
    void mouseReleaseEvent(QMouseEvent*event);
    void showEvent(QShowEvent *);
    void closeEvent(QCloseEvent *);
    void paintEvent(QPaintEvent *event);
private:
    void testEdge();  //检测鼠标是否接近窗口边缘
};

#endif // DATAENTRYDIALOG_H
