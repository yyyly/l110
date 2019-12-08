#ifndef CSTATUBAR_H
#define CSTATUBAR_H

#include <QWidget>
#include <QTimer>
#include <QLineEdit>
class CStatuBar : public QWidget
{
    Q_OBJECT
public:
    explicit CStatuBar(QWidget *parent = nullptr);
    void showMessage(const QString str,int time);
signals:

public slots:
    void HideWidget();
protected:
    void paintEvent(QPaintEvent *event);
    void showEvent(QShowEvent *event);
private:
    QString message;
    QTimer *timer;
};

#endif // CSTATUBAR_H
