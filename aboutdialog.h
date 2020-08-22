#ifndef ABOUTDIALOG_H
#define ABOUTDIALOG_H

#include <QDialog>

namespace Ui {
class aboutDialog;
}

class aboutDialog : public QDialog
{
    Q_OBJECT

public:
    explicit aboutDialog(QWidget *parent = nullptr);
    ~aboutDialog();

private:
    Ui::aboutDialog *ui;
    QPoint mousePos;
    bool mouseIsPress;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
private slots:
    void on_closePushButton_clicked();
};

#endif // ABOUTDIALOG_H
