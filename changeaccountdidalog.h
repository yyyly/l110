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

private:
    Ui::ChangeAccountDidalog *ui;
};

#endif // CHANGEACCOUNTDIDALOG_H
