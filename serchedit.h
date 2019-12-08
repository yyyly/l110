#ifndef SERCHEDIT_H
#define SERCHEDIT_H

#include <QObject>
#include <QLineEdit>
#include <QPushButton>

class SerchEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit SerchEdit(QWidget *parent = nullptr);
private:
    QPushButton *button;
    QString str;
signals:
    void serchStr(const QString &str);
public slots:
    void button_clicked();
    void checkText(const QString &text);
};

#endif // SERCHEDIT_H
