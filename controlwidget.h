#ifndef CONTROLWIDGET_H
#define CONTROLWIDGET_H

#include <QWidget>
#include <QSqlTableModel>
#include <QTableView>

namespace Ui {
class ControlWidget;
}

class ControlWidget : public QWidget
{
    Q_OBJECT

public:
    explicit ControlWidget(QWidget *parent = nullptr);
    ~ControlWidget();
    void setModel(QSqlTableModel *m){model = m;}
    void setView(QTableView *v){view = v;}
signals:
    void stateChange(int s);
    void handleHappened();
private slots:
    void on_alarmWarmingCheckBox_stateChanged(int arg1);

    void on_deviceSystemcheckBox_stateChanged(int arg1);

    void on_handalPushButton_clicked();

    void on_fastHandlePushButton_clicked();

private:
    Ui::ControlWidget *ui;
    int state;
    QSqlTableModel *model;
    QTableView *view;//
};

#endif // CONTROLWIDGET_H
