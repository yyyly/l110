#ifndef ICONDESWIDGET_H
#define ICONDESWIDGET_H

#include <QWidget>

class IconDesWidget : public QWidget
{
    Q_OBJECT
public:
    explicit IconDesWidget(QWidget *parent = nullptr);

signals:

public slots:
protected:
    void paintEvent(QPaintEvent *event);
};

#endif // ICONDESWIDGET_H
