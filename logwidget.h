#ifndef LOGWIDGET_H
#define LOGWIDGET_H

#include <QDialog>
#include <QTimer>

namespace Ui {
class logWidget;
}

class logWidget : public QDialog
{
    Q_OBJECT

public:
    explicit logWidget(QWidget *parent = nullptr);
    void update(bool isSaved,bool isAutoLog,QString account,QString passward);
    ~logWidget();

public slots:
    bool on_logButton_clicked();

    void on_closeButton_clicked();
protected:
    bool eventFilter(QObject *obj, QEvent *event);

private slots:
    void on_toMinButton_clicked();

private:
    Ui::logWidget *ui;
    void setButtonStyle(QPushButton *button, QString imgsrc, int CutSec);
    QTimer *timer;
    QPoint mousePos;
    bool mouseIsPress;
};

#endif // LOGWIDGET_H
