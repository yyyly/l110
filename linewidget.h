#ifndef LINEWIDGET_H
#define LINEWIDGET_H

#include <QWidget>

class LineWidget : public QWidget
{
    Q_OBJECT
public:
    explicit LineWidget(QWidget *parent = nullptr);
protected:
    void paintEvent(QPaintEvent *event);
signals:

public slots:
};

#endif // LINEWIDGET_H
