#ifndef CHANGEACCOUNTDIDALOG_H
#define CHANGEACCOUNTDIDALOG_H

#include <QDialog>

namespace Ui {
class ChangeAccountDidalog;
}

class ChangeAccountDidalog : public QDialog
{
    Q_OBJECT

public:
    explicit ChangeAccountDidalog(QWidget *parent = nullptr);
    QString getCurrentAccount();
    ~ChangeAccountDidalog();
signals:
    void changeAccountSucceed(const QString &account);

private slots:

    void on_okPushButton_clicked();

    void on_closePushButton_clicked();

    void on_cancelPushButton_clicked();

private:
    Ui::ChangeAccountDidalog *ui;
    QPoint mousePos;
    bool mouseIsPress;
protected:
    bool eventFilter(QObject *obj, QEvent *event);
};

#endif // CHANGEACCOUNTDIDALOG_H
