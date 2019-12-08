#ifndef SWITCHBUTTON_H
#define SWITCHBUTTON_H

#include <QWidget>
#include <QPaintEvent>
#include <QMouseEvent>

enum Statu
{
    ON,
    OFF
};

class SwitchButton : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(Statu statu READ getStatu WRITE setStatu NOTIFY statuChanged)
public:
    explicit SwitchButton(QWidget *parent = 0);
    Statu getStatu();
    void setStatu(Statu s);
    QSize sizeHint() const;
    QSize maximumSize() const;
    QSize minimumSize() const;
signals:
    void statuChanged(Statu s);
public slots:
protected:
    void paintEvent(QPaintEvent *event);
    void mouseReleaseEvent(QMouseEvent *event);
private:
    Statu statu;
    QPixmap onPixmap;
    QPixmap offPixmap;
};

#endif // SWITCHBUTTON_H
