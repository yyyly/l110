#ifndef POPWIDGET_H
#define POPWIDGET_H

#include <QWidget>
#include <QDialog>
#include <QStack>
#include <QSqlTableModel>

namespace Ui {
class PopWidget;
}

class PopWidget : public QWidget
{
    Q_OBJECT

public:
    explicit PopWidget(QWidget *parent = nullptr);
    void setModel(QSqlTableModel *m){model = m;}
    void addIndex(int alarm_key);

    ~PopWidget();
protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
    void mousePressEvent(QMouseEvent *event);
private slots:
    void on_handlePushButton_clicked();

    void on_pushButton_clicked();

private:
    Ui::PopWidget *ui;
    QSqlTableModel *model;
    QStack<int> indexStack;
};

#endif // POPWIDGET_H
